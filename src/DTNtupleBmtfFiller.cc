/** \class DTNtupleBmtfFiller DTNtupleBmtfFiller.cc DTDPGAnalysis/DTNtuples/src/DTNtupleBmtfFiller.cc
 *  
 * Helper class : the bmtf filler
 *
 * \author L. Borgonovi (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBmtfFiller.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include <iostream>

#include "TClonesArray.h"
#include "TVectorF.h"

DTNtupleBmtfFiller::DTNtupleBmtfFiller(edm::ConsumesCollector && collector,
				     const std::shared_ptr<DTNtupleConfig> config, 
				     std::shared_ptr<TTree> tree, const std::string & label) : 
  DTNtupleBaseFiller(config, tree, label)
{

  edm::InputTag & iTag = m_config->m_inputTags["ph1BmtfOutTag"];
  if (iTag.label() != "none") m_dtBmtfToken = collector.consumes<l1t::RegionalMuonCandBxCollection>(iTag);

  edm::InputTag & iTag2 = m_config->m_inputTags["ph1BmtfInTag"];
  if (iTag2.label() != "none") m_dtTpgPhiToken = collector.consumes<L1MuDTChambPhContainer>(iTag2);

}

DTNtupleBmtfFiller::~DTNtupleBmtfFiller() 
{ 

};

void DTNtupleBmtfFiller::initialize()
{
  m_muCandStubs = new TClonesArray("TVectorF", 50);  

  m_tree->Branch((m_label + "_nMuCands").c_str(), &m_nMuCands, (m_label + "_nMuCands/i").c_str());

  m_tree->Branch((m_label + "_Pt").c_str(),     &m_tf_pt);
  m_tree->Branch((m_label + "_Phi").c_str(),    &m_tf_phi);
  m_tree->Branch((m_label + "_Eta").c_str(),    &m_tf_eta);
  m_tree->Branch((m_label + "_DXY").c_str(),    &m_tf_dxy);
  m_tree->Branch((m_label + "_Qual").c_str(),   &m_tf_qual);
  m_tree->Branch((m_label + "_HF").c_str(),     &m_tf_hf);
  m_tree->Branch((m_label + "_TriggerStub").c_str(), &m_muCandStubs,  2048000,0);
}

void DTNtupleBmtfFiller::clear()
{

  m_nMuCands = 0;

  m_tf_pt.clear();
  m_tf_phi.clear();
  m_tf_eta.clear();
  m_tf_dxy.clear();
  m_tf_qual.clear();
  m_tf_hf.clear();
  m_muCandStubs->Clear();

}

void DTNtupleBmtfFiller::fill(const edm::Event & ev)
{

  clear();
  int nTrigs = 0;
  TVectorF triggerIndex(4);

  auto bmtfColl = conditionalGet<l1t::RegionalMuonCandBxCollection>(ev, m_dtBmtfToken,"RegionalMuonCandBxCollection");
  auto trigColl = conditionalGet<L1MuDTChambPhContainer>(ev, m_dtTpgPhiToken, "L1MuDTChambPhContainer");
  
  if (bmtfColl.isValid()) 
    {

      auto muCandBX  = bmtfColl->getFirstBX();
      auto muCandLastBX = bmtfColl->getLastBX();

      for (; muCandBX <= muCandLastBX; ++muCandBX)
	{
	  auto muCand = bmtfColl->begin(muCandBX);
	  auto muCandLast = bmtfColl->end(muCandBX);

	  for (;  muCand != muCandLast; ++muCand)
	    {
	      std::map<int, int> mapTA = muCand->trackAddress();
	     
	      int wsign = mapTA[0] == 0 ? +1 : -1 ;
	      int wheel = wsign * mapTA[1];
	      int sector = muCand->processor();

	      int ts_mb[4];
	      int w_mb[4];
	      int s_mb[4];

	      for(int i = 0; i < 4; i++)
                {
		  w_mb[i] = -10;
                  s_mb[i] = -1;
                  ts_mb[i] = -1;
		  triggerIndex[i] = -1;
		}
	      
	      if (mapTA[2] != 3)
		{
		  w_mb[0] = wheel;
		  s_mb[0] = sector;
		  ts_mb[0] = ( mapTA[2] & 1 ) ? 1 : 0;
		}
	      
	      for(int i = 1; i < 4; i++)
		{
		  if (mapTA[i+2] != 15)
		    {
		      ts_mb[i] = ( mapTA[i+2] & 1 ) ? 1 : 0;   // 0 for ts1 , 1 for ts2 
		      w_mb[i]  = ( mapTA[i+2] & 8 ) ? wheel : wheel+wsign;  // own wheel if true, nex wheel if false (depends on sign of the wheel -> case 0+, +1: +1, case 0-, -1: -1)
		      int temp_mapValue = mapTA[i+2] >> 1;  // temp value to remove less significant bit
		      if (temp_mapValue & 1) s_mb[i] = sector != 0 ? sector-1 : 11  ; // if last two remained bit == 01 -> sector-1 (N+1 column of ref table)
		      else if (temp_mapValue & 2) s_mb[i] = sector != 11 ? sector+1 : 0; //  if last two remained bit == 10 -> sector+1 (N-1 column of ref table)
		      else s_mb[i] = sector;  // if last two remained bit == 00 -> sector (N column of ref table)
		    }
		}

	      int temp_index = 0;		      	      	      
	      for(int i = 0; i < 4; i++)
		{
		  nTrigs = 0;
		  if (trigColl.isValid())
		    {
		      const auto trigs = trigColl->getContainer();
		      for(const auto & trig : (*trigs))
			{
			  if (trig.code() != 7)
			    {
			      if (muCandBX == trig.bxNum())
				{
				  if ( (w_mb[i] == trig.whNum()) && (ts_mb[i] == trig.Ts2Tag()) && (s_mb[i] == trig.scNum()) && (i+1 == trig.stNum() ) )
				    {
				      triggerIndex[temp_index] = nTrigs;
				      temp_index++;
				    }
				}
			    }
                          nTrigs++;
                        }
                    }
                }
	      
	      m_tf_pt.push_back((muCand->hwPt())*0.5);
              m_tf_phi.push_back(muCand->hwPhi()); //no conversion yet
              m_tf_eta.push_back((muCand->hwEta())*0.010875);
	      m_tf_dxy.push_back(muCand->hwDXY()); 
	      m_tf_qual.push_back(muCand->hwQual());
	      m_tf_hf.push_back(muCand->hwHF());
	      new ((*m_muCandStubs)[m_nMuCands]) TVectorF(triggerIndex);

	      m_nMuCands++;
	    }
	}
    }
  return;
}



