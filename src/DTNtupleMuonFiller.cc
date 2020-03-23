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
#include "DataFormats/DTRecHit/interface/DTRecSegment4D.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHitFwd.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"

#include "TVectorF.h"

DTNtupleMuonFiller::DTNtupleMuonFiller(edm::ConsumesCollector && collector,
				     const std::shared_ptr<DTNtupleConfig> config, 
				     std::shared_ptr<TTree> tree, const std::string & label) : 
  DTNtupleBaseFiller(config, tree, label)
{
  edm::InputTag & muonTag = m_config->m_inputTags["muonTag"];
  if (muonTag.label() != "none") m_muToken = collector.consumes<reco::MuonCollection>(muonTag);

  edm::InputTag & primaryVertexTag = m_config->m_inputTags["primaryVertexTag"];
  if (primaryVertexTag.label() != "none") m_primaryVertexToken = collector.consumes<std::vector<reco::Vertex>>(primaryVertexTag);

  edm::InputTag & DTSegmentTag = m_config->m_inputTags["ph2DtSegmentTag"];
  if (DTSegmentTag.label() != "none") m_DTSegmentToken = collector.consumes<DTRecSegment4DCollection>(DTSegmentTag);
}

DTNtupleMuonFiller::~DTNtupleMuonFiller() 
{ 

};

void DTNtupleMuonFiller::initialize()
{
  // In DTDPGAnalysis/src/TTreeGenerator.cc vengono inizializzati
  // con una dimensione == recoMuSize_, che in 
  // DTDPGAnalysis/python/DTTTreGenerator_cfi.py è esattamente 20
  m_mu_matches_wh    = new TClonesArray("TVectorF",20);  
  m_mu_matches_sec   = new TClonesArray("TVectorF",20);
  m_mu_matches_st    = new TClonesArray("TVectorF",20);
  m_mu_matches_x     = new TClonesArray("TVectorF",20);
  m_mu_matches_y     = new TClonesArray("TVectorF",20);
  m_mu_matches_phi   = new TClonesArray("TVectorF",20);
  m_mu_matches_eta   = new TClonesArray("TVectorF",20);
  m_mu_matches_edgeX = new TClonesArray("TVectorF",20);
  m_mu_matches_edgeY = new TClonesArray("TVectorF",20);

  m_mu_matchSegIdx    = new TClonesArray("TVectorF",20);

  m_tree->Branch((m_label + "_nMuons").c_str(),     &m_mu_nMuons);
  
  m_tree->Branch((m_label + "_pt").c_str(),     &m_mu_pt);
  m_tree->Branch((m_label + "_phi").c_str(),    &m_mu_phi);
  m_tree->Branch((m_label + "_eta").c_str(),    &m_mu_eta);
  m_tree->Branch((m_label + "_charge").c_str(), &m_mu_charge);

  m_tree->Branch((m_label + "_isGlobal").c_str(), &m_mu_isGlobal);
  m_tree->Branch((m_label + "_isStandalone").c_str(), &m_mu_isStandalone);
  m_tree->Branch((m_label + "_isTracker").c_str(), &m_mu_isTracker);
  m_tree->Branch((m_label + "_isRPC").c_str(), &m_mu_isRPC);

  m_tree->Branch((m_label + "_isLoose").c_str(), &m_mu_isLoose);
  m_tree->Branch((m_label + "_isMedium").c_str(), &m_mu_isMedium);
  m_tree->Branch((m_label + "_isTight").c_str(), &m_mu_isTight);

  m_tree->Branch((m_label + "_trkIso03").c_str(), &m_mu_trkIso03);
  m_tree->Branch((m_label + "_PFIso04").c_str(), &m_mu_PFIso04);

  m_tree->Branch((m_label + "_trk_dxy").c_str(), &m_mu_trk_dxy);
  m_tree->Branch((m_label + "_trk_dz").c_str(), &m_mu_trk_dz);
  m_tree->Branch((m_label + "_trk_numberOfPixelHits").c_str(), &m_mu_trk_numberOfPixelHits);
  m_tree->Branch((m_label + "_trk_numberOfLayerHits").c_str(), &m_mu_trk_numberOfLayerHits);
  m_tree->Branch((m_label + "_mu_TRKMu_numberOfMatchedStations").c_str(), &m_mu_TRKMu_numberOfMatchedStations);
  m_tree->Branch((m_label + "_mu_STAMu_numberOfValidMuonHits").c_str(), &m_mu_STAMu_numberOfValidMuonHits);
  m_tree->Branch((m_label + "_mu_STAMu_normChi2").c_str(), &m_mu_STAMu_normChi2);
  m_tree->Branch((m_label + "_mu_GLBMu_normChi2").c_str(), &m_mu_GLBMu_normChi2);

  m_tree->Branch((m_label + "_mu_nMatches").c_str(), &m_mu_nMatches);

  m_tree->Branch((m_label + "_matches_wh").c_str()   , &m_mu_matches_wh   ,2048000,0);
  m_tree->Branch((m_label + "_matches_sec").c_str()  , &m_mu_matches_sec  ,2048000,0);
  m_tree->Branch((m_label + "_matches_st").c_str()   , &m_mu_matches_st   ,2048000,0);
  m_tree->Branch((m_label + "_matches_x").c_str()    , &m_mu_matches_x    ,2048000,0);
  m_tree->Branch((m_label + "_matches_y").c_str()    , &m_mu_matches_y    ,2048000,0);
  m_tree->Branch((m_label + "_matches_phi").c_str()  , &m_mu_matches_phi  ,2048000,0);
  m_tree->Branch((m_label + "_matches_eta").c_str()  , &m_mu_matches_eta  ,2048000,0);
  m_tree->Branch((m_label + "_matches_edgeX").c_str(), &m_mu_matches_edgeX,2048000,0);
  m_tree->Branch((m_label + "_matches_edgeY").c_str(), &m_mu_matches_edgeY,2048000,0);

  m_tree->Branch((m_label + "_segMatches").c_str()   , &m_mu_matchSegIdx   ,2048000,0);
}

void DTNtupleMuonFiller::clear()
{

  m_mu_nMuons = 0;

  m_mu_pt.clear();
  m_mu_phi.clear();
  m_mu_eta.clear();
  m_mu_charge.clear();

  m_mu_isGlobal.clear();
  m_mu_isStandalone.clear();
  m_mu_isTracker.clear();
  m_mu_isRPC.clear();

  m_mu_isLoose.clear();
  m_mu_isMedium.clear();
  m_mu_isTight.clear();

  m_mu_trkIso03.clear();
  m_mu_PFIso04.clear();

  m_mu_trk_dxy.clear();
  m_mu_trk_dz.clear();
  m_mu_trk_numberOfPixelHits.clear();
  m_mu_trk_numberOfLayerHits.clear();
  
  m_mu_TRKMu_numberOfMatchedStations.clear();
  m_mu_STAMu_numberOfValidMuonHits.clear();
  m_mu_STAMu_normChi2.clear();
  m_mu_GLBMu_normChi2.clear();

  m_mu_nMatches.clear();

  m_mu_matches_wh->Clear();     
  m_mu_matches_sec->Clear();   
  m_mu_matches_st->Clear();     
  m_mu_matches_x->Clear();      
  m_mu_matches_y->Clear();      
  m_mu_matches_phi->Clear();    
  m_mu_matches_eta->Clear();    
  m_mu_matches_edgeX->Clear();  
  m_mu_matches_edgeY->Clear();  

  m_mu_matchSegIdx->Clear();  
}

void DTNtupleMuonFiller::fill(const edm::Event & ev)
{

  clear();

  auto muons = conditionalGet<reco::MuonCollection>(ev, m_muToken,"MuonCollection");
  auto segments = conditionalGet<DTRecSegment4DCollection>(ev,m_DTSegmentToken,"DTRecSegment4DCollection");

  edm::Handle<std::vector<reco::Vertex>> vertexesHandle;
  ev.getByToken(m_primaryVertexToken ,vertexesHandle);

  if (muons.isValid()) 
    {

      for (const auto & muon : (*muons))
	{

	  m_mu_pt.push_back(muon.pt());
	  m_mu_eta.push_back(muon.eta());
	  m_mu_phi.push_back(muon.phi());
	  m_mu_charge.push_back(muon.charge());

	  m_mu_isGlobal.push_back(muon.isGlobalMuon());
	  m_mu_isStandalone.push_back(muon.isStandAloneMuon());
	  m_mu_isTracker.push_back(muon.isTrackerMuon());
	  m_mu_isRPC.push_back(muon.isRPCMuon());

	  m_mu_isLoose.push_back(muon.passed(reco::Muon::CutBasedIdLoose));
	  m_mu_isMedium.push_back(muon.passed(reco::Muon::CutBasedIdMedium));
	  m_mu_isTight.push_back(muon.passed(reco::Muon::CutBasedIdTight));

	  m_mu_trkIso03.push_back(computeTrkIso(muon.isolationR03(),muon.pt()));
	  m_mu_PFIso04.push_back(computePFIso(muon.pfIsolationR04(),muon.pt()));

	  //INNER TRACK VARIABLES
	  if(!muon.innerTrack().isNull()){

	       const reco::TrackRef innerTrackRef = muon.innerTrack();
               const reco::Vertex & vertex = vertexesHandle->at(0);

	       m_mu_trk_dxy.push_back(innerTrackRef->dxy(vertex.position()));
	       m_mu_trk_dz.push_back(innerTrackRef->dz(vertex.position()));
	       m_mu_trk_numberOfPixelHits.push_back(innerTrackRef->hitPattern().numberOfValidPixelHits());
	       m_mu_trk_numberOfLayerHits.push_back(innerTrackRef->hitPattern().trackerLayersWithMeasurement());

	  }
	  else{
	       m_mu_trk_dxy.push_back(DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL);
	       m_mu_trk_dz.push_back(DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL);
	       m_mu_trk_numberOfPixelHits.push_back(DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL);
	       m_mu_trk_numberOfLayerHits.push_back(DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL);
	  }

	  //TRACKER MUON VARIABLES
	  m_mu_TRKMu_numberOfMatchedStations.push_back(muon.numberOfMatchedStations(reco::Muon::ArbitrationType::SegmentAndTrackArbitration));

	  //STANDALONE MUON VARIABLES
	  if(muon.isGlobalMuon() || muon.isStandAloneMuon()){
	       const reco::TrackRef outerTrackRef = muon.outerTrack();
	       m_mu_STAMu_numberOfValidMuonHits.push_back(outerTrackRef->hitPattern().numberOfValidMuonHits());
	       m_mu_STAMu_normChi2.push_back(outerTrackRef->chi2()/outerTrackRef->ndof());
	  }
	  else{
	       m_mu_STAMu_numberOfValidMuonHits.push_back(DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL);
	       m_mu_STAMu_normChi2.push_back(DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL);
	  }

	  //GLOBAL MUON VARIABLES
	  if(muon.isGlobalMuon()){
	       const reco::TrackRef globalTrackRef = muon.globalTrack();
	       m_mu_GLBMu_normChi2.push_back(globalTrackRef->chi2()/globalTrackRef->ndof());
	  }
	  else{
	       m_mu_GLBMu_normChi2.push_back(DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL);
	  }
          //MATCHING VARIABLES
          static TVectorF dummyFloat(1); 
          dummyFloat(0) = DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
          Int_t iMatches = 0;

          TVectorF matchesWh(16);
          TVectorF matchesSec(16);
          TVectorF matchesSt(16);
          
          TVectorF matchesX(16);
          TVectorF matchesY(16);
          
          TVectorF matchesPhi(16);
          TVectorF matchesEta(16);
          
          TVectorF matchesEdgeX(16);
          TVectorF matchesEdgeY(16);
  
          if ( muon.isMatchesValid() ) {

            for ( const auto & match : muon.matches() ) {

              if ( iMatches < 16 && match.id.det() == DetId::Muon &&
                   match.id.subdetId() == MuonSubdetId::DT ) {

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

                ++iMatches;
              }
            }

          }
     
          m_mu_nMatches.push_back(iMatches);

          if (iMatches > 0) {
            new ((*m_mu_matches_wh)[m_mu_nMuons])  TVectorF(matchesWh);
            new ((*m_mu_matches_sec)[m_mu_nMuons]) TVectorF(matchesSec);
            new ((*m_mu_matches_st)[m_mu_nMuons])  TVectorF(matchesSt);
            
            new ((*m_mu_matches_x)[m_mu_nMuons]) TVectorF(matchesX);
            new ((*m_mu_matches_y)[m_mu_nMuons]) TVectorF(matchesY);
            
            new ((*m_mu_matches_phi)[m_mu_nMuons]) TVectorF(matchesPhi);
            new ((*m_mu_matches_eta)[m_mu_nMuons]) TVectorF(matchesEta);
            
            new ((*m_mu_matches_edgeX)[m_mu_nMuons]) TVectorF(matchesEdgeX);
            new ((*m_mu_matches_edgeY)[m_mu_nMuons]) TVectorF(matchesEdgeY);
          }
          else {
            new ((*m_mu_matches_wh)[m_mu_nMuons])  TVectorF(dummyFloat);
            new ((*m_mu_matches_sec)[m_mu_nMuons]) TVectorF(dummyFloat);
            new ((*m_mu_matches_st)[m_mu_nMuons])  TVectorF(dummyFloat);
            
            new ((*m_mu_matches_x)[m_mu_nMuons]) TVectorF(dummyFloat);
            new ((*m_mu_matches_y)[m_mu_nMuons]) TVectorF(dummyFloat);
            
            new ((*m_mu_matches_phi)[m_mu_nMuons]) TVectorF(dummyFloat);
            new ((*m_mu_matches_eta)[m_mu_nMuons]) TVectorF(dummyFloat);
            
            new ((*m_mu_matches_edgeX)[m_mu_nMuons]) TVectorF(dummyFloat);
            new ((*m_mu_matches_edgeY)[m_mu_nMuons]) TVectorF(dummyFloat);
          }

	  //SEGMENT MATCHING VARIABLES
	  TVectorF segmentMatches (100);//is 100 too much/enough ?
	  int iSegMatches = 0;

	  if(!muon.outerTrack().isNull()){

	    reco::TrackRef outerTrackRef = muon.outerTrack();

	    for(auto& recHit : outerTrackRef->recHits()){

	      DetId detId = recHit->geographicalId();

	      if (detId.det() == DetId::Muon && detId.subdetId() == MuonSubdetId::DT){

	        const auto DTSegment = dynamic_cast<const DTRecSegment4D*>(recHit);
		int iSeg = 0;

	        for(const auto& segment : (*segments)){
                  if (DTSegment &&
            	      DTSegment->chamberId().station() == segment.chamberId().station() &&
            	      DTSegment->chamberId().wheel()   == segment.chamberId().wheel()   &&
            	      DTSegment->chamberId().sector()  == segment.chamberId().sector()  &&
            	      std::abs(DTSegment->localPosition().x()  - segment.localPosition().x() ) < 0.01 &&
            	      std::abs(DTSegment->localPosition().y()  - segment.localPosition().y() ) < 0.01 &&
            	      std::abs(DTSegment->localDirection().x() - segment.localDirection().x()) < 0.01 &&
            	      std::abs(DTSegment->localDirection().y() - segment.localDirection().y()) < 0.01 ){
		        
			segmentMatches(iSegMatches) = iSeg;
		        ++iSegMatches;
		      }

		  ++iSeg;
		}//loop over segments
	      }
	    }//loop over recHits
	  }

	  if (iSegMatches > 0){
	    new ((*m_mu_matchSegIdx)[m_mu_nMuons]) TVectorF(segmentMatches);
	  }
	  else{
	    new ((*m_mu_matchSegIdx)[m_mu_nMuons]) TVectorF(dummyFloat);
	  }

          m_mu_nMuons++;
	}
    }

  return;

}

float DTNtupleMuonFiller::computeTrkIso (const reco::MuonIsolation& isolation, float muonPt){
          return isolation.sumPt/muonPt;
}
float DTNtupleMuonFiller::computePFIso (const reco::MuonPFIsolation& PFisolation, float muonPt){
          return (PFisolation.sumChargedHadronPt + std::max(0., PFisolation.sumNeutralHadronEt + PFisolation.sumPhotonEt - 0.5*PFisolation.sumPUPt))/muonPt;
}
