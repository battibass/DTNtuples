#ifndef DTNtuple_DTNtupleTPGThetaFiller_h
#define DTNtuple_DTNtupleTPGThetaFiller_h

/** \class DTNtupleTPGThetaFiller DTNtupleTPGThetaFiller.h DTDPGAnalysis/DTNtuples/src/DTNtupleTPGThetaFiller.h
 *  
 * Helper class : the Phase-1 local trigger filler for TwinMux and BMTF in (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBaseFiller.h"

#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <vector>

class DTNtupleTPGThetaFiller : public DTNtupleBaseFiller
{

 public:

  enum class TriggerTag { TM_IN = 0, BMTF_IN };

  /// Constructor
  DTNtupleTPGThetaFiller(edm::ConsumesCollector && collector,
			     const std::shared_ptr<DTNtupleConfig> config, 
			     std::shared_ptr<TTree> tree, const std::string & label, 
			     TriggerTag tag);

  ///Destructor
  virtual ~DTNtupleTPGThetaFiller();
 
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
  edm::EDGetTokenT<L1MuDTChambThContainer> m_dtTriggerToken;

  /// The variables holding
  /// all digi related information

  unsigned int m_nTrigs; // the # of digis (size of all following vectors)

  std::vector<short> m_lt_wheel;   // wheel (short in [-2:2] range)
  std::vector<short> m_lt_sector;  // sector (short in [1:12] range for TwinMux)
                                   //        (short in [0:11] range for BMTF input) 
                                   // double MB4 stations are part of S4 and S10 in TwinMux
                                   // double MB4 stations are part of S3 and S9 in BMTF input
  std::vector<short> m_lt_station; // station (short in [1:4] range)

  std::vector<short> m_lt_bx;    // BX : (short with a given range)
                                 // BX = 0 is the one where the event is collected
                                 // TwinMux range [X:Y] // CB ask Luigi
                                 // BMTF input range [X:Y] // CB ask Luigi

  std::vector<unsigned short> m_lt_hitMap; // map groups of BTIs that fired in an  unsigned short :
                                           // there are 7 groups of BTI per chamber, the first one
                                           // being the less significant bit of the short // CB ask Luigi

};
  
#endif

