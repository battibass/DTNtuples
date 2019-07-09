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
#include "FWCore/Framework/interface/ESHandle.h"

#include "CalibMuon/DTDigiSync/interface/DTTTrigBaseSync.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"

#include "DTDPGAnalysis/DTNtuples/src/DTTrigGeomUtils.h"

#include <map>
#include <string>
#include <memory>

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

  /// The class to handle DT trigger time pedestals
  DTTTrigBaseSync * m_dtSync; // CB find a way to handle this with a smart pointer

  /// The class to perform DT local trigger coordinate conversions
  std::unique_ptr<DTTrigGeomUtils> m_trigGeomUtils;

  /// Handle to the tracking geometry
  edm::ESHandle<GlobalTrackingGeometry> m_trackingGeometry;

  /// Handle to the DT geometry
  edm::ESHandle<DTGeometry> m_dtGeometry;

};

#endif
