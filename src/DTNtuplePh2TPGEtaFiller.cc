/** \class DTNtuplePh2TPGEtaFiller DTNtuplePh2TPGEtaFiller.cc DTDPGAnalysis/DTNtuples/src/DTNtuplePh2TPGEtaFiller.cc
 *  
 * Helper class : the Phase-1 local trigger filler for TwinMux in/out and BMTF in (the DataFormat is the same)
 *
 * \author L. Borgonovi (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtuplePh2TPGEtaFiller.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include <iostream>


DTNtuplePh2TPGEtaFiller::DTNtuplePh2TPGEtaFiller(edm::ConsumesCollector && collector,
						 const std::shared_ptr<DTNtupleConfig> config,
						 std::shared_ptr<TTree> tree, const std::string & label,
						 TriggerTag tag) :
  DTNtupleBaseFiller(config, tree, label), m_tag(tag)
{
  edm::InputTag iTag;

  switch (m_tag)
    {
    case TriggerTag::HW :
      iTag = m_config->m_inputTags["ph2TPGEtaHwTag"];
      break;
    case TriggerTag::AM :
      iTag = m_config->m_inputTags["ph2TPGEtaEmuAmTag"];
    }

  if (iTag.label() != "none") m_dtTriggerToken = collector.consumes<L1Phase2MuDTEtaContainer>(iTag);
}

DTNtuplePh2TPGEtaFiller::~DTNtuplePh2TPGEtaFiller() 
{ 

};

void DTNtuplePh2TPGEtaFiller::initialize()
{
  
  m_tree->Branch((m_label + "_nTrigs").c_str(), &m_nTrigs, (m_label + "_nTrigs/i").c_str());
  
  m_tree->Branch((m_label + "_wheel").c_str(),   &m_lt_wheel);
  m_tree->Branch((m_label + "_sector").c_str(),  &m_lt_sector);
  m_tree->Branch((m_label + "_station").c_str(), &m_lt_station);

  m_tree->Branch((m_label + "_quality").c_str(), &m_lt_quality);

  m_tree->Branch((m_label + "_rpcFlag").c_str(), &m_lt_rpcFlag);
  m_tree->Branch((m_label + "_chi2").c_str(),    &m_lt_chi2);
  
  m_tree->Branch((m_label + "_BX").c_str(),    &m_lt_bx);
  m_tree->Branch((m_label + "_t0").c_str(),    &m_lt_t0);

  m_tree->Branch((m_label + "_index").c_str(),    &m_lt_index);

  m_tree->Branch((m_label + "_z").c_str(),    &m_lt_z);
  m_tree->Branch((m_label + "_k").c_str(),    &m_lt_k);
  
}

void DTNtuplePh2TPGEtaFiller::clear()
{

  m_nTrigs = 0;

  m_lt_wheel.clear();
  m_lt_sector.clear();
  m_lt_station.clear();

  m_lt_quality.clear();

  m_lt_rpcFlag.clear();
  m_lt_chi2.clear();

  m_lt_bx.clear();
  m_lt_t0.clear();

  m_lt_index.clear();

  m_lt_z.clear();
  m_lt_k.clear();

}

void DTNtuplePh2TPGEtaFiller::fill(const edm::Event & ev)
{

  clear();

  auto trigColl = conditionalGet<L1Phase2MuDTEtaContainer>(ev, m_dtTriggerToken,"L1Phase2MuDTEtaContainer");

  if (trigColl.isValid()) 
    {      
      const auto trigs = trigColl->getContainer();
      for(const auto & trig : (*trigs))
	{

	  m_lt_wheel.push_back(trig.whNum());
	  m_lt_sector.push_back(trig.scNum() + 1); 
	  m_lt_station.push_back(trig.stNum());
	  
	  m_lt_quality.push_back(trig.quality());

	  m_lt_rpcFlag.push_back(trig.rpcFlag());
	  m_lt_chi2.push_back(trig.chi2());
	  
	  m_lt_bx.push_back(trig.bxNum());
	  m_lt_t0.push_back(trig.t0());
	  
	  m_lt_index.push_back(trig.index());

	  m_lt_z.push_back(trig.z());
	  m_lt_k.push_back(trig.k());
	  
	  m_nTrigs++;
	
	}
    }
  
  return;

}

