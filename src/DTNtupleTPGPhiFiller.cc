/** \class DTNtupleTPGPhiFiller DTNtupleTPGPhiFiller.cc DTDPGAnalysis/DTNtuples/src/DTNtupleTPGPhiFiller.cc
 *  
 * Helper class : the Phase-1 local trigger filler for TwinMux in/out and BMTF in (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleTPGPhiFiller.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include <iostream>

DTNtupleTPGPhiFiller::DTNtupleTPGPhiFiller(edm::ConsumesCollector && collector,
					   const std::shared_ptr<DTNtupleConfig> config, 
					   std::shared_ptr<TTree> tree, const std::string & label,
					   TriggerTag tag) : 
  DTNtupleBaseFiller(config, tree, label), m_tag(tag)
{

  edm::InputTag iTag;

  switch (m_tag)
    {
    case TriggerTag::TM_IN :
      iTag = m_config->m_inputTags["ph1TwinMuxInTag"];
      break;
    case TriggerTag::TM_OUT :
      iTag = m_config->m_inputTags["ph1TwinMuxOutTag"];
      break;
    case TriggerTag::BMTF_IN :
      iTag = m_config->m_inputTags["ph1BmtfInTag"];
    }

  if (iTag.label() != "none") m_dtTriggerToken = collector.consumes<L1MuDTChambPhContainer>(iTag);

}

DTNtupleTPGPhiFiller::~DTNtupleTPGPhiFiller() 
{ 

};

void DTNtupleTPGPhiFiller::initialize()
{
  
  m_tree->Branch((m_label + "_nTrigs").c_str(), &m_nTrigs, (m_label + "_nTrigs/i").c_str());
  
  m_tree->Branch((m_label + "_wheel").c_str(),   &m_lt_wheel);
  m_tree->Branch((m_label + "_sector").c_str(),  &m_lt_sector);
  m_tree->Branch((m_label + "_station").c_str(), &m_lt_station);

  m_tree->Branch((m_label + "_quality").c_str(), &m_lt_quality);

  if (m_tag == TriggerTag::TM_OUT)
    m_tree->Branch((m_label + "_rpcBit").c_str(),  &m_lt_rpcBit);

  m_tree->Branch((m_label + "_phi").c_str(),  &m_lt_phi);
  m_tree->Branch((m_label + "_phiB").c_str(), &m_lt_phiB);

  m_tree->Branch((m_label + "_posLoc_x").c_str(),  &m_lt_posLoc_x);
  m_tree->Branch((m_label + "_dirLoc_phi").c_str(), &m_lt_dirLoc_phi);

  m_tree->Branch((m_label + "_BX").c_str(),    &m_lt_bx);
  m_tree->Branch((m_label + "_is2nd").c_str(), &m_lt_is2nd);
  
}

void DTNtupleTPGPhiFiller::clear()
{

  m_nTrigs = 0;

  m_lt_wheel.clear();
  m_lt_sector.clear();
  m_lt_station.clear();

  m_lt_quality.clear();

  if (m_tag == TriggerTag::TM_OUT)
    m_lt_rpcBit.clear();

  m_lt_phi.clear();
  m_lt_phiB.clear();

  m_lt_posLoc_x.clear();
  m_lt_dirLoc_phi.clear();

  m_lt_bx.clear();
  m_lt_is2nd.clear();

}

void DTNtupleTPGPhiFiller::fill(const edm::Event & ev)
{

  clear();

  auto trigColl = conditionalGet<L1MuDTChambPhContainer>(ev, m_dtTriggerToken,"L1MuDTChambPhContainer");

  if (trigColl.isValid()) 
    {      
      const auto trigs = trigColl->getContainer();
      for(const auto & trig : (*trigs))
	{
	  if (trig.code() != 7)
	    {
	      m_lt_wheel.push_back(trig.whNum());
	      m_lt_sector.push_back(trig.scNum() + (m_tag != TriggerTag::BMTF_IN ? 1 : 0)); 
	      m_lt_station.push_back(trig.stNum());

	      m_lt_quality.push_back(trig.code());

	      if (m_tag == TriggerTag::TM_OUT)
		m_lt_rpcBit.push_back(trig.RpcBit());

	      m_lt_phi.push_back(trig.phi());
	      m_lt_phiB.push_back(trig.phiB());

	      m_lt_posLoc_x.push_back(m_config->m_trigGeomUtils->trigPos(&trig));
	      m_lt_dirLoc_phi.push_back(m_config->m_trigGeomUtils->trigDir(&trig));
	  
	      m_lt_bx.push_back(trig.bxNum() - (m_tag == TriggerTag::TM_IN && trig.Ts2Tag() ? 1 : 0));
	      m_lt_is2nd.push_back(trig.Ts2Tag());

	      m_nTrigs++;
	    }
	}
    }
  
  return;

}

