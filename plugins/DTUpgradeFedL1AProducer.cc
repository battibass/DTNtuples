// -*- C++ -*-
//
// Package:    DTDPGAnalysis/DTNtuples
// Class:      DTUpgradeFedL1AProducer
//
/**\class DTUpgradeFedL1AProducer DTUpgradeFedL1AProducer.cc
 DTDPGAnalysis/DTNtuples/plugins/DTUpgradeFedL1AProducer.cc

 Description: Class to add random DT digis on a 
              chamber by chamber basis

*/
//
// Original Author:  Carlo Battilana
//         Created:  Thu, 19 Dec 2019 15:12:51 GMT
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

#include <DataFormats/FEDRawData/interface/FEDRawData.h>
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>

//
// class declaration
//

class DTUpgradeFedL1AProducer : public edm::stream::EDProducer<> {
public:
  explicit DTUpgradeFedL1AProducer(const edm::ParameterSet &);
  ~DTUpgradeFedL1AProducer() override;

  static void fillDescriptions(edm::ConfigurationDescriptions &);

private:
  void produce(edm::Event &, const edm::EventSetup &) override;

  void beginRun(edm::Run const &, edm::EventSetup const &) override;

  // ----------member data ---------------------------

  edm::EDGetTokenT<FEDRawDataCollection> m_rawToken;
  int m_fedNumber;

};

//
// constants, enums and typedefs
//

//
// constructors and destructor
//
DTUpgradeFedL1AProducer::DTUpgradeFedL1AProducer(const edm::ParameterSet &iConfig)
  : m_rawToken(consumes<FEDRawDataCollection>(iConfig.getParameter<edm::InputTag>("rawTag"))),
    m_fedNumber(iConfig.getParameter<int>("fedNumber"))
{

  produces<int>();

}

DTUpgradeFedL1AProducer::~DTUpgradeFedL1AProducer() 
{

}

//
// member functions
//

// ------------ method called to produce the data  ------------
void DTUpgradeFedL1AProducer::produce(edm::Event &event, const edm::EventSetup &conditions) 
{

  std::unique_ptr<int> bxL1A = std::make_unique<int>(-999);

  if (!m_rawToken.isUninitialized()) 
    {
      
      edm::Handle<FEDRawDataCollection> rawPayload;
      event.getByToken(m_rawToken, rawPayload);
      
      FEDRawData data = rawPayload->FEDData(m_fedNumber);
      if ( data.size() > 0 ) 
	{

	  unsigned char * fedLinePointer = data.data();
	  long dataWord = *(reinterpret_cast<long*>(fedLinePointer));

	  (*bxL1A) = (dataWord >> 20) & 0xFFF;

	  // std::cout << (*bxL1A) << std::endl;

	}
    }

  event.put(std::move(bxL1A));

}

// ------------ method called when starting to processes a run  ------------
void DTUpgradeFedL1AProducer::beginRun(edm::Run const &run, edm::EventSetup const &conditions) 
{

}

// ------------ method fills 'descriptions' with the allowed parameters for the
// module  ------------
void DTUpgradeFedL1AProducer::fillDescriptions(edm::ConfigurationDescriptions &descriptions) 
{

  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("rawTag", edm::InputTag("rawDataCollector"));
  desc.add<int>("fedNumber", 1368);

  descriptions.add("dtUpgradeFedL1AProducer", desc);

}

// define this as a plug-in
DEFINE_FWK_MODULE(DTUpgradeFedL1AProducer);
