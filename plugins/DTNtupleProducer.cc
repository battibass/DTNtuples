/** \class DTNtupleProducer DTNtupleProducer.cc DTDPGAnalysis/DTNtuples/src/DTNtupleProducer.cc
 *  
 * Steering class: the edm::EDAnalyzer for DTNtuple prdouction
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/plugins/DTNtupleProducer.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include "FWCore/Framework/interface/Event.h" 
#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleEventFiller.h"

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleGenFiller.h"
#include "DTDPGAnalysis/DTNtuples/src/DTNtupleMuonFiller.h"
#include "DTDPGAnalysis/DTNtuples/src/DTNtupleDigiFiller.h"
#include "DTDPGAnalysis/DTNtuples/src/DTNtupleSegmentFiller.h"
#include "DTDPGAnalysis/DTNtuples/src/DTNtupleTPGPhiFiller.h"
#include "DTDPGAnalysis/DTNtuples/src/DTNtupleTPGThetaFiller.h"
#include "DTDPGAnalysis/DTNtuples/src/DTNtuplePh2TPGPhiFiller.h"
#include "DTDPGAnalysis/DTNtuples/src/DTNtupleEnvironmentFiller.h"
#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBmtfFiller.h"

#include <iostream>

DTNtupleProducer::DTNtupleProducer( const edm::ParameterSet & config )
{

  usesResource("TFileService");
  edm::Service<TFileService> fileService;
  m_tree = std::shared_ptr<TTree>(fileService->make<TTree>("DTTREE","DT Tree"));

  m_config = std::make_shared<DTNtupleConfig>(DTNtupleConfig(config,consumesCollector()));

  auto pushToF = [this]<typename T>(T&& filler) { m_fillers.push_back(std::make_unique<T>(filler)); };

  pushToF(DTNtupleGenFiller(consumesCollector(), m_config, m_tree, "gen"));

  pushToF(DTNtupleEventFiller(consumesCollector(), m_config, m_tree, "event"));  
  pushToF(DTNtupleEnvironmentFiller(consumesCollector(), m_config, m_tree, "environment"));

  pushToF(DTNtupleDigiFiller(consumesCollector(), m_config, m_tree, "digi",    DTNtupleDigiFiller::DigiTag::PH1));
  pushToF(DTNtupleDigiFiller(consumesCollector(), m_config, m_tree, "ph2Digi", DTNtupleDigiFiller::DigiTag::PH2));

  pushToF(DTNtupleSegmentFiller(consumesCollector(), m_config, m_tree, "seg",    DTNtupleSegmentFiller::SegmentTag::PH1));
  pushToF(DTNtupleSegmentFiller(consumesCollector(), m_config, m_tree, "ph2Seg", DTNtupleSegmentFiller::SegmentTag::PH2));

  // m_fillers.push_back(std::make_unique<DTNtupleMuonFiller>(consumesCollector(), m_config, m_tree, "mu"));

  pushToF(DTNtupleTPGPhiFiller(consumesCollector(), m_config, m_tree, "ltTwinMuxIn",  DTNtupleTPGPhiFiller::TriggerTag::TM_IN));
  pushToF(DTNtupleTPGPhiFiller(consumesCollector(), m_config, m_tree, "ltTwinMuxOut", DTNtupleTPGPhiFiller::TriggerTag::TM_OUT));
  pushToF(DTNtupleTPGPhiFiller(consumesCollector(), m_config, m_tree, "ltBmtfIn",     DTNtupleTPGPhiFiller::TriggerTag::BMTF_IN));

  pushToF(DTNtupleTPGThetaFiller(consumesCollector(), m_config, m_tree, "ltTwinMuxInTh",  DTNtupleTPGThetaFiller::TriggerTag::TM_IN));
  pushToF(DTNtupleTPGThetaFiller(consumesCollector(), m_config, m_tree, "ltBmtfInTh",     DTNtupleTPGThetaFiller::TriggerTag::BMTF_IN));

  pushToF(DTNtuplePh2TPGPhiFiller(consumesCollector(), m_config, m_tree, "ph2TpgPhiHw",    DTNtuplePh2TPGPhiFiller::TriggerTag::HW));
  pushToF(DTNtuplePh2TPGPhiFiller(consumesCollector(), m_config, m_tree, "ph2TpgPhiEmuHb", DTNtuplePh2TPGPhiFiller::TriggerTag::HB));
  pushToF(DTNtuplePh2TPGPhiFiller(consumesCollector(), m_config, m_tree, "ph2TpgPhiEmuAm", DTNtuplePh2TPGPhiFiller::TriggerTag::AM));

  pushToF(DTNtupleBmtfFiller(consumesCollector(), m_config, m_tree, "tfBmtfOut"));

}


void DTNtupleProducer::beginJob() 
{

  for (const auto & filler : m_fillers) 
    {
      filler->initialize();
      filler->clear();
    }

}

void DTNtupleProducer::beginRun(const edm::Run & run, const edm::EventSetup & environment )
{

  m_config->getES(run, environment);

}

void DTNtupleProducer::endJob() 
{

  m_tree->GetCurrentFile()->Write();

}

void DTNtupleProducer::analyze(const edm::Event & ev, const edm::EventSetup & environment )
{
 
  m_config->getES(environment);

  for (const auto & filler : m_fillers) 
    {
      filler->fill(ev);
    }

 m_tree->Fill();

}

// define this as a plug-in
DEFINE_FWK_MODULE(DTNtupleProducer);
