#ifndef DTNtuple_DTNtuplePh2TPGPhiFiller_h
#define DTNtuple_DTNtuplePh2TPGPhiFiller_h

/** \class DTNtuplePh2TPGPhiFiller DTNtuplePh2TPGPhiFiller.h DTDPGAnalysis/DTNtuples/src/DTNtuplePh2TPGPhiFiller.h
 *  
 * Helper class : the Phase-1 local trigger filler for TwinMux in/out and BMTF in (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBaseFiller.h"

#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTPhContainer.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <vector>

class DTNtuplePh2TPGPhiFiller : public DTNtupleBaseFiller
{

 public:

  enum class TriggerTag { HW = 0, HB, AM };

  /// Constructor
  DTNtuplePh2TPGPhiFiller(edm::ConsumesCollector && collector,
			     const std::shared_ptr<DTNtupleConfig> config, 
			     std::shared_ptr<TTree> tree, const std::string & label, 
			     TriggerTag tag);

  ///Destructor
  virtual ~DTNtuplePh2TPGPhiFiller();
 
  /// Intialize function : setup tree branches etc ... 
  virtual void initialize() final;
  
  /// Clear branches before event filling 
  virtual void clear() final;

  /// Fill tree branches for a given events
  virtual void fill(const edm::Event & ev) final;    

 private :

  /// Enum to activate "flavour-by-flavour"
  /// changes in the filling logic
  TriggerTag m_tag;

  /// The digi token
  edm::EDGetTokenT<L1Phase2MuDTPhContainer> m_dtTriggerToken;

  /// The variables holding
  /// all digi related information

  unsigned int m_nTrigs; // the # of digis (size of all following vectors)

  std::vector<short> m_lt_wheel;   // wheel (short in [-2:2] range)
  std::vector<short> m_lt_sector;  // sector (short in [1:12] range)
  std::vector<short> m_lt_station; // station (short in [1:4] range)

  std::vector<short> m_lt_quality; // quality (short in [X:Y] range)
                                   // ... // CB to be defined

  std::vector<short> m_lt_superLayer; // superlayer (short in [X:Y] range)
                                      // ... // CB to be defined

  std::vector<int> m_lt_chi2; // chi2 (int in [X:Y] range)
                              // ... // CB to be defined

  std::vector<short> m_lt_rpcFlag; // quality (short in [X:Y] range)
                                   // ... // CB to be defined

  std::vector<int> m_lt_phi;  // phi : (int with a given scale)
                              // 65536 corresponds to 0.8 rad
                              // 0 is @ (sector - 1) * 30 deg in global CMS phi
  std::vector<int> m_lt_phiB; // phi bending : (int with a given scale)
                              // 2048 corresponds to 1.4 rad
                              // 0 is for a segment from a prompt muon 
                              // with infinite pT (straight line)

  std::vector<float> m_lt_posLoc_x; // position x in chamber local coordinates (cm)
  std::vector<float> m_lt_dirLoc_phi; // direction phi angle in chamber local coordinates (deg)

  std::vector<int> m_lt_bx;  // BX : (short with a given range)
                             // ... // CB to be defined
  std::vector<int> m_lt_t0;  // t0 - time with sub BX precision: 
                             // (int with a given scale) // CB to be defined

  std::vector<short> m_lt_index; // index : (short in [X:Y] range) 
                                 // tags multiple primitives per chamber per BX
                                 // ... // CB to be defined

};
  
#endif
