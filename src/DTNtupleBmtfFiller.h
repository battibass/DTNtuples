#ifndef DTNtuple_DTNtupleBmtfFiller_h
#define DTNtuple_DTNtupleBmtfFiller_h

/** \class DTNtupleBmtfFiller DTNtupleBmtfFiller.h DTDPGAnalysis/DTNtuples/src/DTNtupleBmtfFiller.h
 *  
 * Helper class : the bmtf filler
 *
 * \author L. Borgonovi (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBaseFiller.h"
#include "DTDPGAnalysis/DTNtuples/src/DTNtupleTPGPhiFiller.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
 
#include <vector>

class DTNtupleBmtfFiller : public DTNtupleBaseFiller
{

 public:

  /// Constructor
  DTNtupleBmtfFiller(edm::ConsumesCollector && collector,
		    const std::shared_ptr<DTNtupleConfig> config, 
		    std::shared_ptr<TTree> tree, const std::string & label);

  ///Destructor
  virtual ~DTNtupleBmtfFiller();
 
  /// Intialize function : setup tree branches etc ... 
  virtual void initialize() final;
  
  /// Clear branches before event filling 
  virtual void clear() final;

  /// Fill tree branches for a given events
  virtual void fill(const edm::Event & ev) final;    

 private :

  /// The gen particles token
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> m_dtBmtfToken;
  edm::EDGetTokenT<L1MuDTChambPhContainer> m_dtTpgPhiToken;

  unsigned int m_nMuCands; // the # of digis (size of all following vectors)    

  std::vector<float> m_tf_pt;       // pt
  std::vector<int> m_tf_phi;        // phi
  std::vector<float> m_tf_eta;      // eta
  std::vector<int> m_tf_dxy;        // dxy
  std::vector<int> m_tf_qual;       // trigger quality
  std::vector<int> m_tf_hf;         // HF (halo / fine) eta
  TClonesArray *m_muCandStubs;      // index of trigger stubs
};
  
#endif
