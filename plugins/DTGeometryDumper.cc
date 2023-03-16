// -*- C++ -*-
//
// Package:    DTDPGAnalysis/DTNtuples
// Class:      DTGeometryDumper
//
/**\class DTGeometryDumper DTGeometryDumper.cc
 DTDPGAnalysis/DTNtuples/plugins/DTGeometryDumper.cc

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

class DTGeometryDumper : public edm::stream::EDProducer<> {
public:
  explicit DTGeometryDumper(const edm::ParameterSet &);
  ~DTGeometryDumper() override;

  static void fillDescriptions(edm::ConfigurationDescriptions &);

private:
  void produce(edm::Event &, const edm::EventSetup &) override;

  void beginRun(edm::Run const &, edm::EventSetup const &) override;

  // ----------member data ---------------------------

  std::vector<std::string> m_geomTags;
  using GeomToken = edm::ESGetToken<DTGeometry, MuonGeometryRecord>;
  std::map<std::string, GeomToken> m_dtGeomTokens;
};

//
// constants, enums and typedefs
//

//
// constructors and destructor
//
DTGeometryDumper::DTGeometryDumper(const edm::ParameterSet &iConfig)
    : m_geomTags(iConfig.getParameter<std::vector<std::string>>("geomTags")) {
  for (const auto &geomTag : m_geomTags) {
    m_dtGeomTokens[geomTag] = consumesCollector().esConsumes<edm::Transition::BeginRun>(edm::ESInputTag("", geomTag));
  }
}

DTGeometryDumper::~DTGeometryDumper() {}

//
// member functions
//

// ------------ method called to produce the data  ------------
void DTGeometryDumper::produce(edm::Event &event, const edm::EventSetup &conditions) {}

// ------------ method called when starting to processes a run  ------------
void DTGeometryDumper::beginRun(edm::Run const &run, edm::EventSetup const &conditions) {
  std::map<std::string, edm::ESHandle<DTGeometry>> dtGeometries;

  for (const auto &geomTokenPair : m_dtGeomTokens) {
    dtGeometries[geomTokenPair.first] = conditions.getHandle(geomTokenPair.second);
  }

  for (const auto &dtTagGeomPair : dtGeometries) {
    std::cout << "Geometry tag : " << dtTagGeomPair.first << " **********" << std::endl;

    for (const auto chamb : dtTagGeomPair.second->chambers()) {
      auto chId = chamb->id();
      if (chId.sector() == 4) {
        auto globPos = chamb->position();
        std::cout << chId << " x: " << globPos.x() << " y: " << globPos.y() << " z: " << globPos.z() << std::endl;
      }
    }
  }
}

// ------------ method fills 'descriptions' with the allowed parameters for the
// module  ------------
void DTGeometryDumper::fillDescriptions(edm::ConfigurationDescriptions &descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<std::vector<std::string>>("geomTags", {"", "idealForDigi"});

  descriptions.add("dtGeometryDumper", desc);
}

// define this as a plug-in
DEFINE_FWK_MODULE(DTGeometryDumper);
