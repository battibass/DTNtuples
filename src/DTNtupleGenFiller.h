#ifndef DTNtuple_DTNtupleGenFiller_h
#define DTNtuple_DTNtupleGenFiller_h

/** \class DTNtupleGenFiller DTNtupleGenFiller.h DTDPGAnalysis/DTNtuples/src/DTNtupleGenFiller.h
 *  
 * Helper class : the gen particles filler
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBaseFiller.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <vector>

class DTNtupleGenFiller : public DTNtupleBaseFiller
{

 public:

  /// Constructor
  DTNtupleGenFiller(edm::ConsumesCollector && collector,
		    const std::shared_ptr<DTNtupleConfig> config, 
		    std::shared_ptr<TTree> tree, const std::string & label);

  ///Destructor
  virtual ~DTNtupleGenFiller();
 
  /// Intialize function : setup tree branches etc ... 
  virtual void initialize() final;
  
  /// Clear branches before event filling 
  virtual void clear() final;

  /// Fill tree branches for a given events
  virtual void fill(const edm::Event & ev) final;    

 private :

  /// The gen particles token
  edm::EDGetTokenT<reco::GenParticleCollection> m_genPartToken;
  
  unsigned int m_nGenParts; // the # of gen particles (size of all following vectors)

  std::vector<int>   m_gen_pdgId; // the gen particle pdgId 
                                  // (https://twiki.cern.ch/twiki/bin/view/Main/PdgId)

  std::vector<float> m_gen_pt;     // gen particle pT (float)
  std::vector<float> m_gen_phi;    // gen particle phi (float in [-pi:pi] range) // CB check range
  std::vector<float> m_gen_eta;    // gen particle eta
  std::vector<short> m_gen_charge; // gen particle charge
  
};
  
#endif
