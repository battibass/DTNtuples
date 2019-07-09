/** \class DTNtupleSegmentFiller DTNtupleSegmentFiller.cc DTDPGAnalysis/DTNtuples/src/DTNtupleSegmentFiller.cc
 *  
 * Helper class : the segment filler for Phase-1 / Phase2 segments (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleSegmentFiller.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "TrackingTools/GeomPropagators/interface/StraightLinePlaneCrossing.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"

#include "TClonesArray.h"

DTNtupleSegmentFiller::DTNtupleSegmentFiller(edm::ConsumesCollector && collector,
				       const std::shared_ptr<DTNtupleConfig> config, 
				       std::shared_ptr<TTree> tree, const std::string & label,
				       SegmentTag tag) : 
  DTNtupleBaseFiller(config, tree, label), m_tag(tag), m_nullVecF()
{

  edm::InputTag & iTag = m_tag == SegmentTag::PH1 ?
                                  m_config->m_inputTags["ph1DtSegmentTag"] :
                                  m_config->m_inputTags["ph2DtSegmentTag"];

  if (iTag.label() != "none") m_dtSegmentToken = collector.consumes<DTRecSegment4DCollection>(iTag);

  // m_nullVecF(0) = DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL;

}

DTNtupleSegmentFiller::~DTNtupleSegmentFiller() 
{ 

};

void DTNtupleSegmentFiller::initialize()
{

  m_seg4D_hitsExpPos     = new TClonesArray("TVectorF", 500); //CB what if the limit exceeds 500 ? 
  m_seg4D_hitsExpPosCh   = new TClonesArray("TVectorF", 500);
  m_seg4D_hitsExpWire    = new TClonesArray("TVectorF", 500);

  m_seg2D_phiHits_pos        = new TClonesArray("TVectorF", 500);
  m_seg2D_phiHits_posCh      = new TClonesArray("TVectorF", 500);
  m_seg2D_phiHits_posErr     = new TClonesArray("TVectorF", 500);
  m_seg2D_phiHits_side       = new TClonesArray("TVectorF", 500);
  m_seg2D_phiHits_wire       = new TClonesArray("TVectorF", 500);
  m_seg2D_phiHits_wirePos    = new TClonesArray("TVectorF", 500);
  m_seg2D_phiHits_layer      = new TClonesArray("TVectorF", 500);
  m_seg2D_phiHits_superLayer = new TClonesArray("TVectorF", 500);
  m_seg2D_phiHits_time       = new TClonesArray("TVectorF", 500);
  m_seg2D_phiHits_timeCali   = new TClonesArray("TVectorF", 500);

  m_seg2D_zHits_pos      = new TClonesArray("TVectorF", 500);
  m_seg2D_zHits_posCh    = new TClonesArray("TVectorF", 500);
  m_seg2D_zHits_posErr   = new TClonesArray("TVectorF", 500);
  m_seg2D_zHits_side     = new TClonesArray("TVectorF", 500);
  m_seg2D_zHits_wire     = new TClonesArray("TVectorF", 500);
  m_seg2D_zHits_wirePos  = new TClonesArray("TVectorF", 500);
  m_seg2D_zHits_layer    = new TClonesArray("TVectorF", 500);
  m_seg2D_zHits_time     = new TClonesArray("TVectorF", 500);
  m_seg2D_zHits_timeCali = new TClonesArray("TVectorF", 500);
  
  m_tree->Branch((m_label + "_nSegments").c_str(), &m_nSegments, (m_label + "_nSegments/i").c_str());
  
  m_tree->Branch((m_label + "_wheel").c_str(),   &m_seg4D_wheel);
  m_tree->Branch((m_label + "_sector").c_str(),  &m_seg4D_sector);
  m_tree->Branch((m_label + "_station").c_str(), &m_seg4D_station);

  m_tree->Branch((m_label + "_hasPhi").c_str(), &m_seg4D_hasPhi);
  m_tree->Branch((m_label + "_hasZed").c_str(), &m_seg4D_hasZed);

  m_tree->Branch((m_label + "_posLoc_x").c_str(), &m_seg4D_posLoc_x);
  m_tree->Branch((m_label + "_posLoc_y").c_str(), &m_seg4D_posLoc_y);
  m_tree->Branch((m_label + "_posLoc_z").c_str(), &m_seg4D_posLoc_z);
  m_tree->Branch((m_label + "_dirLoc_x").c_str(), &m_seg4D_dirLoc_x);
  m_tree->Branch((m_label + "_dirLoc_y").c_str(), &m_seg4D_dirLoc_y);
  m_tree->Branch((m_label + "_dirLoc_z").c_str(), &m_seg4D_dirLoc_z);

  m_tree->Branch((m_label + "_posLoc_x_SL1").c_str(), &m_seg4D_posLoc_x_SL1);
  m_tree->Branch((m_label + "_posLoc_x_SL3").c_str(), &m_seg4D_posLoc_x_SL3);
  m_tree->Branch((m_label + "_posLoc_x_midPlane").c_str(), &m_seg4D_posLoc_x_midPlane);

  m_tree->Branch((m_label + "_posGlb_phi").c_str(), &m_seg4D_posGlb_phi);
  m_tree->Branch((m_label + "_posGlb_eta").c_str(), &m_seg4D_posGlb_eta);

  m_tree->Branch((m_label + "_dirGlb_phi").c_str(), &m_seg4D_dirGlb_phi);
  m_tree->Branch((m_label + "_dirGlb_eta").c_str(), &m_seg4D_dirGlb_eta);

  m_tree->Branch((m_label + "_hitsExpPos").c_str(),     &m_seg4D_hitsExpPos,     2048000,0);
  m_tree->Branch((m_label + "_hitsExpPosCh").c_str(),   &m_seg4D_hitsExpPosCh,   2048000,0);
  m_tree->Branch((m_label + "_hitsExpWire").c_str(),    &m_seg4D_hitsExpWire,    2048000,0);

  m_tree->Branch((m_label + "_phi_t0").c_str(), &m_seg2D_phi_t0);
  m_tree->Branch((m_label + "_phi_vDrift").c_str(), &m_seg2D_phi_vDrift);
  m_tree->Branch((m_label + "_phi_normChi2").c_str(), &m_seg2D_phi_normChi2);

  m_tree->Branch((m_label + "_phi_nHits").c_str(),           &m_seg2D_phi_nHits);

  m_tree->Branch((m_label + "_phiHits_pos").c_str(),        &m_seg2D_phiHits_pos,        2048000,0);
  m_tree->Branch((m_label + "_phiHits_posCh").c_str(),      &m_seg2D_phiHits_posCh,      2048000,0);
  m_tree->Branch((m_label + "_phiHits_posErr").c_str(),     &m_seg2D_phiHits_posErr,     2048000,0);
  m_tree->Branch((m_label + "_phiHits_side").c_str(),       &m_seg2D_phiHits_side,       2048000,0);
  m_tree->Branch((m_label + "_phiHits_wire").c_str(),       &m_seg2D_phiHits_wire,       2048000,0);
  m_tree->Branch((m_label + "_phiHits_wirePos").c_str(),    &m_seg2D_phiHits_wirePos,    2048000,0);
  m_tree->Branch((m_label + "_phiHits_layer").c_str(),      &m_seg2D_phiHits_layer,      2048000,0);
  m_tree->Branch((m_label + "_phiHits_superLayer").c_str(), &m_seg2D_phiHits_superLayer, 2048000,0);
  m_tree->Branch((m_label + "_phiHits_time").c_str(),       &m_seg2D_phiHits_time,       2048000,0);
  m_tree->Branch((m_label + "_phiHits_timeCali").c_str(),   &m_seg2D_phiHits_timeCali,   2048000,0);

  m_tree->Branch((m_label + "_z_normChi2").c_str(), &m_seg2D_z_normChi2);

  m_tree->Branch((m_label + "_z_nHits").c_str(),           &m_seg2D_z_nHits);

  m_tree->Branch((m_label + "_zHits_pos").c_str(),      &m_seg2D_zHits_pos,      2048000,0);
  m_tree->Branch((m_label + "_zHits_posCh").c_str(),    &m_seg2D_zHits_posCh,    2048000,0);
  m_tree->Branch((m_label + "_zHits_posErr").c_str(),   &m_seg2D_zHits_posErr,   2048000,0);
  m_tree->Branch((m_label + "_zHits_side").c_str(),     &m_seg2D_zHits_side,     2048000,0);
  m_tree->Branch((m_label + "_zHits_wire").c_str(),     &m_seg2D_zHits_wire,     2048000,0);
  m_tree->Branch((m_label + "_zHits_wirePos").c_str(),  &m_seg2D_zHits_wirePos,  2048000,0);
  m_tree->Branch((m_label + "_zHits_layer").c_str(),    &m_seg2D_zHits_layer,    2048000,0);
  m_tree->Branch((m_label + "_zHits_time").c_str(),     &m_seg2D_zHits_time,     2048000,0);
  m_tree->Branch((m_label + "_zHits_timeCali").c_str(), &m_seg2D_zHits_timeCali, 2048000,0);

}

void DTNtupleSegmentFiller::clear()
{

  m_nSegments = 0;

  m_seg4D_wheel.clear();
  m_seg4D_sector.clear();
  m_seg4D_station.clear();
  
  m_seg4D_hasPhi.clear();
  m_seg4D_hasZed.clear();   
                     
  m_seg4D_posLoc_x.clear(); 
  m_seg4D_posLoc_y.clear(); 
  m_seg4D_posLoc_z.clear(); 
  m_seg4D_dirLoc_x.clear(); 
  m_seg4D_dirLoc_y.clear(); 
  m_seg4D_dirLoc_z.clear(); 
                     
  m_seg4D_posLoc_x_SL1.clear(); 
  m_seg4D_posLoc_x_SL3.clear();
  m_seg4D_posLoc_x_midPlane.clear(); 

  m_seg4D_posGlb_phi.clear();
  m_seg4D_posGlb_eta.clear();
  m_seg4D_dirGlb_phi.clear();
  m_seg4D_dirGlb_eta.clear();

  m_seg4D_hitsExpPos->Clear();
  m_seg4D_hitsExpPosCh->Clear();
  m_seg4D_hitsExpWire->Clear();

  m_seg2D_phi_t0.clear();
  m_seg2D_phi_vDrift.clear();
  m_seg2D_phi_normChi2.clear();

  m_seg2D_phi_nHits.clear();

  m_seg2D_phiHits_pos->Clear();
  m_seg2D_phiHits_posCh->Clear();
  m_seg2D_phiHits_posErr->Clear();
  m_seg2D_phiHits_side->Clear();
  m_seg2D_phiHits_wire->Clear();
  m_seg2D_phiHits_wirePos->Clear();
  m_seg2D_phiHits_layer->Clear();
  m_seg2D_phiHits_superLayer->Clear();
  m_seg2D_phiHits_time->Clear();
  m_seg2D_phiHits_timeCali->Clear();

  m_seg2D_z_normChi2.clear();

  m_seg2D_z_nHits.clear();

  m_seg2D_zHits_pos->Clear();
  m_seg2D_zHits_posCh->Clear();
  m_seg2D_zHits_posErr->Clear();
  m_seg2D_zHits_side->Clear();
  m_seg2D_zHits_wire->Clear();
  m_seg2D_zHits_wirePos->Clear();
  m_seg2D_zHits_layer->Clear();
  m_seg2D_zHits_time->Clear();
  m_seg2D_zHits_timeCali->Clear();

}

void DTNtupleSegmentFiller::fill(const edm::Event & ev)
{

  clear();

  auto segments4D = conditionalGet<DTRecSegment4DCollection>(ev, m_dtSegmentToken,"DTRecSegment4DCollection");

  if (segments4D.isValid()) 
    {
      
      auto chambIt  = segments4D->id_begin();
      auto chambEnd = segments4D->id_end();
      
      for (; chambIt != chambEnd; ++chambIt)
	{
	  
	  const auto range = segments4D->get(*chambIt);
	  
	  auto segment4D    = range.first;  // CB check naming
	  auto segment4DEnd = range.second;
	  
	  for (; segment4D != segment4DEnd; ++segment4D)
	    {
	      
	      auto wheel   = (*chambIt).wheel();
	      auto sector  = (*chambIt).sector();
	      auto station = (*chambIt).station();
	      
	      m_seg4D_wheel.push_back(wheel);
	      m_seg4D_sector.push_back(sector);
	      m_seg4D_station.push_back(station);
	      
	      bool hasPhi = segment4D->hasPhi();
	      bool hasZed = segment4D->hasZed();
	      
	      m_seg4D_hasPhi.push_back(hasPhi);
	      m_seg4D_hasZed.push_back(hasZed);
	      
	      auto pos = segment4D->localPosition();
	      auto dir = segment4D->localDirection();
	      
	      m_seg4D_posLoc_x.push_back(pos.x());
	      m_seg4D_posLoc_y.push_back(pos.y());
	      m_seg4D_posLoc_z.push_back(pos.z());
	      
	      m_seg4D_dirLoc_x.push_back(dir.x());
	      m_seg4D_dirLoc_y.push_back(dir.y());
	      m_seg4D_dirLoc_z.push_back(dir.z());
	      
	      float xPosLocSL[2] = { DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL,
				     DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL };
	      bool hasPptSL[2] = { false, false };
	      float xPosLocMidPlane = DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL;

	      if (hasPhi || hasZed)
		{
		  TVectorF hitExpectedPos(12);
		  TVectorF hitExpectedPosCh(12);
		  TVectorF hitExpectedWire(12);
		  
		  std::vector<DTWireId> wireVector;
		  
		  for (int iSL = 1; iSL <= DTChamberId::maxSuperLayerId; ++iSL)
		    {
		      if (station == 4 && iSL == 2) continue;
		      
		      for (int iL = 1; iL <= DTChamberId::maxLayerId ; ++iL)
			{
			  wireVector.push_back(DTWireId(wheel,station,sector,iSL,iL,2));
			}
		    }
		
		  int iWire=0;
		  const auto * chamb = m_config->m_dtGeometry->chamber(*chambIt);
 
		  StraightLinePlaneCrossing segmentPlaneCrossing(chamb->toGlobal(pos).basicVector(),
								 chamb->toGlobal(dir).basicVector(),
								 anyDirection);

		  if (hasPhi)
		    {

		      for (int iSL = 0 ; iSL <= 1; ++iSL)
			{
			  const auto * sl = chamb->superLayer(1 + 2 * iSL);
		      
			  auto pptSL = segmentPlaneCrossing.position(sl->surface());
			  hasPptSL[iSL] = pptSL.first;
		      
			  if (hasPptSL[iSL])
			    {
			      GlobalPoint segExrapolationToSL(pptSL.second);
			      LocalPoint  segPosAtSLChamber = chamb->toLocal(segExrapolationToSL);
			      xPosLocSL[iSL] = segPosAtSLChamber.x();
			    }
			}
		      
		      if (hasPptSL[0] && hasPptSL[1])
			{
			  xPosLocMidPlane = (xPosLocSL[0] + xPosLocSL[1]) * 0.5;
			}
		    }
 
		  for(const auto & wire : wireVector)
		    {
		      auto * layer = m_config->m_dtGeometry->layer(wire);
		      
		      auto ppt = segmentPlaneCrossing.position(layer->surface());
		      
		      bool success = ppt.first; // check for failure
		      
		      float theExpPos = DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
		      float theExpPosCh = DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
		      int theExpWire = DTNtupleBaseFiller::DEFAULT_INT_VAL_POS;
		       
		      if (success)
			{ 
			  GlobalPoint segExrapolationToLayer(ppt.second);
			  LocalPoint  segPosAtZWireLayer   = layer->toLocal(segExrapolationToLayer); 
			  LocalPoint  segPosAtZWireChamber = chamb->toLocal(segExrapolationToLayer); 
			  

			  if ((iWire < 4 || iWire > 7) && hasPhi)
			    {
			      theExpPos   = segPosAtZWireLayer.x();
			      theExpPosCh = segPosAtZWireChamber.x();
			      theExpWire = layer->specificTopology().channel(segPosAtZWireLayer);
			    }
			  else if ((iWire >= 4 && iWire<=7) && hasZed)
			    {
			      theExpPos   = segPosAtZWireLayer.x();
			      theExpPosCh = segPosAtZWireChamber.y();
			      theExpWire = layer->specificTopology().channel(segPosAtZWireLayer);
			    }
			  
			}
		      
		      hitExpectedPos[iWire] = theExpPos;
		      hitExpectedPosCh[iWire] = theExpPosCh;
		      hitExpectedWire[iWire] = theExpWire;
		      
		      ++iWire;
		      
		      if (station == 4 && iWire == 4) 
			iWire += 4;
		      
		    }

		  new ((*m_seg4D_hitsExpPos)[m_nSegments])     TVectorF(hitExpectedPos);
		  new ((*m_seg4D_hitsExpPosCh)[m_nSegments])   TVectorF(hitExpectedPosCh);
		  new ((*m_seg4D_hitsExpWire)[m_nSegments])    TVectorF(hitExpectedWire);
		}
	      else 
		{
		  new ((*m_seg4D_hitsExpPos)[m_nSegments])     TVectorF(m_nullVecF);
		  new ((*m_seg4D_hitsExpPosCh)[m_nSegments])   TVectorF(m_nullVecF);
		  new ((*m_seg4D_hitsExpWire)[m_nSegments])    TVectorF(m_nullVecF);
		}
	    
	      m_seg4D_posLoc_x_SL1.push_back(xPosLocSL[0]);
	      m_seg4D_posLoc_x_SL3.push_back(xPosLocSL[1]);
	      m_seg4D_posLoc_x_midPlane.push_back(xPosLocMidPlane);

	      const GeomDet * geomDet = m_config->m_trackingGeometry->idToDet(segment4D->geographicalId());
	      auto posGlb = geomDet->toGlobal(pos);
	      auto dirGlb = geomDet->toGlobal(dir); // CB do values have sense?
	      
	      m_seg4D_posGlb_phi.push_back(posGlb.phi());
	      m_seg4D_posGlb_eta.push_back(posGlb.eta());

	      m_seg4D_dirGlb_phi.push_back(dirGlb.phi());
	      m_seg4D_dirGlb_eta.push_back(dirGlb.eta());
	      
	      if(hasPhi) 
		fillPhi(segment4D->phiSegment(), geomDet);
	      else
		fillPhiEmpty();
	
	      if(hasZed) 
		fillZ(segment4D->zSegment(), geomDet);
	      else
		fillZEmpty();

	      ++m_nSegments;
	    }
	}

    }
  
  return;

}

void DTNtupleSegmentFiller::fillPhi(const DTChamberRecSegment2D* phiSeg, const GeomDet* chamb)
{

  auto recHits = phiSeg->specificRecHits();

  //segment information
  m_seg2D_phi_t0.push_back(phiSeg->t0());
  m_seg2D_phi_vDrift.push_back(phiSeg->vDrift());
  m_seg2D_phi_normChi2.push_back(phiSeg->chi2() / phiSeg->degreesOfFreedom());
  
  //rechits information
  const int nRecHits = recHits.size();
  m_seg2D_phi_nHits.push_back(nRecHits);

  TVectorF posRechits(nRecHits);
  TVectorF posChRechits(nRecHits);
  TVectorF posErrRechits(nRecHits);
  TVectorF sideRechits(nRecHits);
  TVectorF wireRechits(nRecHits);
  TVectorF wirePosRechits(nRecHits);
  TVectorF layerRechits(nRecHits);
  TVectorF superLayerRechits(nRecHits);
  TVectorF timeRechits(nRecHits);
  TVectorF timeCaliRechits(nRecHits);

  int iRecHit = 0;

  for(const auto & recHit : recHits)
    {

      const auto wireId  = recHit.wireId();
      const auto layerId = wireId.layerId();

      const auto * layer = m_config->m_dtGeometry->layer(layerId);

      posRechits(iRecHit)    = recHit.localPosition().x();
      posChRechits(iRecHit)  = chamb->toLocal(layer->toGlobal(recHit.localPosition())).x();
      posErrRechits(iRecHit) = recHit.localPositionError().xx();

      sideRechits(iRecHit)       = recHit.lrSide();
      wireRechits(iRecHit)       = wireId.wire();
      wirePosRechits(iRecHit)    = layer->specificTopology().wirePosition(wireId.wire());
      layerRechits(iRecHit)      = layerId.layer();
      superLayerRechits(iRecHit) = layerId.superlayer();

      float digiTime = recHit.digiTime();
      float tTrig = m_config->m_dtSync->offset(wireId);

      timeRechits(iRecHit)  = digiTime;
      timeCaliRechits(iRecHit)  = digiTime - tTrig;

      iRecHit++;
    }

  new ((*m_seg2D_phiHits_pos)[m_nSegments])        TVectorF(posRechits);
  new ((*m_seg2D_phiHits_posCh)[m_nSegments])      TVectorF(posChRechits);
  new ((*m_seg2D_phiHits_posErr)[m_nSegments])     TVectorF(posErrRechits);
  new ((*m_seg2D_phiHits_side)[m_nSegments])       TVectorF(sideRechits);
  new ((*m_seg2D_phiHits_wire)[m_nSegments])       TVectorF(wireRechits);
  new ((*m_seg2D_phiHits_wirePos)[m_nSegments])    TVectorF(wirePosRechits);
  new ((*m_seg2D_phiHits_layer)[m_nSegments])      TVectorF(layerRechits);
  new ((*m_seg2D_phiHits_superLayer)[m_nSegments]) TVectorF(superLayerRechits);
  new ((*m_seg2D_phiHits_time)[m_nSegments])       TVectorF(timeRechits);
  new ((*m_seg2D_phiHits_timeCali)[m_nSegments])   TVectorF(timeCaliRechits);

  return;
  
}

void DTNtupleSegmentFiller::fillZ(const DTSLRecSegment2D* zSeg, const GeomDet* chamb)
{

  auto recHits = zSeg->specificRecHits();

  //segment information
  m_seg2D_z_normChi2.push_back(zSeg->chi2() / zSeg->degreesOfFreedom());
  
  //rechits information
  const int nRecHits = recHits.size();
  m_seg2D_z_nHits.push_back(nRecHits);

  TVectorF posRechits(nRecHits);
  TVectorF posChRechits(nRecHits);
  TVectorF posErrRechits(nRecHits);
  TVectorF sideRechits(nRecHits);
  TVectorF wireRechits(nRecHits);
  TVectorF wirePosRechits(nRecHits);
  TVectorF layerRechits(nRecHits);
  TVectorF timeRechits(nRecHits);
  TVectorF timeCaliRechits(nRecHits);

  int iRecHit = 0;

  for(const auto & recHit : recHits)
    {

      const auto wireId  = recHit.wireId();
      const auto layerId = wireId.layerId();

      const auto * layer = m_config->m_dtGeometry->layer(layerId);

      posRechits(iRecHit)    = recHit.localPosition().x();
      posChRechits(iRecHit)  = chamb->toLocal(layer->toGlobal(recHit.localPosition())).y();
      posErrRechits(iRecHit) = recHit.localPositionError().xx();

      sideRechits(iRecHit)       = recHit.lrSide();
      wireRechits(iRecHit)       = wireId.wire();
      wirePosRechits(iRecHit)    = layer->specificTopology().wirePosition(wireId.wire());
      layerRechits(iRecHit)      = layerId.layer();

      float digiTime = recHit.digiTime();
      float tTrig = m_config->m_dtSync->offset(wireId);

      timeRechits(iRecHit)  = digiTime;
      timeCaliRechits(iRecHit)  = digiTime - tTrig;

      iRecHit++;
    }

  new ((*m_seg2D_zHits_pos)[m_nSegments])        TVectorF(posRechits);
  new ((*m_seg2D_zHits_posCh)[m_nSegments])      TVectorF(posChRechits);
  new ((*m_seg2D_zHits_posErr)[m_nSegments])     TVectorF(posErrRechits);
  new ((*m_seg2D_zHits_side)[m_nSegments])       TVectorF(sideRechits);
  new ((*m_seg2D_zHits_wire)[m_nSegments])       TVectorF(wireRechits);
  new ((*m_seg2D_zHits_wirePos)[m_nSegments])    TVectorF(wirePosRechits);
  new ((*m_seg2D_zHits_layer)[m_nSegments])      TVectorF(layerRechits);
  new ((*m_seg2D_zHits_time)[m_nSegments])       TVectorF(timeRechits);
  new ((*m_seg2D_zHits_timeCali)[m_nSegments])   TVectorF(timeCaliRechits);

  return;  

}

void DTNtupleSegmentFiller::fillPhiEmpty()
{
		
  m_seg2D_phi_nHits.push_back(0);
  m_seg2D_phi_t0.push_back(DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL);
  m_seg2D_phi_vDrift.push_back(DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL);
  m_seg2D_phi_normChi2.push_back(DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL_POS);

  new ((*m_seg2D_phiHits_pos)[m_nSegments])        TVectorF(m_nullVecF);
  new ((*m_seg2D_phiHits_posCh)[m_nSegments])      TVectorF(m_nullVecF);
  new ((*m_seg2D_phiHits_posErr)[m_nSegments])     TVectorF(m_nullVecF);
  new ((*m_seg2D_phiHits_side)[m_nSegments])       TVectorF(m_nullVecF);
  new ((*m_seg2D_phiHits_wire)[m_nSegments])       TVectorF(m_nullVecF);
  new ((*m_seg2D_phiHits_wirePos)[m_nSegments])    TVectorF(m_nullVecF);
  new ((*m_seg2D_phiHits_layer)[m_nSegments])      TVectorF(m_nullVecF);
  new ((*m_seg2D_phiHits_superLayer)[m_nSegments]) TVectorF(m_nullVecF);
  new ((*m_seg2D_phiHits_time)[m_nSegments])       TVectorF(m_nullVecF);
  new ((*m_seg2D_phiHits_timeCali)[m_nSegments])   TVectorF(m_nullVecF);

}

void DTNtupleSegmentFiller::fillZEmpty()
{
		
  m_seg2D_z_nHits.push_back(0);
  m_seg2D_z_normChi2.push_back(DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL_POS);

  new ((*m_seg2D_zHits_pos)[m_nSegments])        TVectorF(m_nullVecF);
  new ((*m_seg2D_zHits_posCh)[m_nSegments])      TVectorF(m_nullVecF);
  new ((*m_seg2D_zHits_posErr)[m_nSegments])     TVectorF(m_nullVecF);
  new ((*m_seg2D_zHits_side)[m_nSegments])       TVectorF(m_nullVecF);
  new ((*m_seg2D_zHits_wire)[m_nSegments])       TVectorF(m_nullVecF);
  new ((*m_seg2D_zHits_wirePos)[m_nSegments])    TVectorF(m_nullVecF);
  new ((*m_seg2D_zHits_layer)[m_nSegments])      TVectorF(m_nullVecF);
  new ((*m_seg2D_zHits_time)[m_nSegments])       TVectorF(m_nullVecF);
  new ((*m_seg2D_zHits_timeCali)[m_nSegments])   TVectorF(m_nullVecF);

}
