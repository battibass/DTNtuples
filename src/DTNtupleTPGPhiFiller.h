#ifndef DTNtuple_DTNtupleTPGPhiFiller_h
#define DTNtuple_DTNtupleTPGPhiFiller_h

/** \class DTNtupleTPGPhiFiller DTNtupleTPGPhiFiller.h DTDPGAnalysis/DTNtuples/src/DTNtupleTPGPhiFiller.h
 *  
 * Helper class : the Phase-1 local trigger filler for TwinMux in/out and BMTF in (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBaseFiller.h"

#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <vector>

class DTNtupleTPGPhiFiller : public DTNtupleBaseFiller
{

 public:

  enum class TriggerTag { TM_IN = 0, TM_OUT, BMTF_IN };

  /// Constructor
  DTNtupleTPGPhiFiller(edm::ConsumesCollector && collector,
			     const std::shared_ptr<DTNtupleConfig> config, 
			     std::shared_ptr<TTree> tree, const std::string & label, 
			     TriggerTag tag);

  ///Destructor
  virtual ~DTNtupleTPGPhiFiller();
 
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
  edm::EDGetTokenT<L1MuDTChambPhContainer> m_dtTriggerToken;

  /// The variables holding
  /// all digi related information

  unsigned int m_nTrigs; // the # of digis (size of all following vectors)

  std::vector<short> m_lt_wheel;   // wheel (short in [-2:2] range)
  std::vector<short> m_lt_sector;  // sector (short in [1:12] range for TwinMux)
                                   //        (short in [0:11] range for BMTF input) 
                                   // double MB4 stations are part of S4 and S10 in TwinMux
                                   // double MB4 stations are part of S3 and S9 in BMTF input
  std::vector<short> m_lt_station; // station (short in [1:4] range)

  std::vector<short> m_lt_quality; // quality (short in [0:6] range)
                                   // [0:1] : uncorrelated L triggers
                                   // [2:3] : uncorrelated H triggers
                                   // 4 : correlated LL triggers
                                   // 5 : correlated HL triggers
                                   // 6 : correlated HH triggers
  std::vector<short> m_lt_rpcBit;  // use of RPC (short in [0:2] range)
                                   // 0 : RPC not used
                                   // 1 : RPC+DT combined trigger
                                   // 2 : RPC-only trigger

  std::vector<int> m_lt_phi;  // phi : (int with a given scale)
                              // 4096 correstpond to 1 rad
                              // 0 is @ (sector - 1) * 30 deg in global CMS phi (TwinMux)
  std::vector<int> m_lt_phiB; // phi bending : (int with a given scale)
                              // 512 correstpond to 1 rad
                              // 0 is a muon with infinite pT (straight line)

  std::vector<float> m_lt_posLoc_x; // position x in chamber local coordinates (cm)
  std::vector<float> m_lt_dirLoc_phi; // direction phi angle in chamber local coordinates (deg)

  std::vector<short> m_lt_bx;    // BX : (short with a given range)
                                 // BX = 0 is the one where the event is collected
                                 // TwinMux range [X:Y] // CB ask Luigi
                                 // BMTF input range [X:Y] // CB ask Luigi
  std::vector<short> m_lt_is2nd; // 1st/2nd track flag : (short [0:1])
                                 // 0 (1) is first (second) track

};
  
#endif

