#ifndef DTNtuple_DTNtupleMuonFiller_h
#define DTNtuple_DTNtupleMuonFiller_h

/** \class DTNtupleMuonFiller DTNtupleMuonFiller.h DTDPGAnalysis/DTNtuples/src/DTNtupleMuonFiller.h
 *  
 * Helper class : the muon filler
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBaseFiller.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonIsolation.h"
#include "DataFormats/MuonReco/interface/MuonPFIsolation.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4D.h"

#include <vector>
#include "TClonesArray.h"


class DTNtupleMuonFiller : public DTNtupleBaseFiller
{

 public:

  /// Constructor
  DTNtupleMuonFiller(edm::ConsumesCollector && collector,
		    const std::shared_ptr<DTNtupleConfig> config, 
		    std::shared_ptr<TTree> tree, const std::string & label);

  ///Destructor
  virtual ~DTNtupleMuonFiller();
 
  /// Intialize function : setup tree branches etc ... 
  virtual void initialize() final;
  
  /// Clear branches before event filling 
  virtual void clear() final;

  /// Fill tree branches for a given events
  virtual void fill(const edm::Event & ev) final;    

 private :

  /// Tokens
  edm::EDGetTokenT<reco::MuonCollection>      m_muToken;
  edm::EDGetTokenT<DTRecSegment4DCollection>  m_DTSegmentToken;
  edm::EDGetTokenT<std::vector<reco::Vertex>> m_primaryVertexToken;

  short   m_mu_nMuons; 

  std::vector<float> m_mu_pt;     // muon pT 
  std::vector<float> m_mu_phi;    // muon phi 
  std::vector<float> m_mu_eta;    // muon eta
  std::vector<short> m_mu_charge; // muon charge

  std::vector<bool>  m_mu_isGlobal;
  std::vector<bool>  m_mu_isStandalone;
  std::vector<bool>  m_mu_isTracker;
  std::vector<bool>  m_mu_isRPC;

  std::vector<bool>  m_mu_isLoose; //Loose muon ID
  std::vector<bool>  m_mu_isMedium;//Medium muon ID
  std::vector<bool>  m_mu_isTight; //Tight muon ID

  std::vector<float> m_mu_trkIso03;//Relative tracker isolation (0.3 cone)
  std::vector<float> m_mu_PFIso04; //Relative PF-isolation (delta beta corrected, 0.4 cone)
  
  std::vector<float> m_mu_trk_dxy;//Inner track dxy parameter with respect to the primary vertex
  std::vector<float> m_mu_trk_dz; //Inner track dz parameter with respect to the primary vertex

  std::vector<int>   m_mu_trk_numberOfPixelHits;//Number of valid pixel hits
  std::vector<int>   m_mu_trk_numberOfLayerHits;//Number of valid layer hits
  
  std::vector<int>   m_mu_TRKMu_numberOfMatchedStations;//Number of matched stations
  std::vector<int>   m_mu_STAMu_numberOfValidMuonHits;//Number of valid muon hits
  std::vector<float> m_mu_STAMu_normChi2;             // chi2/ndof
  std::vector<float> m_mu_GLBMu_normChi2;             // chi2/ndof

  std::vector<int>   m_mu_nMatches;// Number of DT matches

  TClonesArray *m_mu_matches_wh; //Index associated to the matched chamber wheel 
  TClonesArray *m_mu_matches_sec;//Index associated to the matched chamber sector
  TClonesArray *m_mu_matches_st; //Index associated to the matched chamber station
  TClonesArray *m_mu_matches_x;//X position of the track on the matched chamber
  TClonesArray *m_mu_matches_y;//Y position of the track on the matched chamber

  TClonesArray *m_mu_matches_phi;//Phi of the (x,y) position on the matched chamber (global reference frame)
  TClonesArray *m_mu_matches_eta;//Eta of the (x,y) position on the matched chamber (global reference frame)
  TClonesArray *m_mu_matches_edgeX;// distance to closest edge in X on the matched chamber
  TClonesArray *m_mu_matches_edgeY;// distance to closest edge in Y on the matched chamber

  TClonesArray *m_mu_matchSegIdx;// Index associated to the matching segment

  float computeTrkIso (const reco::MuonIsolation& isolation, float muonPt);
  float computePFIso  (const reco::MuonPFIsolation& PFisolation, float muonPt);
};
  
#endif
