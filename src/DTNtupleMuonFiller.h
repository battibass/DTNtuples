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

  std::vector<float> m_mu_pt;     // muon pT (float)
  std::vector<float> m_mu_phi;    // muon phi (float in [-pi:pi] range) // CB check range
  std::vector<float> m_mu_eta;    // muon eta
  std::vector<short> m_mu_charge; // muon charge

  std::vector<bool>  m_mu_isGlobal;
  std::vector<bool>  m_mu_isStandalone;
  std::vector<bool>  m_mu_isTracker;
  std::vector<bool>  m_mu_isRPC;

  std::vector<bool>  m_mu_isLoose;//Loose muon ID
  std::vector<bool>  m_mu_isMedium;//Medium muon ID
  std::vector<bool>  m_mu_isTight;//Tight muon ID

  std::vector<float> m_mu_trkIso03;//Relative tracker isolation (0.3 cone)
  std::vector<float> m_mu_PFIso04;//Relative PF-isolation (delta beta corrected, 0.4 cone)
  
  std::vector<float> m_mu_trk_dxy;//Inner track dxy parameter with respect to the primary vertex
  std::vector<float> m_mu_trk_dz;//Inner track dz parameter with respect to the primary vertex

  std::vector<int>   m_mu_trk_numberOfPixelHits;//Number of valid pixel hits
  std::vector<int>   m_mu_trk_numberOfLayerHits;//Number of valid layer hits
  
  std::vector<int>   m_mu_TRKMu_numberOfMatchedStations;//Number of matched stations
  std::vector<int>   m_mu_STAMu_numberOfValidMuonHits;//Number of valid muon hits
  std::vector<float> m_mu_STAMu_normChi2;// chi2/ndof
  std::vector<float> m_mu_GLBMu_normChi2;// chi2/ndof

  std::vector<int>   m_mu_nMatches;

  TClonesArray *m_mu_matches_wh;  
  TClonesArray *m_mu_matches_sec;
  TClonesArray *m_mu_matches_st;
  TClonesArray *m_mu_matches_x;
  TClonesArray *m_mu_matches_y;
  TClonesArray *m_mu_matches_phi;
  TClonesArray *m_mu_matches_eta;
  TClonesArray *m_mu_matches_edgeX;
  TClonesArray *m_mu_matches_edgeY;

  TClonesArray *m_mu_matchSegIdx;// Matching segment index

  float computeTrkIso (const reco::MuonIsolation& isolation, float muonPt);
  float computePFIso  (const reco::MuonPFIsolation& PFisolation, float muonPt);
};
  
#endif
