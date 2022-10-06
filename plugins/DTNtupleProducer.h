#ifndef DTNtuple_DTNtupleProducer_h
#define DTNtuple_DTNtupleProducer_h

/** \class DTNtupleProducer DTNtupleProducer.h DTDPGAnalysis/DTNtuples/src/DTNtupleProducer.h
 *  
 * Steering class: the edm::EDAnalyzer for DTNtuple prdouction
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBaseFiller.h"
#include "DTDPGAnalysis/DTNtuples/src/DTNtupleConfig.h"

#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"

#include <memory>

class DTNtupleProducer : public edm::one::EDAnalyzer<edm::one::SharedResources,edm::one::WatchRuns>
{
 public:

  /// Constructor
  DTNtupleProducer(const edm::ParameterSet &);
  
  /// Fill ntuples event by event
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

  /// Configure event setup
  virtual void beginRun(const edm::Run&, const edm::EventSetup&) override;

  /// Empty, needed by interface
  virtual void endRun(const edm::Run&, const edm::EventSetup&) override { };

  /// General "una tantum" configuration
  virtual void beginJob() override;

  /// Write ntuples to file
  virtual void endJob() override;
  
 private:
  
  /// Pointer to the TTree
  std::shared_ptr<TTree> m_tree;

  /// Ponter to the configuration 
  std::shared_ptr<DTNtupleConfig> m_config;

  /// The container with all the fillers
  std::vector<std::unique_ptr<DTNtupleBaseFiller>> m_fillers;

};

#endif
