// -*- C++ -*-
//
// Package:    DTDPGAnalysis/DTNtuples
// Class:      DTRandomDigiGenerator
//
/**\class DTRandomDigiGenerator DTRandomDigiGenerator.cc
 DTDPGAnalysis/DTNtuples/plugins/DTRandomDigiGenerator.cc

 Description: Class to add random DT digis on a 
              chamber by chamber basis

*/
//
// Original Author:  Carlo Battilana
//         Created:  Mon, 11 Dec 2019 15:12:51 GMT
//
//

// system include files
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "Geometry/DTGeometry/interface/DTTopology.h"
#include "Geometry/DTGeometry/interface/DTChamber.h"
#include "Geometry/DTGeometry/interface/DTLayer.h"

#include "DataFormats/DTDigi/interface/DTDigiCollection.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/DTLayerId.h"
#include "DataFormats/MuonDetId/interface/DTWireId.h"

#include "CondFormats/DataRecord/interface/MuonSystemAgingRcd.h"
#include "CondFormats/RecoMuonObjects/interface/MuonSystemAging.h"

#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandPoissonQ.h"
#include "CLHEP/Random/RandFlat.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"

//
// class declaration
//

class DTRandomDigiGenerator : public edm::stream::EDProducer<> {
public:
  explicit DTRandomDigiGenerator(const edm::ParameterSet &);
  ~DTRandomDigiGenerator() override;

  static void fillDescriptions(edm::ConfigurationDescriptions &);

private:
  void produce(edm::Event &, const edm::EventSetup &) override;

  void beginRun(edm::Run const &, edm::EventSetup const &) override;

  void createMaskedChamberCollection(edm::ESHandle<DTGeometry> &);

  // ----------member data ---------------------------

  edm::EDGetTokenT<DTDigiCollection> m_digiToken;

  std::map<std::string, double> m_stRateSlopes;

  double m_targetLumi;
  double m_timeWindowMin;
  double m_timeWindowMax;
  double m_deadTime;

  std::map<unsigned int, float> m_chRates;
};

//
// constants, enums and typedefs
//

//
// constructors and destructor
//
DTRandomDigiGenerator::DTRandomDigiGenerator(const edm::ParameterSet &iConfig)
  : m_digiToken(consumes<DTDigiCollection>(iConfig.getParameter<edm::InputTag>("digiTag"))) 
{

  m_stRateSlopes["-2_1"] = iConfig.getParameter<double>("rateSlopeYBm2MB1");
  m_stRateSlopes["-2_2"] = iConfig.getParameter<double>("rateSlopeYBm2MB2");
  m_stRateSlopes["-2_3"] = iConfig.getParameter<double>("rateSlopeYBm2MB3");
  m_stRateSlopes["-2_4"] = iConfig.getParameter<double>("rateSlopeYBm2MB4");
  m_stRateSlopes["-1_1"] = iConfig.getParameter<double>("rateSlopeYBm1MB1");
  m_stRateSlopes["-1_2"] = iConfig.getParameter<double>("rateSlopeYBm1MB2");
  m_stRateSlopes["-1_3"] = iConfig.getParameter<double>("rateSlopeYBm1MB3");
  m_stRateSlopes["-1_4"] = iConfig.getParameter<double>("rateSlopeYBm1MB4");
  m_stRateSlopes["0_1"]  = iConfig.getParameter<double>("rateSlopeYB0MB1");
  m_stRateSlopes["0_2"]  = iConfig.getParameter<double>("rateSlopeYB0MB2");
  m_stRateSlopes["0_3"]  = iConfig.getParameter<double>("rateSlopeYB0MB3");
  m_stRateSlopes["0_4"]  = iConfig.getParameter<double>("rateSlopeYB0MB4");
  m_stRateSlopes["1_1"]  = iConfig.getParameter<double>("rateSlopeYB1MB1");
  m_stRateSlopes["1_2"]  = iConfig.getParameter<double>("rateSlopeYB1MB2");
  m_stRateSlopes["1_3"]  = iConfig.getParameter<double>("rateSlopeYB1MB3");
  m_stRateSlopes["1_4"]  = iConfig.getParameter<double>("rateSlopeYB1MB4");
  m_stRateSlopes["2_1"]  = iConfig.getParameter<double>("rateSlopeYB2MB1");
  m_stRateSlopes["2_2"]  = iConfig.getParameter<double>("rateSlopeYB2MB2");
  m_stRateSlopes["2_3"]  = iConfig.getParameter<double>("rateSlopeYB2MB3");
  m_stRateSlopes["2_4"]  = iConfig.getParameter<double>("rateSlopeYB2MB4");

  m_targetLumi    = iConfig.getParameter<double>("targetLumi");

  m_timeWindowMin = iConfig.getParameter<double>("timeWindowMin");
  m_timeWindowMax = iConfig.getParameter<double>("timeWindowMax");
  m_deadTime      = iConfig.getParameter<double>("deadTime");

  produces<DTDigiCollection>();

}

DTRandomDigiGenerator::~DTRandomDigiGenerator() 
{

}

//
// member functions
//

// ------------ method called to produce the data  ------------
void DTRandomDigiGenerator::produce(edm::Event &event, const edm::EventSetup &conditions) 
{

  edm::Service<edm::RandomNumberGenerator> randGenService;
  CLHEP::HepRandomEngine &randGen = randGenService->getEngine(event.streamID());
  
  std::unique_ptr<DTDigiCollection> bkgDigis(new DTDigiCollection());

  std::map<uint32_t,std::map<int,std::vector<double>>> digisByLayer;  

  if (!m_digiToken.isUninitialized()) 
    {
      
      edm::Handle<DTDigiCollection> dtDigis;
      event.getByToken(m_digiToken, dtDigis);
      
      for (const auto & dtDigiByLayer : (*dtDigis))
	{

	  const auto layRawId = dtDigiByLayer.first.rawId();

	  auto dtDigiIt  = dtDigiByLayer.second.first;
	  auto dtDigiEnd = dtDigiByLayer.second.second;

	  for (; dtDigiIt != dtDigiEnd; ++dtDigiIt)
	    {
	      digisByLayer[layRawId][dtDigiIt->wire()].push_back(dtDigiIt->time());
	    }      

	}

    }
  
  edm::ESHandle<DTGeometry> dtGeometry;
  conditions.get<MuonGeometryRecord>().get(dtGeometry);

  for (const auto layer : dtGeometry->layers())
    {

      auto layId = layer->id();
      auto chId  = layer->chamber()->id();
      auto & digiByWire = digisByLayer[layId.rawId()];

      const auto topology = layer->specificTopology();
      
      auto wireArea  = topology.cellWidth() * topology.cellLenght();
      auto timeRange = (m_timeWindowMax - m_timeWindowMin) * 1E-9;
      auto probPerWire = m_chRates[chId.rawId()] * wireArea * timeRange;
      CLHEP::RandPoissonQ randPoissonQ(randGen, probPerWire);

      // std::cout << layId << std::endl;
      // std::cout << m_chRates[chId.rawId()] << " " << wireArea << " " << probPerWire << std::endl;

      for (int wire = topology.firstChannel(); wire <= topology.lastChannel(); ++wire)
	{

	  std::vector<double> digis;
	  for (const auto & digiTime : digiByWire[wire])
	    digis.push_back(digiTime);

	  // int nCleanDigis = digis.size();
 
	  int nRandDigis = randPoissonQ.fire();

	  for (int iDigi = 0; iDigi < nRandDigis; ++iDigi)
	    digis.push_back(CLHEP::RandFlat::shoot(&randGen,m_timeWindowMin,m_timeWindowMax));
	  
	  std::sort(digis.begin(),digis.end());

	  // std::cout << "wire : " << wire << " " << nCleanDigis << " " << nRandDigis << " " << digis.size() << std::endl;

	  int iDigi = 0;
	  double pDigiTime = -999.;
	  for (const auto & digiTime : digis)
	    {
	      if ((digiTime - pDigiTime) > m_deadTime)
		{
		  bkgDigis->insertDigi(layId, DTDigi(wire,digiTime,iDigi));
		  pDigiTime = digiTime;
		  ++iDigi;
		}
	    }
	      
	}
    }

  event.put(std::move(bkgDigis));

}

// ------------ method called when starting to processes a run  ------------
void DTRandomDigiGenerator::beginRun(edm::Run const &run, edm::EventSetup const &conditions) 
{

  m_chRates.clear();

  edm::ESHandle<DTGeometry> dtGeometry;
  conditions.get<MuonGeometryRecord>().get(dtGeometry);

  for (const auto chamb : dtGeometry->chambers())
    {

      auto chId = chamb->id();
      std::string ringTag = std::to_string(chId.wheel()) + "_" + std::to_string(chId.station());

      m_chRates[chId.rawId()] = m_targetLumi * m_stRateSlopes[ringTag];

      // std::cout << chId << " " << m_chRates[chId.rawId()] << std::endl;
      
    }

}

// ------------ method fills 'descriptions' with the allowed parameters for the
// module  ------------
void DTRandomDigiGenerator::fillDescriptions(edm::ConfigurationDescriptions &descriptions) 
{

  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("digiTag", edm::InputTag("simMuonDTDigis"));
  desc.add<double>("rateSlopeYBm2MB1", 3.5);
  desc.add<double>("rateSlopeYBm2MB2", 0.7);
  desc.add<double>("rateSlopeYBm2MB3", 0.2);
  desc.add<double>("rateSlopeYBm2MB4", 4.0);
  desc.add<double>("rateSlopeYBm1MB1", 1.5);
  desc.add<double>("rateSlopeYBm1MB2", 0.25);
  desc.add<double>("rateSlopeYBm1MB3", 0.1);
  desc.add<double>("rateSlopeYBm1MB4", 3.0);
  desc.add<double>("rateSlopeYB0MB1",  0.5);
  desc.add<double>("rateSlopeYB0MB2",  0.15);
  desc.add<double>("rateSlopeYB0MB3",  0.1);
  desc.add<double>("rateSlopeYB0MB4",  3.0);
  desc.add<double>("rateSlopeYB1MB1",  1.5);
  desc.add<double>("rateSlopeYB1MB2",  0.25);
  desc.add<double>("rateSlopeYB1MB3",  0.1);
  desc.add<double>("rateSlopeYB1MB4",  3.0); 
  desc.add<double>("rateSlopeYB2MB1",  3.5);
  desc.add<double>("rateSlopeYB2MB2",  0.7);
  desc.add<double>("rateSlopeYB2MB3",  0.2);
  desc.add<double>("rateSlopeYB2MB4",  4.0);

  desc.add<double>("targetLumi", 5.);

  desc.add<double>("timeWindowMin", 0.);
  desc.add<double>("timeWindowMax", 1250.);
  desc.add<double>("deadTime", 50.);
  
  descriptions.add("dtRandomDigiGenerator", desc);

}

// define this as a plug-in
DEFINE_FWK_MODULE(DTRandomDigiGenerator);
