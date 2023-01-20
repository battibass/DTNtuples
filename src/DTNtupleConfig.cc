/** \class DTNtupleConfig DTNtupleConfig.cc DTDPGAnalysis/DTNtuples/src/DTNtupleConfig.cc
 *  
 * Helper class to handle :
 * - configuration parameters for edm::ParameterSet
 * - DB information from edm::EventSetup
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleConfig.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Run.h"

#include "CalibMuon/DTDigiSync/interface/DTTTrigSyncFactory.h"

#include "TString.h"
#include "TRegexp.h"

DTNtupleConfig::DTNtupleConfig(const edm::ParameterSet &config, edm::ConsumesCollector &&collector)
    : m_trackingGeomToken{collector.esConsumes<>()},
      m_dtGeomToken{collector.esConsumes<edm::Transition::BeginRun>()},
      m_dtIdealGeomToken{collector.esConsumes<edm::Transition::BeginRun>(edm::ESInputTag("", "idealForDigi"))} {
  edm::InputTag none = edm::InputTag("none");

  m_inputTags["genPartTag"] = config.getUntrackedParameter<edm::InputTag>("genPartTag", none);

  m_inputTags["dtFedBxTag"] = config.getUntrackedParameter<edm::InputTag>("dtFedBxTag", none);

  m_inputTags["puInfoTag"]          = config.getUntrackedParameter<edm::InputTag>("puInfoTag", none);
  m_inputTags["lumiScalerTag"]      = config.getUntrackedParameter<edm::InputTag>("lumiScalerTag", none);
  m_inputTags["lumiScalerTag2017"]  = config.getUntrackedParameter<edm::InputTag>("lumiScalerTag2017", none);
  m_inputTags["primaryVerticesTag"] = config.getUntrackedParameter<edm::InputTag>("primaryVerticesTag", none);

  m_inputTags["ph1DtDigiTag"] = config.getUntrackedParameter<edm::InputTag>("ph1DtDigiTag", none);
  m_inputTags["ph2DtDigiTag"] = config.getUntrackedParameter<edm::InputTag>("ph2DtDigiTag", none);

  m_inputTags["ph1DtSegmentTag"] = config.getUntrackedParameter<edm::InputTag>("ph1DtSegmentTag", none);
  m_inputTags["ph2DtSegmentTag"] = config.getUntrackedParameter<edm::InputTag>("ph2DtSegmentTag", none);

  m_inputTags["muonTag"] = config.getUntrackedParameter<edm::InputTag>("muonTag", none);

  m_inputTags["trigResultsTag"] = config.getUntrackedParameter<edm::InputTag>("trigResultsTag", none);
  m_inputTags["trigEventTag"] = config.getUntrackedParameter<edm::InputTag>("trigEventTag", none);

  m_inputTags["ph1TwinMuxInTag"] = config.getUntrackedParameter<edm::InputTag>("ph1TwinMuxInTag", none);
  m_inputTags["ph1TwinMuxOutTag"] = config.getUntrackedParameter<edm::InputTag>("ph1TwinMuxOutTag", none);
  m_inputTags["ph1BmtfInTag"] = config.getUntrackedParameter<edm::InputTag>("ph1BmtfInTag", none);

  m_inputTags["ph1TwinMuxInThTag"] = config.getUntrackedParameter<edm::InputTag>("ph1TwinMuxInThTag", none);
  m_inputTags["ph1BmtfInThTag"] = config.getUntrackedParameter<edm::InputTag>("ph1BmtfInThTag", none);

  m_inputTags["ph2TPGPhiHwTag"] = config.getUntrackedParameter<edm::InputTag>("ph2TPGPhiHwTag", none);
  m_inputTags["ph2TPGPhiEmuHbTag"] = config.getUntrackedParameter<edm::InputTag>("ph2TPGPhiEmuHbTag", none);
  m_inputTags["ph2TPGPhiEmuAmTag"] = config.getUntrackedParameter<edm::InputTag>("ph2TPGPhiEmuAmTag", none);

  m_inputTags["ph2TPGThHwTag"] = config.getUntrackedParameter<edm::InputTag>("ph2TPGThHwTag", none);
  m_inputTags["ph2TPGThEmuAmTag"] = config.getUntrackedParameter<edm::InputTag>("ph2TPGThEmuAmTag", none);

  m_inputTags["ph1BmtfOutTag"] = config.getUntrackedParameter<edm::InputTag>("ph1BmtfOutTag", none);

  if (m_inputTags["ph1DtSegmentTag"].label() != "none")
    m_dtSyncs[PhaseTag::PH1] =
        DTTTrigSyncFactory::get()->create(config.getUntrackedParameter<std::string>("ph1tTrigMode"),
                                          config.getUntrackedParameter<edm::ParameterSet>("ph1tTrigModeConfig"),
                                          collector);

  if (m_inputTags["ph2DtSegmentTag"].label() != "none")
    m_dtSyncs[PhaseTag::PH2] =
        DTTTrigSyncFactory::get()->create(config.getUntrackedParameter<std::string>("ph2tTrigMode"),
                                          config.getUntrackedParameter<edm::ParameterSet>("ph2tTrigModeConfig"),
                                          collector);

  m_isoTrigName = config.getUntrackedParameter<std::string>("isoTrigName", "HLT_IsoMu24_v*");
  m_trigName = config.getUntrackedParameter<std::string>("trigName", "HLT_Mu50_v*");
}

void DTNtupleConfig::getES(const edm::EventSetup &environment) {
  m_trackingGeometry = environment.getHandle(m_trackingGeomToken);

  if (m_inputTags["ph1DtSegmentTag"].label() != "none")
    m_dtSyncs[PhaseTag::PH1]->setES(environment);

  if (m_inputTags["ph2DtSegmentTag"].label() != "none")
    m_dtSyncs[PhaseTag::PH2]->setES(environment);
}

void DTNtupleConfig::getES(const edm::Run &run, const edm::EventSetup &environment) {
  m_dtGeometry = environment.getHandle(m_dtGeomToken);

  const auto dtIdealGeom = environment.getHandle(m_dtIdealGeomToken);

  m_trigGeomUtils.reset();
  m_trigGeomUtils = std::make_unique<DTTrigGeomUtils>(dtIdealGeom);

  // getES(environment);

  bool changed = true;
  m_hltConfig.init(run, environment, "HLT", changed);

  bool enableWildcard = true;

  TString tName = TString(m_trigName);
  TRegexp tNamePattern = TRegexp(tName, enableWildcard);

  for (unsigned iPath = 0; iPath < m_hltConfig.size(); ++iPath) {
    TString pathName = TString(m_hltConfig.triggerName(iPath));
    if (pathName.Contains(tNamePattern))
      m_trigIndices.push_back(static_cast<int>(iPath));
  }

  tName = TString(m_isoTrigName);
  tNamePattern = TRegexp(tName, enableWildcard);

  for (unsigned iPath = 0; iPath < m_hltConfig.size(); ++iPath) {
    TString pathName = TString(m_hltConfig.triggerName(iPath));
    if (pathName.Contains(tNamePattern))
      m_isoTrigIndices.push_back(static_cast<int>(iPath));
  }
}
