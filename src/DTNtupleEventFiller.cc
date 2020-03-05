/** \class DTNtupleEventFiller DTNtupleEventFiller.cc DTDPGAnalysis/DTNtuples/src/DTNtupleEventFiller.cc
 *  
 * Helper class : the digi filler for Phase-1 / Phase2 digis (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleEventFiller.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

DTNtupleEventFiller::DTNtupleEventFiller(edm::ConsumesCollector && collector,
					 const std::shared_ptr<DTNtupleConfig> config, 
					 std::shared_ptr<TTree> tree, const std::string & label) : 
  DTNtupleBaseFiller(config, tree, label)
{

  edm::InputTag iTag = m_config->m_inputTags["dtFedBxTag"];
  if (iTag.label() != "none") 
    m_dtFedBxToken = collector.consumes<int>(iTag);

}

DTNtupleEventFiller::~DTNtupleEventFiller() 
{ 

};

void DTNtupleEventFiller::initialize()
{
  
  m_tree->Branch((m_label + "_runNumber").c_str(), &m_runNumber, (m_label + "_runNumber/I").c_str());
  m_tree->Branch((m_label + "_lumiBlock").c_str(), &m_lumiBlock, (m_label + "_lumiBlock/I").c_str());
  m_tree->Branch((m_label + "_eventNumber").c_str(), &m_eventNumber, (m_label + "_eventNumber/L").c_str());
  
  m_tree->Branch((m_label + "_timeStamp").c_str(), &m_timeStamp, (m_label + "_timeStamp/l").c_str());

  m_tree->Branch((m_label + "_bunchCrossing").c_str(), &m_bunchCrossing, (m_label + "_bunchCrossing/I").c_str());
  m_tree->Branch((m_label + "_orbitNumber").c_str(), &m_orbitNumber, (m_label + "_orbitNumber/L").c_str());
  
}

void DTNtupleEventFiller::clear()
{

  m_runNumber   = DTNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_lumiBlock   = DTNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_eventNumber = DTNtupleBaseFiller::DEFAULT_INT_VAL_POS;

  m_timeStamp   = 0;
  
  m_bunchCrossing = DTNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_orbitNumber   = DTNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  
}

void DTNtupleEventFiller::fill(const edm::Event & ev)
{

  clear();

  m_runNumber   = ev.run();
  m_lumiBlock   = ev.getLuminosityBlock().luminosityBlock();
  m_eventNumber = ev.eventAuxiliary().event();

  m_timeStamp = ev.eventAuxiliary().time().value();


  auto dtFedBx = conditionalGet<int>(ev, m_dtFedBxToken, "int");

  m_bunchCrossing = dtFedBx.isValid() ? (*dtFedBx) : ev.eventAuxiliary().bunchCrossing();;

  m_orbitNumber   = ev.eventAuxiliary().orbitNumber();
  
  return;

}
