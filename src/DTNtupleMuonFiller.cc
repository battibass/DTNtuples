/** \class DTNtupleMuonFiller DTNtupleMuonFiller.cc DTDPGAnalysis/DTNtuples/src/DTNtupleMuonFiller.cc
 *  
 * Helper class : the muon filler
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleMuonFiller.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "Geometry/DTGeometry/interface/DTGeometry.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonReco/interface/MuonChamberMatch.h"
#include "DataFormats/MuonReco/interface/MuonSegmentMatch.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

#include "DataFormats/DTRecHit/interface/DTRecSegment4D.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHitFwd.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"

#include "TVectorF.h"

DTNtupleMuonFiller::DTNtupleMuonFiller(edm::ConsumesCollector && collector,
				       const std::shared_ptr<DTNtupleConfig> config, 
				       std::shared_ptr<TTree> tree, const std::string & label) : 
  DTNtupleBaseFiller(config, tree, label), m_nullVecF()
{

  edm::InputTag & muonTag = m_config->m_inputTags["muonTag"];
  if (muonTag.label() != "none") m_muToken = collector.consumes<reco::MuonCollection>(muonTag);

  edm::InputTag & primaryVerticesTag = m_config->m_inputTags["primaryVerticesTag"];
  if (primaryVerticesTag.label() != "none") m_primaryVerticesToken = collector.consumes<std::vector<reco::Vertex>>(primaryVerticesTag);

  edm::InputTag & dtSegmentTag = m_config->m_inputTags["ph1DtSegmentTag"];
  if (dtSegmentTag.label() != "none") m_dtSegmentToken = collector.consumes<DTRecSegment4DCollection>(dtSegmentTag);

}

DTNtupleMuonFiller::~DTNtupleMuonFiller() 
{ 

}

void DTNtupleMuonFiller::initialize()
{
 
  m_matches_wheel   = new TClonesArray("TVectorF", 24);  
  m_matches_sector  = new TClonesArray("TVectorF", 24);
  m_matches_station = new TClonesArray("TVectorF", 24);
  m_matches_x     = new TClonesArray("TVectorF", 24);
  m_matches_y     = new TClonesArray("TVectorF", 24);
  m_matches_phi   = new TClonesArray("TVectorF", 24);
  m_matches_eta   = new TClonesArray("TVectorF", 24);
  m_matches_edgeX = new TClonesArray("TVectorF", 24);
  m_matches_edgeY = new TClonesArray("TVectorF", 24);
  m_matches_dXdZ  = new TClonesArray("TVectorF", 24);
  m_matches_dYdZ  = new TClonesArray("TVectorF", 24);

  m_matchSegIdx = new TClonesArray("TVectorF", 10);

  m_tree->Branch((m_label + "_nMuons").c_str(), &m_nMuons);
  
  m_tree->Branch((m_label + "_pt").c_str(), &m_pt);
  m_tree->Branch((m_label + "_phi").c_str(), &m_phi);
  m_tree->Branch((m_label + "_eta").c_str(), &m_eta);
  m_tree->Branch((m_label + "_charge").c_str(), &m_charge);

  m_tree->Branch((m_label + "_isGlobal").c_str(), &m_isGlobal);
  m_tree->Branch((m_label + "_isStandalone").c_str(), &m_isStandalone);
  m_tree->Branch((m_label + "_isTracker").c_str(), &m_isTracker);
  m_tree->Branch((m_label + "_isTrackerArb").c_str(), &m_isTrackerArb);
  m_tree->Branch((m_label + "_isRPC").c_str(), &m_isRPC);

  // m_tree->Branch((m_label + "_firesIsoTrig").c_str(), &m_firesIsoTrig);
  // m_tree->Branch((m_label + "_firesTrig").c_str(), &m_firesTrig);

  m_tree->Branch((m_label + "_isLoose").c_str(), &m_isLoose);
  m_tree->Branch((m_label + "_isMedium").c_str(), &m_isMedium);
  m_tree->Branch((m_label + "_isTight").c_str(), &m_isTight);

  m_tree->Branch((m_label + "_trkIso03").c_str(), &m_trkIso03);
  m_tree->Branch((m_label + "_pfIso04").c_str(), &m_pfIso04);

  m_tree->Branch((m_label + "_trk_dxy").c_str(), &m_trk_dxy);
  m_tree->Branch((m_label + "_trk_dz").c_str(), &m_trk_dz);
  m_tree->Branch((m_label + "_trk_algo").c_str(), &m_trk_algo);
  m_tree->Branch((m_label + "_trk_origAlgo").c_str(), &m_trk_origAlgo);
  m_tree->Branch((m_label + "_trk_numberOfValidPixelHits").c_str(), &m_trk_numberOfValidPixelHits);
  m_tree->Branch((m_label + "_trk_numberOfValidTrackerLayers").c_str(), &m_trk_numberOfValidTrackerLayers);
  m_tree->Branch((m_label + "_trkMu_numberOfMatchedStations").c_str(), &m_trkMu_numberOfMatchedStations);
  m_tree->Branch((m_label + "_trkMu_numberOfMatchedRPCLayers").c_str(), &m_trkMu_numberOfMatchedRPCLayers);
  m_tree->Branch((m_label + "_staMu_numberOfValidMuonHits").c_str(), &m_staMu_numberOfValidMuonHits);
  m_tree->Branch((m_label + "_staMu_normChi2").c_str(), &m_staMu_normChi2);
  m_tree->Branch((m_label + "_glbMu_normChi2").c_str(), &m_glbMu_normChi2);

  m_tree->Branch((m_label + "_nMatches").c_str(), &m_nMatches);

  m_tree->Branch((m_label + "_matches_wheel").c_str()   , &m_matches_wheel   , 2048000, 0);
  m_tree->Branch((m_label + "_matches_sector").c_str()  , &m_matches_sector  , 2048000, 0);
  m_tree->Branch((m_label + "_matches_station").c_str() , &m_matches_station , 2048000, 0);
  m_tree->Branch((m_label + "_matches_x").c_str()       , &m_matches_x       , 2048000, 0);
  m_tree->Branch((m_label + "_matches_y").c_str()       , &m_matches_y       , 2048000, 0);
  m_tree->Branch((m_label + "_matches_phi").c_str()     , &m_matches_phi     , 2048000, 0);
  m_tree->Branch((m_label + "_matches_eta").c_str()     , &m_matches_eta     , 2048000, 0);
  m_tree->Branch((m_label + "_matches_edgeX").c_str()   , &m_matches_edgeX   , 2048000, 0);
  m_tree->Branch((m_label + "_matches_edgeY").c_str()   , &m_matches_edgeY   , 2048000, 0);
  m_tree->Branch((m_label + "_matches_dXdZ").c_str()    , &m_matches_dXdZ    , 2048000, 0);
  m_tree->Branch((m_label + "_matches_dYdZ").c_str()    , &m_matches_dXdZ    , 2048000, 0);

  m_tree->Branch((m_label + "_matchSegIdx").c_str(), &m_matchSegIdx, 2048000, 0);

}

void DTNtupleMuonFiller::clear()
{

  m_nMuons = 0;

  m_pt.clear();
  m_phi.clear();
  m_eta.clear();
  m_charge.clear();

  m_isGlobal.clear();
  m_isStandalone.clear();
  m_isTracker.clear();
  m_isTrackerArb.clear();
  m_isRPC.clear();

  // m_firesIsoTrig.clear();
  // m_firesTrig.clear();

  m_isLoose.clear();
  m_isMedium.clear();
  m_isTight.clear();

  m_trkIso03.clear();
  m_pfIso04.clear();

  m_trk_dxy.clear();
  m_trk_dz.clear();
  m_trk_origAlgo.clear();
  m_trk_numberOfValidPixelHits.clear();
  m_trk_numberOfValidTrackerLayers.clear();
  
  m_trkMu_numberOfMatchedStations.clear();
  m_trkMu_numberOfMatchedRPCLayers.clear();
  m_staMu_numberOfValidMuonHits.clear();
  m_staMu_normChi2.clear();
  m_glbMu_normChi2.clear();

  m_nMatches.clear();

  m_matches_wheel->Clear();     
  m_matches_sector->Clear();   
  m_matches_station->Clear();     
  m_matches_x->Clear();      
  m_matches_y->Clear();      
  m_matches_phi->Clear();    
  m_matches_eta->Clear();    
  m_matches_edgeX->Clear();  
  m_matches_edgeY->Clear();  
  m_matches_dXdZ->Clear();      
  m_matches_dYdZ->Clear();      

  m_matchSegIdx->Clear();  

}

void DTNtupleMuonFiller::fill(const edm::Event & ev)
{

  clear();

  auto muons = conditionalGet<reco::MuonCollection>(ev, m_muToken, "MuonCollection");
  auto segments = conditionalGet<DTRecSegment4DCollection>(ev,m_dtSegmentToken, "DTRecSegment4DCollection");
  auto vtxs = conditionalGet<std::vector<reco::Vertex>>(ev, m_primaryVerticesToken, "std::vector<reco::Vertex>");

  if (muons.isValid() && segments.isValid() && vtxs.isValid()) 
    {

      for (const auto & muon : (*muons))
	{

	  m_pt.push_back(muon.pt());
	  m_eta.push_back(muon.eta());
	  m_phi.push_back(muon.phi());
	  m_charge.push_back(muon.charge());

	  bool isTrackerArb = muon::isGoodMuon(muon, muon::TrackerMuonArbitrated);

	  m_isGlobal.push_back(muon.isGlobalMuon());
	  m_isStandalone.push_back(muon.isStandAloneMuon());
	  m_isTracker.push_back(muon.isTrackerMuon());
	  m_isTrackerArb.push_back(isTrackerArb);
	  m_isRPC.push_back(muon.isRPCMuon());

	  // m_firesIsoTrig.push_back(muon.triggered(m_config->m_isoTrigName));
	  // m_firesTrig.push_back(muon.triggered(m_config->m_trigName));

	  m_isLoose.push_back(muon.passed(reco::Muon::CutBasedIdLoose));
	  m_isMedium.push_back(muon.passed(reco::Muon::CutBasedIdMedium));
	  m_isTight.push_back(muon.passed(reco::Muon::CutBasedIdTight));

	  m_trkIso03.push_back(computeTrkIso(muon.isolationR03(),muon.pt()));
	  m_pfIso04.push_back(computePFIso(muon.pfIsolationR04(),muon.pt()));

	  //INNER TRACK VARIABLES
	  if(!muon.innerTrack().isNull())
	    {
	      
	      const reco::TrackRef innerTrackRef = muon.innerTrack();
	      const reco::Vertex & vertex = vtxs->at(0);

	      m_trk_dxy.push_back(innerTrackRef->dxy(vertex.position()));
	      m_trk_dz.push_back(innerTrackRef->dz(vertex.position()));
	      m_trk_algo.push_back(innerTrackRef->algo());
	      m_trk_origAlgo.push_back(innerTrackRef->originalAlgo());
	      m_trk_numberOfValidPixelHits.push_back(innerTrackRef->hitPattern().numberOfValidPixelHits());
	      m_trk_numberOfValidTrackerLayers.push_back(innerTrackRef->hitPattern().trackerLayersWithMeasurement());

	    }
	  else
	    {

	       m_trk_dxy.push_back(DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL);
	       m_trk_dz.push_back(DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL);
	       m_trk_algo.push_back(DTNtupleBaseFiller::DEFAULT_INT_VAL_POS);
	       m_trk_origAlgo.push_back(DTNtupleBaseFiller::DEFAULT_INT_VAL_POS);
	       m_trk_numberOfValidPixelHits.push_back(DTNtupleBaseFiller::DEFAULT_INT_VAL_POS);
	       m_trk_numberOfValidTrackerLayers.push_back(DTNtupleBaseFiller::DEFAULT_INT_VAL_POS);

	    }

	  //TRACKER / RPC MUON VARIABLES
	  m_trkMu_numberOfMatchedStations.push_back(muon.numberOfMatchedStations());
	  m_trkMu_numberOfMatchedRPCLayers.push_back(muon.numberOfMatchedRPCLayers());

	  //STANDALONE MUON VARIABLES
	  if(muon.isStandAloneMuon())
	    {
	      
	      const reco::TrackRef outerTrackRef = muon.outerTrack();
	      m_staMu_numberOfValidMuonHits.push_back(outerTrackRef->hitPattern().numberOfValidMuonHits());
	      m_staMu_normChi2.push_back(outerTrackRef->chi2()/outerTrackRef->ndof());
	      
	    }
	  else
	    {
	      
	      m_staMu_numberOfValidMuonHits.push_back(DTNtupleBaseFiller::DEFAULT_INT_VAL_POS);
	      m_staMu_normChi2.push_back(DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL_POS);
	      
	    }

	  //GLOBAL MUON VARIABLES
	  if(muon.isGlobalMuon())
	    {
	      
	      const reco::TrackRef globalTrackRef = muon.globalTrack();
	      m_glbMu_normChi2.push_back(globalTrackRef->chi2()/globalTrackRef->ndof());
	      
	    }
	  else
	    {
	      
	      m_glbMu_normChi2.push_back(DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL_POS);
	      
	    }

          Int_t iMatches = 0;

          TVectorF matchesWh(24);
          TVectorF matchesSec(24);
          TVectorF matchesSt(24);
          
          TVectorF matchesX(24);
          TVectorF matchesY(24);
          
          TVectorF matchesPhi(24);
          TVectorF matchesEta(24);
          
          TVectorF matchesEdgeX(24);
          TVectorF matchesEdgeY(24);

          TVectorF matchesdXdZ(24);
          TVectorF matchesdYdZ(24);
  
          if ( muon.isMatchesValid() ) 
	    {

	      for ( const auto & match : muon.matches() ) 
		{

		  if ( iMatches < 16 && match.id.det() == DetId::Muon &&
		       match.id.subdetId() == MuonSubdetId::DT ) 
		    {

		      DTChamberId dtId (match.id.rawId());
		      const auto chamb = m_config->m_dtGeometry->chamber(static_cast<DTChamberId>(match.id));
 
		      matchesWh(iMatches) = dtId.wheel();
		      matchesSec(iMatches) = dtId.sector();
		      matchesSt(iMatches) = dtId.station();
		      
		      matchesX(iMatches) = match.x;
		      matchesY(iMatches) = match.y;
		      
		      matchesPhi(iMatches) = chamb->toGlobal(LocalPoint(match.x,match.y,0.)).phi();
		      matchesEta(iMatches) = chamb->toGlobal(LocalPoint(match.x,match.y,0.)).eta();
		      
		      matchesEdgeX(iMatches) = match.edgeX;
		      matchesEdgeY(iMatches) = match.edgeY;

		      matchesdXdZ(iMatches) = match.dXdZ;
		      matchesdYdZ(iMatches) = match.dYdZ;
		      
		      ++iMatches;

		    }

		}
	      
	    }
     
          m_nMatches.push_back(iMatches);

          if (iMatches > 0) 
	    {

	      new ((*m_matches_wheel)[m_nMuons])   TVectorF(matchesWh);
	      new ((*m_matches_sector)[m_nMuons])  TVectorF(matchesSec);
	      new ((*m_matches_station)[m_nMuons]) TVectorF(matchesSt);
	      
	      new ((*m_matches_x)[m_nMuons]) TVectorF(matchesX);
	      new ((*m_matches_y)[m_nMuons]) TVectorF(matchesY);
	      
	      new ((*m_matches_phi)[m_nMuons]) TVectorF(matchesPhi);
	      new ((*m_matches_eta)[m_nMuons]) TVectorF(matchesEta);
	      
	      new ((*m_matches_edgeX)[m_nMuons]) TVectorF(matchesEdgeX);
	      new ((*m_matches_edgeY)[m_nMuons]) TVectorF(matchesEdgeY);

	      new ((*m_matches_dXdZ)[m_nMuons]) TVectorF(matchesdXdZ);
	      new ((*m_matches_dYdZ)[m_nMuons]) TVectorF(matchesdYdZ);

	    }
          else 
	    {

	      new ((*m_matches_wheel)[m_nMuons])   TVectorF(m_nullVecF);
	      new ((*m_matches_sector)[m_nMuons])  TVectorF(m_nullVecF);
	      new ((*m_matches_station)[m_nMuons]) TVectorF(m_nullVecF);
	      
	      new ((*m_matches_x)[m_nMuons]) TVectorF(m_nullVecF);
	      new ((*m_matches_y)[m_nMuons]) TVectorF(m_nullVecF);
	      
	      new ((*m_matches_phi)[m_nMuons]) TVectorF(m_nullVecF);
	      new ((*m_matches_eta)[m_nMuons]) TVectorF(m_nullVecF);
	      
	      new ((*m_matches_edgeX)[m_nMuons]) TVectorF(m_nullVecF);
	      new ((*m_matches_edgeY)[m_nMuons]) TVectorF(m_nullVecF);

	      new ((*m_matches_dXdZ)[m_nMuons]) TVectorF(m_nullVecF);
	      new ((*m_matches_dYdZ)[m_nMuons]) TVectorF(m_nullVecF);

	    }
	  
	  //SEGMENT MATCHING VARIABLES
	  TVectorF segmentMatches (10);
	  int iSegMatches = 0;

	  if(!muon.outerTrack().isNull())
	    {

	      reco::TrackRef outerTrackRef = muon.outerTrack();
	      
	      for(auto& recHit : outerTrackRef->recHits())
		{
		  
		  DetId detId = recHit->geographicalId();
		  
		  if (detId.det() == DetId::Muon && detId.subdetId() == MuonSubdetId::DT)
		    {
		    
		      const auto dtSegmentSta = dynamic_cast<const DTRecSegment4D*>(recHit);
		      int iSeg = 0;

		      for(const auto& segment : (*segments))
			{

			  if (dtSegmentSta &&
			      dtSegmentSta->chamberId().station() == segment.chamberId().station() &&
			      dtSegmentSta->chamberId().wheel()   == segment.chamberId().wheel()   &&
			      dtSegmentSta->chamberId().sector()  == segment.chamberId().sector()  &&
			      std::abs(dtSegmentSta->localPosition().x()  - segment.localPosition().x() ) < 0.01 &&
			      std::abs(dtSegmentSta->localPosition().y()  - segment.localPosition().y() ) < 0.01 &&
			      std::abs(dtSegmentSta->localDirection().x() - segment.localDirection().x()) < 0.01 &&
			      std::abs(dtSegmentSta->localDirection().y() - segment.localDirection().y()) < 0.01 ){
			    
			    segmentMatches(iSegMatches) = iSeg;
			    ++iSegMatches;

			  }
			  
			  ++iSeg;
			}//loop over segments
		    
		    }
	    
		}//loop over recHits
	    }

	  if (iSegMatches > 0)
	    {

	    new ((*m_matchSegIdx)[m_nMuons]) TVectorF(segmentMatches);

	    }
	  else
	    {

	    new ((*m_matchSegIdx)[m_nMuons]) TVectorF(m_nullVecF);

	    }

          m_nMuons++;

	}

    }

  return;
  
}

float DTNtupleMuonFiller::computeTrkIso(const reco::MuonIsolation& isolation, float muonPt)
{

  return isolation.sumPt / muonPt;

}

float DTNtupleMuonFiller::computePFIso (const reco::MuonPFIsolation& pfIsolation, float muonPt)
{

  return (pfIsolation.sumChargedHadronPt + std::max(0., pfIsolation.sumNeutralHadronEt + pfIsolation.sumPhotonEt - 0.5*pfIsolation.sumPUPt))/muonPt;

}
