#ifndef DTNtuple_DTNtupleDigiFiller_h
#define DTNtuple_DTNtupleDigiFiller_h

/** \class DTNtupleDigiFiller DTNtupleDigiFiller.h DTDPGAnalysis/DTNtuples/src/DTNtupleDigiFiller.h
 *  
 * Helper class : the digi filler for Phase-1 / Phase2 digis (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBaseFiller.h"

#include "DataFormats/DTDigi/interface/DTDigiCollection.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <vector>

class DTNtupleDigiFiller : public DTNtupleBaseFiller
{

 public:

  enum class DigiTag { PH1 = 0, PH2 };

  /// Constructor
  DTNtupleDigiFiller(edm::ConsumesCollector && collector,
		     const std::shared_ptr<DTNtupleConfig> config, 
		     std::shared_ptr<TTree> tree, const std::string & label, 
		     DigiTag tag);

  ///Destructor
  virtual ~DTNtupleDigiFiller();
 
  /// Intialize function : setup tree branches etc ... 
  virtual void initialize() final;
  
  /// Clear branches before event filling 
  virtual void clear() final;

  /// Fill tree branches for a given events
  virtual void fill(const edm::Event & ev) final;    

 private :

  /// Enum to activate "flavour-by-flavour"
  /// changes in the filling logic
  DigiTag m_tag;

  /// The digi token
  edm::EDGetTokenT<DTDigiCollection> m_dtDigiToken;

  /// The variables holding
  /// all digi related information

  unsigned int m_nDigis; // the # of digis (size of all following vectors)

  std::vector<short> m_digi_wheel;   // wheel (short in [-2:2] range)
  std::vector<short> m_digi_sector;  // sector (short in [1:14] range)
                                     // sector 13 used for the second MB4 of sector 4
                                     // sector 14 used for the second MB4 of sector 10
  std::vector<short> m_digi_station; // station (short in [1:4] range)
  
  std::vector<short> m_digi_superLayer; // superlayer (short in [1:3] range)
                                        // SL 1 and 3 are phi SLs
                                        // SL 2 is theta SL
  std::vector<short> m_digi_layer;      // layer (short in [1:4] range)
  std::vector<short> m_digi_wire;       // wire (short in [1:X] range)
                                        // X varies for different chambers SLs and layers

  std::vector<float> m_digi_time; // float with digi time in ns (no pedestal subtraction) 

};
  
#endif

