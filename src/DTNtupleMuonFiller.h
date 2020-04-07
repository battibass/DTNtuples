#ifndef DTNtuple_DTNtupleMuonFiller_h
#define DTNtuple_DTNtupleMuonFiller_h

/** \class DTNtupleMuonFiller DTNtupleMuonFiller.h DTDPGAnalysis/DTNtuples/src/DTNtupleMuonFiller.h
 *  
 * Helper class : the muon filler
 *
 * \author L. Lunerti (INFN BO)
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

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"

#include <vector>
#include "TClonesArray.h"
#include "TVectorF.h"


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

  float computeTrkIso(const reco::MuonIsolation& isolation, float muonPt);
  float computePFIso(const reco::MuonPFIsolation& PFisolation, float muonPt);

  bool hasTrigger(std::vector<int> & trigIndices, 
		  const trigger::TriggerObjectCollection & trigObjs, 
		  edm::Handle<trigger::TriggerEvent>  & trigEvent, 
		  const reco::Muon & muon); 

  /// Tokens
  edm::EDGetTokenT<reco::MuonCollection>      m_muToken;
  edm::EDGetTokenT<DTRecSegment4DCollection>  m_dtSegmentToken;
  edm::EDGetTokenT<std::vector<reco::Vertex>> m_primaryVerticesToken;

  edm::EDGetTokenT<edm::TriggerResults>   m_trigResultsToken;
  edm::EDGetTokenT<trigger::TriggerEvent> m_trigEventToken;

  /// The default TVercorF for empty TClonesArray vectors
  TVectorF m_nullVecF;

  unsigned int m_nMuons; 

  std::vector<float> m_pt;     // muon pT [GeV/c]
  std::vector<float> m_phi;    // muon phi [rad]
  std::vector<float> m_eta;    // muon eta
  std::vector<short> m_charge; // muon charge

  std::vector<bool>  m_isGlobal;
  std::vector<bool>  m_isStandalone;
  std::vector<bool>  m_isTracker;
  std::vector<bool>  m_isTrackerArb;
  std::vector<bool>  m_isRPC;

  std::vector<bool>  m_firesIsoTrig; // True if the muon is matched to an isolated trigger
                                     // specified in the ntuple config file
  std::vector<bool>  m_firesTrig;    // True if the muon is matched to a (non isolated) trigger
                                     // specified in the ntuple config file

  std::vector<bool>  m_isLoose;  // Loose muon ID
  std::vector<bool>  m_isMedium; // Medium muon ID
  std::vector<bool>  m_isTight;  // Tight muon ID

  std::vector<float> m_trkIso03; // Relative tracker isolation (0.3 cone)
  std::vector<float> m_pfIso04;  // Relative PF-isolation (delta beta corrected, 0.4 cone)
  
  std::vector<float> m_trk_dxy; // Inner track dxy parameter with respect to the primary vertex
  std::vector<float> m_trk_dz;  // Inner track dz parameter with respect to the primary vertex

  std::vector<int>   m_trk_algo; // Iterative tracking algorithm
                                 // used to build the inner track

  std::vector<int>   m_trk_origAlgo; // Original (pre muon iterations) iterative tracking 
                                     // algorithm used to build the inner track

  std::vector<int>   m_trk_numberOfValidPixelHits;     // Number of valid pixel hits
  std::vector<int>   m_trk_numberOfValidTrackerLayers; // Number of valid tracker layers
  
  std::vector<int>   m_trkMu_numberOfMatchedStations;  // Number of matched stations
  std::vector<int>   m_trkMu_numberOfMatchedRPCLayers; // Number of matched RPC layers

  std::vector<int>   m_staMu_numberOfValidMuonHits;   // Number of valid muon hits
  std::vector<float> m_staMu_normChi2;                // chi2/ndof (standalone track)
  std::vector<float> m_glbMu_normChi2;                // chi2/ndof (global track)

  std::vector<unsigned int> m_nMatches;// Number of muon chamber matches (DT only)

  TClonesArray *m_matches_wheel;   // Index associated to the matched chamber wheel 
  TClonesArray *m_matches_sector;  // Index associated to the matched chamber sector
  TClonesArray *m_matches_station; // Index associated to the matched chamber station

  TClonesArray *m_matches_x; // X position of the extrapolated track on the matched chamber
  TClonesArray *m_matches_y; // Y position of the extrapolated track on the matched chamber

  TClonesArray *m_matches_phi;   // Phi of the (x,y) position on the matched chamber (global reference frame)
  TClonesArray *m_matches_eta;   // Eta of the (x,y) position on the matched chamber (global reference frame)
  TClonesArray *m_matches_edgeX; // Distance to closest edge in X on the matched chamber (<0 means inside chamber)
  TClonesArray *m_matches_edgeY; // Distance to closest edge in Y on the matched chamber (<0 means inside chamber)

  TClonesArray *m_matches_dXdZ; // dXdZ of the extrapolated track on the matched chamber
  TClonesArray *m_matches_dYdZ; // dYdZ of the extrapolated track on the matched chamber

  TClonesArray *m_matchSegIdx; // Index of segments used in the standalone track

};
  
#endif
