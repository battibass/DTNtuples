#ifndef DTNtuple_DTNtupleConfig_h
#define DTNtuple_DTNtupleConfig_h

/** \class DTNtupleConfig DTNtupleConfig.h DTDPGAnalysis/DTNtuples/src/DTNtupleConfig.h
 *  
 * Helper class to handle :
 * - configuration parameters for edm::ParameterSet
 * - DB information from edm::EventSetup
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "FWCore/Utilities/interface/InputTag.h"

#include <map>
#include <string>

namespace edm
{
  class ParameterSet;
  class EventSetup;
}

class DTNtupleConfig
{

 public :

  /// Constructor
  DTNtupleConfig(const edm::ParameterSet & config);

  /// Update EventSetup information
  void getES(const edm::EventSetup & environment);

  /// Map containing different input tags
  std::map<std::string, edm::InputTag> m_inputTags;

};

#endif
