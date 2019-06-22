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

#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "CalibMuon/DTDigiSync/interface/DTTTrigSyncFactory.h"

DTNtupleConfig::DTNtupleConfig(const edm::ParameterSet & config) 
{ 

  edm::InputTag none = edm::InputTag("none");

  m_inputTags["genPartTag"] = config.getUntrackedParameter<edm::InputTag>("genPartTag", none);

  m_inputTags["puInfoTag"] = config.getUntrackedParameter<edm::InputTag>("puInfoTag", none);
  m_inputTags["lumiScalerTag"] = config.getUntrackedParameter<edm::InputTag>("lumiScalerTag", none);
  m_inputTags["primaryVerticesTag"] = config.getUntrackedParameter<edm::InputTag>("primaryVerticesTag", none);

  m_inputTags["ph1DtDigiTag"] = config.getUntrackedParameter<edm::InputTag>("ph1DtDigiTag", none);
  m_inputTags["ph2DtDigiTag"] = config.getUntrackedParameter<edm::InputTag>("ph2DtDigiTag", none);

  m_inputTags["ph1DtSegmentTag"] = config.getUntrackedParameter<edm::InputTag>("ph1DtSegmentTag", none);
  m_inputTags["ph2DtSegmentTag"] = config.getUntrackedParameter<edm::InputTag>("ph2DtSegmentTag", none);

  m_inputTags["ph1TwinMuxInTag"] = config.getUntrackedParameter<edm::InputTag>("ph1TwinMuxInTag", none);
  m_inputTags["ph1TwinMuxOutTag"] = config.getUntrackedParameter<edm::InputTag>("ph1TwinMuxOutTag", none);
  m_inputTags["ph1BmtfInTag"] = config.getUntrackedParameter<edm::InputTag>("ph1BmtfInTag", none);

  m_inputTags["ph1TwinMuxInThTag"] = config.getUntrackedParameter<edm::InputTag>("ph1TwinMuxInThTag", none);
  m_inputTags["ph1BmtfInThTag"] = config.getUntrackedParameter<edm::InputTag>("ph1BmtfInThTag", none);

  m_inputTags["ph2TPGPhiHwTag"] = config.getUntrackedParameter<edm::InputTag>("ph2TPGPhiHwTag", none);
  m_inputTags["ph2TPGPhiEmuHbTag"] = config.getUntrackedParameter<edm::InputTag>("ph2TPGPhiEmuHbTag", none);
  m_inputTags["ph2TPGPhiEmuAmTag"] = config.getUntrackedParameter<edm::InputTag>("ph2TPGPhiEmuAmTag", none);

  m_dtSync = DTTTrigSyncFactory::get()->create(config.getUntrackedParameter<std::string>("tTrigMode"),
					       config.getUntrackedParameter<edm::ParameterSet>("tTrigModeConfig"));

}

void DTNtupleConfig::getES(const edm::EventSetup & environment) 
{ 

  m_dtSync->setES(environment);

  environment.get<MuonGeometryRecord>().get(m_dtGeometry);
  environment.get<GlobalTrackingGeometryRecord>().get(m_trackingGeometry);

  edm::ESHandle<DTGeometry> dtIdealGeom; 
  environment.get<MuonGeometryRecord>().get("idealForDigi",dtIdealGeom);

  m_trigGeomUtils.reset();
  m_trigGeomUtils = std::make_unique<DTTrigGeomUtils>(dtIdealGeom);

}
