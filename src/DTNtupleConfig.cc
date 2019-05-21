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

DTNtupleConfig::DTNtupleConfig(const edm::ParameterSet & config) 
{ 

  edm::InputTag none = edm::InputTag("none");

  m_inputTags["ph1DtDigiTag"] = config.getUntrackedParameter<edm::InputTag>("ph1DtDigiTag", none);
  m_inputTags["ph2DtDigiTag"] = config.getUntrackedParameter<edm::InputTag>("ph2DtDigiTag", none);

  m_inputTags["ph1TwinMuxInTag"] = config.getUntrackedParameter<edm::InputTag>("ph1TwinMuxInTag", none);
  m_inputTags["ph1TwinMuxOutTag"] = config.getUntrackedParameter<edm::InputTag>("ph1TwinMuxOutTag", none);
  m_inputTags["ph1BmtfInTag"] = config.getUntrackedParameter<edm::InputTag>("ph1BmtfInTag", none);

  m_inputTags["ph1TwinMuxInThTag"] = config.getUntrackedParameter<edm::InputTag>("ph1TwinMuxInThTag", none);
  m_inputTags["ph1BmtfInThTag"] = config.getUntrackedParameter<edm::InputTag>("ph1BmtfInThTag", none);

  m_inputTags["ph2TPGPhiHwTag"] = config.getUntrackedParameter<edm::InputTag>("ph2TPGPhiHwTag", none);
  m_inputTags["ph2TPGPhiEmuHbTag"] = config.getUntrackedParameter<edm::InputTag>("ph2TPGPhiEmuHbTag", none);
  m_inputTags["ph2TPGPhiEmuAmTag"] = config.getUntrackedParameter<edm::InputTag>("ph2TPGPhiEmuAmTag", none);

}

void DTNtupleConfig::getES(const edm::EventSetup & environment) 
{ 

}
