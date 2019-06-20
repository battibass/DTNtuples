#ifndef DTNtuple_DTNtupleSegmentFiller_h
#define DTNtuple_DTNtupleSegmentFiller_h

/** \class DTNtupleSegmentFiller DTNtupleSegmentFiller.h DTDPGAnalysis/DTNtuples/src/DTNtupleSegmentFiller.h
 *  
 * Helper class : the segment filler for Phase-1 / Phase2 segments (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBaseFiller.h"

#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "TVectorF.h"

#include <vector>



class DTNtupleSegmentFiller : public DTNtupleBaseFiller
{

 public:

  enum class SegmentTag { PH1 = 0, PH2 };

  /// Constructor
  DTNtupleSegmentFiller(edm::ConsumesCollector && collector,
		     const std::shared_ptr<DTNtupleConfig> config, 
		     std::shared_ptr<TTree> tree, const std::string & label, 
		     SegmentTag tag);

  ///Destructor
  virtual ~DTNtupleSegmentFiller();
 
  /// Intialize function : setup tree branches etc ... 
  virtual void initialize() final;
  
  /// Clear branches before event filling 
  virtual void clear() final;

  /// Fill tree branches for a given event
  virtual void fill(const edm::Event & ev) final;    

  /// Fill 2D segment phi component for a given event
  void fillPhi(const DTChamberRecSegment2D* phiSeg, const GeomDet* chamb);

  /// Fill 2D segment z component for a given event
  void fillZ(const DTSLRecSegment2D* zSeg, const GeomDet* chamb);

  /// Fill with an empty 2D segment phi component for a given event
  void fillPhiEmpty();

  /// Fill with an empty 2D segment z component for a given event
  void fillZEmpty();

 private :

  /// Enum to activate "flavour-by-flavour"
  /// changes in the filling logic
  SegmentTag m_tag;

  /// The default TVercorF for empty vectors
  TVectorF m_nullVecF;

  /// The digi token
  edm::EDGetTokenT<DTRecSegment4DCollection> m_dtSegmentToken;

  /// The variables holding
  /// all digi related information

  unsigned int m_nSegments; // the # of digis (size of all following vectors)

  std::vector<short> m_seg4D_wheel;   // wheel (short in [-2:2] range)
  std::vector<short> m_seg4D_sector;  // sector (short in [1:14] range)
                                      // sector 13 used for the second MB4 of sector 4
                                      // sector 14 used for the second MB4 of sector 10
  std::vector<short> m_seg4D_station; // station (short in [1:4] range)
 
  std::vector<short> m_seg4D_hasPhi; // has segment phi view (0/1 = no/yes)
  std::vector<short> m_seg4D_hasZed; // has segment zed view (0/1 = no/yes)

  std::vector<float> m_seg4D_posLoc_x; // position x in local coordinates (float in cm)
  std::vector<float> m_seg4D_posLoc_y; // position y in local coordinates (float in cm)
  std::vector<float> m_seg4D_posLoc_z; // position z in local coordinates (float in cm)
  std::vector<float> m_seg4D_dirLoc_x; // direction x in local coordinates (float)
  std::vector<float> m_seg4D_dirLoc_y; // direction y in local coordinates (float)
  std::vector<float> m_seg4D_dirLoc_z; // direction z in local coordinates (float)

  std::vector<float> m_seg4D_posLoc_x_SL1; // position x at SL1 in local coordinates (float in cm)
  std::vector<float> m_seg4D_posLoc_x_SL3; // position x at SL3 in local coordinates (float in cm)
  std::vector<float> m_seg4D_posLoc_x_midPlane; // position x at SL1 - SL3 mid plane in local coordinates (float in cm)

  std::vector<float> m_seg4D_posGlb_phi; // position phi in global coordinates (float in radians [-pi:pi])
  std::vector<float> m_seg4D_posGlb_eta; // position eta in global coordinates (float)
  std::vector<float> m_seg4D_dirGlb_phi; // position phi in global coordinates (float in radians [-pi:pi])
  std::vector<float> m_seg4D_dirGlb_eta; // position eta in global coordinates (float)

  // TClones arrays index is layer [0:3] = SL phi 1 [4:7] = SL theta [8:11] = SL phi 2
  TClonesArray *m_seg4D_hitsExpPos;     // expected position of segment extrapolated 
                                        // to a given layer in layer local coordinates
                                        // (float, local layer x coordinates, cm)
  TClonesArray *m_seg4D_hitsExpPosCh;   // expected position of segment extrapolated 
                                        // to a given layer in chamber local coordinates
                                        // (float, local chamber x/y coordinates, cm)
  TClonesArray *m_seg4D_hitsExpWire;    // expected wire crossed by segment extrapolated
                                        // to a given layer (int, range depends on chamber size)

  std::vector<float> m_seg2D_phi_t0;       // t0 from segments with phi view (float in ns)
  std::vector<float> m_seg2D_phi_vDrift;   // v_drift from segments with phi view (float CB relativa a DB?)
  std::vector<float> m_seg2D_phi_normChi2; // chi2/n.d.o.f. from segments with phi view (float)

  std::vector<short> m_seg2D_phi_nHits; // # hits in phi view (short in [0:8] range)

  // TClones arrays index is the hit number for hits in the phi view
  TClonesArray *m_seg2D_phiHits_pos;        // local position of a hit in layer local coordinates (float, x coordinate)
  TClonesArray *m_seg2D_phiHits_posCh;      // local position of a hit in chamber local coordinates (float, x coordinate)
  TClonesArray *m_seg2D_phiHits_posErr;     // local position error of a hit in layer local coordinates (float, xx component of error matrix)
  TClonesArray *m_seg2D_phiHits_side;       // is hit on L/R side of a cell wire (float, 1/2 is R/L) 
  TClonesArray *m_seg2D_phiHits_wire;       // hit wire number (int, [1:X] X as for digi)
  TClonesArray *m_seg2D_phiHits_wirePos;    // hit wire position (float, x coordinate in layer RF)
  TClonesArray *m_seg2D_phiHits_layer;      // hit layer number (float, [1:4] as for digi)
  TClonesArray *m_seg2D_phiHits_superLayer; // hit SL number (float, [1 or 3] the two phi SLs)
  TClonesArray *m_seg2D_phiHits_time;       // digi time (float in ns, pedestal not subtracted) 
  TClonesArray *m_seg2D_phiHits_timeCali;   // digi time (float in ns, pedestal subtracted)

  std::vector<float> m_seg2D_z_normChi2; // chi2/n.d.o.f. from segments with z view (float)

  std::vector<short> m_seg2D_z_nHits; // # hits in z view (short in [0:4] range) 

  TClonesArray *m_seg2D_zHits_pos;       // local position of a hit in layer local coordinates (float, x coordinate)  
  TClonesArray *m_seg2D_zHits_posCh;     // local position of a hit in chamber local coordinates (float, y coordinate)
  TClonesArray *m_seg2D_zHits_posErr;    // local position error of a hit in layer local coordinates (float, xx component of error matrix)
  TClonesArray *m_seg2D_zHits_side;      // is hit on L/R side of a cell wire (float, 1/2 is R/L)
  TClonesArray *m_seg2D_zHits_wire;      // hit wire number (int, [1:X] X as for digi) 
  TClonesArray *m_seg2D_zHits_wirePos;   // hit wire position (float, x coordinate in layer RF)
  TClonesArray *m_seg2D_zHits_layer;     // hit layer number (float, [1:4] as for digi)
  TClonesArray *m_seg2D_zHits_time;      // digi time (float in ns, pedestal not subtracted)
  TClonesArray *m_seg2D_zHits_timeCali;  // digi time (float in ns, pedestal subtracted) 

};
  
#endif

