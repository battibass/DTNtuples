#ifndef DTNtuple_DTNtupleEnvironmentFiller_h
#define DTNtuple_DTNtupleEnvironmentFiller_h

/** \class DTNtupleEnvironmentFiller DTNtupleEnvironmentFiller.h DTDPGAnalysis/DTNtuples/src/DTNtupleEnvironmentFiller.h
 *  
 * Helper class : the pile-up, luminosity and reco vertices filler
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBaseFiller.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "DataFormats/Scalers/interface/LumiScalers.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include <vector>

class DTNtupleEnvironmentFiller : public DTNtupleBaseFiller
{

 public:

  /// Constructor
  DTNtupleEnvironmentFiller(edm::ConsumesCollector && collector,
			    const std::shared_ptr<DTNtupleConfig> config, 
			    std::shared_ptr<TTree> tree, const std::string & label);

  ///Destructor
  virtual ~DTNtupleEnvironmentFiller();
 
  /// Intialize function : setup tree branches etc ... 
  virtual void initialize() final;
  
  /// Clear branches before event filling 
  virtual void clear() final;

  /// Fill tree branches for a given events
  virtual void fill(const edm::Event & ev) final;    

 private :

  /// The PU info token
  edm::EDGetTokenT<std::vector<PileupSummaryInfo> > m_puInfoToken;

  /// The lumi scalers token
  edm::EDGetTokenT<LumiScalersCollection> m_lumiScalerToken;

  /// The primary vertices token
  edm::EDGetTokenT<reco::VertexCollection> m_primariVerticesToken;

  short m_truePileUp;   // The <PU> value reflecting the luminosity per bunch (short)
  short m_actualPileUp; // The actual number of PU interaction 
                        // (poissonian distribution with avg. = <PU>, short)

  int m_instLumi; // The instantaneous luminosity from online scalers
                  // (int in unit 10^30 cm^-1 s^-1)

  short m_nPV;  // Number of reconstructed primary vertices (short)

  float m_pv_x; // First primary vertex x coordinate (float, cm)
  float m_pv_y; // First primary vertex y coordinate (float, cm) 
  float m_pv_z; // First primary vertex z coordinate (float, cm) 

  float m_pv_xxErr; // First primary vertex xx error component (float) 
  float m_pv_yyErr; // First primary vertex yy error component (float)
  float m_pv_zzErr; // First primary vertex zz error component (float)
  float m_pv_xyErr; // First primary vertex xy error component (float)
  float m_pv_xzErr; // First primary vertex xz error component (float)
  float m_pv_yzErr; // First primary vertex yz error component (float)

};
  
#endif
