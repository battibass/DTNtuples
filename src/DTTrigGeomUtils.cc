/*
 * \file DTTrigGeomUtils.cc
 *
 * \author C. Battilana - CIEMAT
 *
*/

#include "DTDPGAnalysis/DTNtuples/src/DTTrigGeomUtils.h"

// Framework
#include "FWCore/Framework/interface/EventSetup.h"

// Trigger
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTPhDigi.h"

// Geometry & Segment
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/DTGeometry/interface/DTChamber.h"
#include "Geometry/DTGeometry/interface/DTSuperLayer.h"
#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Geometry/DTGeometry/interface/DTTopology.h"

#include <iostream>

using namespace edm;
using namespace std;


DTTrigGeomUtils::DTTrigGeomUtils(ESHandle<DTGeometry> muonGeom, bool dirInDeg) : muonGeom_(muonGeom) 
{

  radToDeg_ = dirInDeg ? 180./Geom::pi() : 1;
  
  for (int ist=1; ist<=4; ++ist) 
    {
      const DTChamberId chId(-2,ist,4);
      const DTChamber *chamb = muonGeom_->chamber(chId);
      const DTSuperLayer *sl1 = chamb->superLayer(DTSuperLayerId(chId,1));
      const DTSuperLayer *sl3 = chamb->superLayer(DTSuperLayerId(chId,3));
      zsl1_[ist-1]=chamb->surface().toLocal(sl1->position()).z();
      zsl3_[ist-1]=chamb->surface().toLocal(sl3->position()).z();
      zcn_[ist-1] = 0.5*(zsl1_[ist-1] + zsl3_[ist-1]);
      //std::cout<<"Station "<<ist<<" zsl1="<<zsl1_[ist-1]<<" zsl3="<<zsl3_[ist-1]<<" zmid="<<zcn_[ist-1]<<std::endl;
  }

  for (int iwh=-2; iwh<2; iwh++)
    {
      const DTChamber* chambS4   = muonGeom_->chamber(DTChamberId(iwh,4,4));
      xCenter_[iwh+2][0]=chambS4->position().x();
      const DTChamber* chambS13   = muonGeom_->chamber(DTChamberId(iwh,4,13));
      xCenter_[iwh+2][1]=chambS13->position().x();
      const DTChamber* chambS10   = muonGeom_->chamber(DTChamberId(iwh,4,10));
      xCenter_[iwh+2][2]=chambS10->position().x();
      const DTChamber* chambS14   = muonGeom_->chamber(DTChamberId(iwh,4,14));
      xCenter_[iwh+2][3]=chambS14->position().x();
    }
}


DTTrigGeomUtils::~DTTrigGeomUtils() 
{
  
}


void DTTrigGeomUtils::computeSCCoordinates(const DTRecSegment4D* track, int& scsec, float& x, float& xdir, float& y, float& ydir)
{

  int wheel   = track->chamberId().wheel();
  int sector  = track->chamberId().sector();
  int station = track->chamberId().station();
  xdir = atan(track->localDirection().x()/ track->localDirection().z())*radToDeg_;
  ydir = atan(track->localDirection().y()/ track->localDirection().z())*radToDeg_;

  scsec = sector>12 ? sector==13 ? 4 : 10 : sector;

  int scsecidx=0;;
  if (sector==4) scsecidx=0;
  if (sector==13) scsecidx=1;
  if (sector==10) scsecidx=2;
  if (sector==14) scsecidx=3;
  
  float xcenter = (scsec==4||scsec==10) ? (sector-12.9)/abs(sector-12.9)*xCenter_[wheel+2][scsecidx] : 0.;
  x = track->localPosition().x()+xcenter*(station==4);
  y = track->localPosition().y();
  
}


void DTTrigGeomUtils::phiRange(const DTChamberId& id, float& min, float& max, int& nbins, float step)
{
  
  int station = id.station();
  int sector  = id.sector();

  const DTLayer *layer   = muonGeom_->layer(DTLayerId(id,1,1));
  const DTTopology &topo = layer->specificTopology();
  double range = topo.channels()*topo.cellWidth();
  min = -range*.5;
  max =  range*.5;

  if (station==4 && (sector==4 || sector == 10))
    {
      min = -range-10;
      max =  range+10;
    }
  nbins = static_cast<int>((max-min)/step);

  return;
  
}


void DTTrigGeomUtils::thetaRange(const DTChamberId& id, float& min, float& max, int& nbins, float step)
{

  const DTLayer  *layer = muonGeom_->layer(DTLayerId(id,2,1));
  const DTTopology& topo = layer->specificTopology();
  double range = topo.channels()*topo.cellWidth();
  min = -range*.5;
  max =  range*.5;
  
  nbins = static_cast<int>((max-min)/step);
  
  return;
  
}


float DTTrigGeomUtils::trigPos(const L1MuDTChambPhDigi* trig)
{

  int wh   = trig->whNum();
  int sec  = trig->scNum()+1;
  int st   = trig->stNum();
  int phi  = trig->phi();

  float phin = (sec-1)*Geom::pi()/6;
  float phicenter = 0;
  float r = 0;
  
  if (sec==4 && st==4) 
    {
      GlobalPoint gpos = phi>0 ? muonGeom_->chamber(DTChamberId(wh,st,13))->position() : muonGeom_->chamber(DTChamberId(wh,st,4))->position();
      phicenter =  gpos.phi();
      r = gpos.perp();
    } 
  else if (sec==10 && st==4) 
    {
      GlobalPoint gpos = phi>0 ? muonGeom_->chamber(DTChamberId(wh,st,14))->position() : muonGeom_->chamber(DTChamberId(wh,st,10))->position();
      phicenter =  gpos.phi();
      r = gpos.perp();
    } 
  else 
    {
      GlobalPoint gpos = muonGeom_->chamber(DTChamberId(wh,st,sec))->position();
      phicenter =  gpos.phi();
      r = gpos.perp();
    }
  float deltaphi = phicenter-phin;
  if (phi<0) phi--;
  float x = r*tan(phi/4096.)*cos(deltaphi)-r*sin(deltaphi); //zcn is in local coordinates -> z invreases approching to vertex
  // LG: zcn offset was removed 
  if (hasPosRF(wh,sec)){ x = -x; } // change sign in case of positive wheels
  return x;

}


float DTTrigGeomUtils::trigDir(const L1MuDTChambPhDigi* trig)
{

  int wh   = trig->whNum();
  int sec  = trig->scNum()+1;
  int phi  = trig->phi();
  int phib = trig->phiB();
  
  float dir = (phib/512.+phi/4096.)*radToDeg_;
  
  // change sign in case of negative wheels
  if (!hasPosRF(wh,sec)) { dir = -dir; }
  
  return dir;
  
}

float DTTrigGeomUtils::trigPosCHT(const L1Phase2MuDTPhDigi* trig)
{

  int wh   = trig->whNum();
  int sec  = trig->scNum()+1;
  int st   = trig->stNum();
  int phi  = trig->phi();
  int quality = trig->quality();
  int sl = trig->slNum();

  float phin = (sec-1)*Geom::pi()/6;
  float phicenter = 0;
  float r = 0;
  
  if (sec==4 && st==4) 
    {
      GlobalPoint gpos = phi>0 ? muonGeom_->chamber(DTChamberId(wh,st,13))->position() : muonGeom_->chamber(DTChamberId(wh,st,4))->position();
      phicenter =  gpos.phi();
      r = gpos.perp();
    } 
  else if (sec==10 && st==4) 
    {
      GlobalPoint gpos = phi>0 ? muonGeom_->chamber(DTChamberId(wh,st,14))->position() : muonGeom_->chamber(DTChamberId(wh,st,10))->position();
      phicenter =  gpos.phi();
      r = gpos.perp();
    } 
  else 
    {
      GlobalPoint gpos = muonGeom_->chamber(DTChamberId(wh,st,sec))->position();
      phicenter =  gpos.phi();
      r = gpos.perp();
    }

  float deltaphi = phicenter-phin;

  double zRF=0;
  if (quality>=6 && quality !=7) zRF=zcn_[st-1];
  if ((quality <6 || quality==7) && sl==1) zRF=zsl1_[st-1];
  if ((quality <6 || quality==7) && sl==3) zRF=zsl3_[st-1];

  double x = (tan(phi*0.8/65536.))*(r*cos(deltaphi) - zRF)-r*sin(deltaphi); //zRF is in local coordinates -> z invreases approching to vertex

  if (hasPosRF(wh,sec)){ x = -x; } // change sign in case of positive wheels

  return x;

}


float DTTrigGeomUtils::trigDirCHT(const L1Phase2MuDTPhDigi* trig)
{

  int wh   = trig->whNum();
  int sec  = trig->scNum()+1;
  int phi  = trig->phi();
  int phib = trig->phiBend();
  
  float dir = (phib*1.4/2048.+phi*0.8/65536.)*radToDeg_;

  // change sign in case of negative wheels
  if (!hasPosRF(wh,sec)) { dir = -dir; }
  
  return dir;
  
}

float DTTrigGeomUtils::trigPosAM(const L1Phase2MuDTPhDigi* trig)
{

  int wh   = trig->whNum();
  int sec  = trig->scNum()+1;
  int st   = trig->stNum();
  int phi  = trig->phi();
  int quality = trig->quality();
  int sl = trig->slNum();

  double phin = (sec-1)*Geom::pi()/6;
  double phicenter = 0;
  double r = 0;

  if (sec==4 && st==4) 
    {
      GlobalPoint gpos = phi>0 ? muonGeom_->chamber(DTChamberId(wh,st,13))->position() : muonGeom_->chamber(DTChamberId(wh,st,4))->position();
      phicenter =  gpos.phi();
      r = gpos.perp();
    } 
  else if (sec==10 && st==4) 
    {
      GlobalPoint gpos = phi>0 ? muonGeom_->chamber(DTChamberId(wh,st,14))->position() : muonGeom_->chamber(DTChamberId(wh,st,10))->position();
      phicenter =  gpos.phi();
      r = gpos.perp();
    } 
  else 
    {
      GlobalPoint gpos = muonGeom_->chamber(DTChamberId(wh,st,sec))->position();
      phicenter =  gpos.phi();
      r = gpos.perp();
    }

  double deltaphi = phicenter-phin;

  double zRF=0;
  if (quality>=6 && quality !=7) zRF=zcn_[st-1];
  if ((quality <6 || quality==7) && sl==1) zRF=zsl1_[st-1];
  if ((quality <6 || quality==7) && sl==3) zRF=zsl3_[st-1];

  double x = (tan(phi*0.8/65536.))*(r*cos(deltaphi) - zRF)-r*sin(deltaphi); //zRF is in local coordinates -> z invreases approching to vertex

  if (hasPosRF(wh,sec)){ x = -x; } // change sign in case of positive wheels

  return x;
}

float DTTrigGeomUtils::trigDirAM(const L1Phase2MuDTPhDigi* trig)
{

  int wh   = trig->whNum();
  int sec  = trig->scNum()+1;
  int phi  = trig->phi();
  int phib = trig->phiBend();

  float dir = (phib*1.4/2048.+phi*0.8/65536.)*radToDeg_;
  
  // change sign in case of negative wheels
  if (!hasPosRF(wh,sec)) { dir = -dir; }

  return dir;

}
