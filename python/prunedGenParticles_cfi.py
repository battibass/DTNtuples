import FWCore.ParameterSet.Config as cms

prunedGenParticles = cms.EDProducer("GenParticlePruner",
                                    src = cms.InputTag("genParticles"),
                                    select = cms.vstring("drop *"
                                                         , "++keep pdgId =  13 && pt > 3"
                                                         , "++keep pdgId = -13 && pt > 3"
                                                         )
                                    )

