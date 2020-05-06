#ifndef DTNtuple_DTNtupleConfig_h
#define DTNtuple_DTNtupleConfig_h

/** \class DTNtupleConfig DTNtupleConfig.h DTDPGAnalysis/DTNtuples/src/DTNtupleConfig.h
 *  
 * Helper class to handle :
 * - configuration parameters for edm::ParameterSet
 * - DB information from edm::EventSetup
 * - HLT configuration from dm::EventSetup and dm::Run
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

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include <map>
#include <string>
#include <memory>

namespace edm
{
  class ParameterSet;
  class EventSetup;
  class Run;
}

class DTNtupleConfig
{

 public :

  /// Constructor
  DTNtupleConfig(const edm::ParameterSet & config);

  /// Update EventSetup information
  void getES(const edm::EventSetup & environment);

  /// Update EventSetup information
  void getES(const edm::Run &run, 
	     const edm::EventSetup & environment);

  /// Map containing different input tags
  std::map<std::string, edm::InputTag> m_inputTags;

  /// The class to handle DT trigger time pedestals
  std::unique_ptr<DTTTrigBaseSync> m_dtSync;

  /// The class to perform DT local trigger coordinate conversions
  std::unique_ptr<DTTrigGeomUtils> m_trigGeomUtils;

  /// Handle to the tracking geometry
  edm::ESHandle<GlobalTrackingGeometry> m_trackingGeometry;

  /// Handle to the DT geometry
  edm::ESHandle<DTGeometry> m_dtGeometry;

  /// HLT config procider
  HLTConfigProvider m_hltConfig;
  
  /// Name and indices of the isolated trigger used by muon filler for trigger matching
  std::string m_isoTrigName;
  std::vector<int> m_isoTrigIndices;

  /// Name and indices of the non isolated trigger used by muon filler for trigger matching
  std::string m_trigName;
  std::vector<int> m_trigIndices;

};

#endif
