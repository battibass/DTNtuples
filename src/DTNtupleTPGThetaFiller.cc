/** \class DTNtupleTPGThetaFiller DTNtupleTPGThetaFiller.cc DTDPGAnalysis/DTNtuples/src/DTNtupleTPGThetaFiller.cc
 *  
 * Helper class : the Phase-1 local trigger filler for TwinMux in/out and BMTF in (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleTPGThetaFiller.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include <iostream>

DTNtupleTPGThetaFiller::DTNtupleTPGThetaFiller(edm::ConsumesCollector && collector,
					   const std::shared_ptr<DTNtupleConfig> config, 
					   std::shared_ptr<TTree> tree, const std::string & label,
					   TriggerTag tag) : 
  DTNtupleBaseFiller(config, tree, label), m_tag(tag)
{

  edm::InputTag iTag;

  switch (m_tag)
    {
    case TriggerTag::TM_IN :
      iTag = m_config->m_inputTags["ph1TwinMuxInThTag"];
      break;
    case TriggerTag::BMTF_IN :
      iTag = m_config->m_inputTags["ph1BmtfInThTag"];
    }

  if (iTag.label() != "none") m_dtTriggerToken = collector.consumes<L1MuDTChambThContainer>(iTag);

}

DTNtupleTPGThetaFiller::~DTNtupleTPGThetaFiller() 
{ 

};

void DTNtupleTPGThetaFiller::initialize()
{
  
  m_tree->Branch((m_label + "_nTrigs").c_str(), &m_nTrigs, (m_label + "_nTrigs/i").c_str());
  
  m_tree->Branch((m_label + "_wheel").c_str(),   &m_lt_wheel);
  m_tree->Branch((m_label + "_sector").c_str(),  &m_lt_sector);
  m_tree->Branch((m_label + "_station").c_str(), &m_lt_station);

  m_tree->Branch((m_label + "_BX").c_str(),    &m_lt_bx);

  m_tree->Branch((m_label + "_hitMap").c_str(), &m_lt_hitMap);
  
}

void DTNtupleTPGThetaFiller::clear()
{

  m_nTrigs = 0;

  m_lt_wheel.clear();
  m_lt_sector.clear();
  m_lt_station.clear();

  m_lt_bx.clear();

  m_lt_hitMap.clear();

}

void DTNtupleTPGThetaFiller::fill(const edm::Event & ev)
{

  clear();

  auto trigColl = conditionalGet<L1MuDTChambThContainer>(ev, m_dtTriggerToken,"L1MuDTChambThContainer");

  if (trigColl.isValid()) 
    {      
      const auto trigs = trigColl->getContainer();
      for(const auto & trig : (*trigs))
	{

	  bool hasData = false;
	  for (int pos = 0; pos < 7; ++pos)
	    { 
	      if (trig.code(pos))
		{ 
		  hasData = true;
		  break;
		}
	    }

	  if (!hasData)
	    continue;

	  m_lt_wheel.push_back(trig.whNum());
	  m_lt_sector.push_back(trig.scNum() + (m_tag != TriggerTag::BMTF_IN ? 1 : 0)); 
	  m_lt_station.push_back(trig.stNum());
	  
	  m_lt_bx.push_back(trig.bxNum());
	  
	  unsigned short hitMap = 0;
	  for (int pos = 0; pos < 7; ++pos)
	    if (trig.code(pos))
	      hitMap=hitMap | (0x1 << pos);
	  
	  m_lt_hitMap.push_back(hitMap);
	  
	  m_nTrigs++;

	}
    }
  
  return;

}

