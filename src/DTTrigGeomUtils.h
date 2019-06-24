#ifndef DTTrigGeomUtils_H
#define DTTrigGeomUtils_H

/*
 * \file DTTrigGeomUtils.h
 *
 * \author C. Battilana - CIEMAT
 *
*/

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/GeometryVector/interface/Pi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTPhDigi.h"
#include<cmath>

class DTGeometry;
class DTRecSegment4D;
class DTChamberId;
class L1MuDTChambPhDigi;
class L1MuDTChambDigi;

class DTTrigGeomUtils {

 public:

  /// Constructor
  DTTrigGeomUtils(edm::ESHandle<DTGeometry> muonGeom, bool dirInDeg=true);

  /// Destructor
  virtual ~DTTrigGeomUtils();

  /// Compute phi range in local chamber coordinates
  void phiRange(const DTChamberId& id, float& min, float& max, int&nbins, float step=15);

  /// Compute theta range in local chamber coordinates
  void thetaRange(const DTChamberId& id, float& min, float& max, int& nbins, float step=15);

  /// Compute track coordinates with SC sector numbering
  void computeSCCoordinates(const DTRecSegment4D* track, int& scsec, float& x, float& xdir, float& y, float& ydir);

  /// Return local position (trigger RF) for a given trigger primitive, legacy
  float trigPos(const L1MuDTChambPhDigi* trig);

  /// Return local direction (trigger RF) for a given trigger primitive, legacy
  float trigDir(const L1MuDTChambPhDigi* trig);

  /// Return local position (trigger RF) for a given trigger primitive, CHT algo
  float trigPosCHT(const L1Phase2MuDTPhDigi* trig);

  /// Return local direction (trigger RF) for a given trigger primitive, CHT algo
  float trigDirCHT(const L1Phase2MuDTPhDigi* trig);

  /// Return local position (trigger RF) for a given trigger primitive, AM algo
  float trigPosAM(const L1Phase2MuDTPhDigi* trig);

  /// Return local direction (trigger RF) for a given trigger primitive, AM algo
  float trigDirAM(const L1Phase2MuDTPhDigi* trig);

  /// Compute Trigger x coordinate in chamber RF
  void trigToSeg(int st, float& x, float dir) { x -= tan(dir/radToDeg_)*zcn_[st-1]; };

  /// Checks id the chamber has positive RF;
  bool hasPosRF(int wh, int sec) { return wh>0 || (wh==0 && sec%4>1); };

 private:

  edm::ESHandle<DTGeometry> muonGeom_;
  float  zcn_[4];
  float radToDeg_;
  float xCenter_[5][4];
  float zsl1_[4];
  float zsl3_[4];
};

#endif

/* Local Variables: */
/* show-trailing-whitespace: t */
/* truncate-lines: t */
/* End: */
