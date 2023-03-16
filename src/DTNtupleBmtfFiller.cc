/** \class DTNtupleBmtfFiller DTNtupleBmtfFiller.cc DTDPGAnalysis/DTNtuples/src/DTNtupleBmtfFiller.cc
 *  
 * Helper class : the BMTF filler
 *
 * \author L. Borgonovi (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBmtfFiller.h"

#include "FWCore/Framework/interface/Event.h"
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

  iTag = m_config->m_inputTags["ph1BmtfInTag"];
  if (iTag.label() != "none") m_dtTpgPhiToken = collector.consumes<L1MuDTChambPhContainer>(iTag);

}

DTNtupleBmtfFiller::~DTNtupleBmtfFiller() 
{ 

};

void DTNtupleBmtfFiller::initialize()
{

  m_matchedTpIdx = new TClonesArray("TVectorF", 4);  

  m_tree->Branch((m_label + "_nBmtfCands").c_str(), &m_nBmtfCands, (m_label + "_nBmtfCands/i").c_str());

  m_tree->Branch((m_label + "_pt").c_str(),     &m_tf_pt);
  m_tree->Branch((m_label + "_bx").c_str(),     &m_tf_bx);
  m_tree->Branch((m_label + "_phi").c_str(),    &m_tf_phi);
  m_tree->Branch((m_label + "_eta").c_str(),    &m_tf_eta);
  m_tree->Branch((m_label + "_dxy").c_str(),    &m_tf_dxy);
  m_tree->Branch((m_label + "_qual").c_str(),   &m_tf_qual);
  m_tree->Branch((m_label + "_etaFine").c_str(),     &m_tf_etaFine);
  m_tree->Branch((m_label + "_matchedTpIdx").c_str(), &m_matchedTpIdx,  2048000,0);

}

void DTNtupleBmtfFiller::clear()
{

  m_nBmtfCands = 0;

  m_tf_pt.clear();
  m_tf_bx.clear();
  m_tf_phi.clear();
  m_tf_eta.clear();
  m_tf_dxy.clear();
  m_tf_qual.clear();
  m_tf_etaFine.clear();
  m_matchedTpIdx->Clear();

}

void DTNtupleBmtfFiller::fill(const edm::Event & ev)
{

  clear();

  TVectorF triggerIndex(4);

  auto bmtfColl = conditionalGet<l1t::RegionalMuonCandBxCollection>(ev, m_dtBmtfToken,"RegionalMuonCandBxCollection");
  auto trigColl = conditionalGet<L1MuDTChambPhContainer>(ev, m_dtTpgPhiToken, "L1MuDTChambPhContainer");
  
  if (bmtfColl.isValid()) 
    {

      auto bmtfCandBX  = bmtfColl->getFirstBX();
      auto bmtfCandLastBX = bmtfColl->getLastBX();

      for (; bmtfCandBX <= bmtfCandLastBX; ++bmtfCandBX)
	{

	  auto bmtfCand = bmtfColl->begin(bmtfCandBX);
	  auto bmtfCandLast = bmtfColl->end(bmtfCandBX);

	  for (;  bmtfCand != bmtfCandLast; ++bmtfCand)
	    {

	      std::map<int, int> mapTA = bmtfCand->trackAddress();
	     
	      int wsign = mapTA[0] == 0 ? 1 : -1;
	      int wheel = wsign * mapTA[1];
	      int sector = bmtfCand->processor();

	      int ts_mb[4] = {DEFAULT_INT_VAL, DEFAULT_INT_VAL, DEFAULT_INT_VAL, DEFAULT_INT_VAL};
	      int w_mb[4]  = {DEFAULT_INT_VAL, DEFAULT_INT_VAL, DEFAULT_INT_VAL, DEFAULT_INT_VAL};
	      int s_mb[4]  = {DEFAULT_INT_VAL, DEFAULT_INT_VAL, DEFAULT_INT_VAL, DEFAULT_INT_VAL};

	      for(int iSt = 0; iSt < N_STAT; ++iSt)
                {
		  triggerIndex[iSt] = DEFAULT_INT_VAL; // CB can this be assigned with {}
		}
	      
	      if (mapTA[2] != 3)
		{
		  w_mb[0] = wheel;
		  s_mb[0] = sector;
		  ts_mb[0] = (mapTA[2] & 1) ? 1 : 0;
		}
	      
	      for(int iSt = 1; iSt < N_STAT; ++iSt)
		{

		  if (mapTA[iSt+2] != 15)
		    {
		      ts_mb[iSt] = (mapTA[iSt+2] & 1) ? 1 : 0;   // 0 for ts1 , 1 for ts2 
		      w_mb[iSt]  = (mapTA[iSt+2] & 8) ? wheel : wheel + wsign;  // own wheel if true, nex wheel if false (depends on sign of the wheel -> case 0+, +1: +1, case 0-, -1: -1)
		      int tmpMapValue = mapTA[iSt+2] >> 1;  // temp value to remove less significant bit
		      if (tmpMapValue & 1) s_mb[iSt] = sector != 0 ? sector - 1 : 11  ; // if last two remained bit == 01 -> sector-1 (N+1 column of ref table)
		      else if (tmpMapValue & 2) s_mb[iSt] = sector != 11 ? sector + 1 : 0; //  if last two remained bit == 10 -> sector+1 (N-1 column of ref table)
		      else s_mb[iSt] = sector;  // if last two remained bit == 00 -> sector (N column of ref table)
		    }

		}

	      int tmpIdx = 0;		      	      	      

	      for(int iSt = 0; iSt < N_STAT; ++iSt)
		{

		  int iTP = 0;

		  if (trigColl.isValid())
		    {

		      const auto trigs = trigColl->getContainer();

		      for(const auto & trig : (*trigs))
			{
			  if (trig.code() != 7)
			    {
			      if (bmtfCandBX == trig.bxNum())
				{
				  if ((w_mb[iSt]  == trig.whNum())  && 
				      (ts_mb[iSt] == trig.Ts2Tag()) && 
				      (s_mb[iSt]  == trig.scNum())  && 
				      (iSt + 1    == trig.stNum()))
				    {
				      triggerIndex[tmpIdx] = iTP;
				      tmpIdx++;
				    }
				}
			    }

                          iTP++;

                        }
                    }
                }
	      
	      int iPhi = bmtfCand->hwPhi() + bmtfCand->processor() * 48 - 15;
	      if (iPhi < 0) iPhi += 576;

	      m_tf_bx.push_back(bmtfCandBX);
	      m_tf_pt.push_back((bmtfCand->hwPt()) * PT_SCALE);
              m_tf_phi.push_back(iPhi * PHI_SCALE); //no conversion yet
              m_tf_eta.push_back((bmtfCand->hwEta()) * ETA_SCALE);
	      m_tf_dxy.push_back(bmtfCand->hwDXY()); 
	      m_tf_qual.push_back(bmtfCand->hwQual());
	      m_tf_etaFine.push_back(bmtfCand->hwHF());

	      new ((*m_matchedTpIdx)[m_nBmtfCands]) TVectorF(triggerIndex);

	      m_nBmtfCands++;
	    }
	}
    }

  return;

}



