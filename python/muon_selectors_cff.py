import FWCore.ParameterSet.Config as cms

##### The selector for the DTNtupleMuonFiller input ##################################

muonsForNtuples = cms.EDFilter("MuonSelector",
                               src = cms.InputTag("muons"),
                               cut = cms.string('abs(eta) < 1.4 || (pt > 20 && abs(eta) > 1.4 && abs(eta) < 2.4)'),
                               filter = cms.bool(True)                                
                              )

##### The selector for the SingleMuon worklow (e.g. Express and Expres Cosmics) #####

cosmicMuons = cms.EDFilter("MuonSelector",
                           src = cms.InputTag("muons"),
                           cut = cms.string('abs(eta) < 1.4 && isStandAlone)'),
                           filter = cms.bool(True)                                
                          )

promptMuons = cms.EDFilter("MuonSelector",
                           src = cms.InputTag("muons"),
                           cut = cms.string('abs(eta) < 1.4 && innerTrack().hitPattern().numberOfValidPixelHits() > 0 && \
                                             innerTrack().hitPattern().trackerLayersWithMeasurement() > 5 && (isolationR03().sumPt / pt) < 0.1)'),
                           filter = cms.bool(True)                                
                          )

##### The selector for the DT ZMu skim ##############################################

looseMuonsForSkim = muonsForNtuples.clone()

tightMuonsForSkim = cms.EDFilter("MuonSelector",
                                 src = cms.InputTag("looseMuonsForSkim"),
                                 cut = cms.string('(selectors & 3) && ((isolationR03.sumPt)/(pt) < 0.05) && pt > 25.0 && abs(eta) < 2.4'),
                                 filter = cms.bool(True)                                
                                )

dimuonsForSkim = cms.EDProducer("CandViewShallowCloneCombiner",
                                checkCharge = cms.bool(False),
                                cut = cms.string('mass > 60 &&  (charge=0) && (abs(daughter(0).vz - daughter(1).vz) < 0.1)'),
                                decay = cms.string("tightMuonsForSkim looseMuonsForSkim")
                               )

dimuonsFilterForSkim = cms.EDFilter("CandViewCountFilter",
                                    src = cms.InputTag("dimuonsForSkim"),
                                    minNumber = cms.uint32(1)
                                   )

zMuSkimSeq = cms.Sequence(looseMuonsForSkim
                          * tightMuonsForSkim 
                          * dimuonsForSkim 
                          * dimuonsFilterForSkim
                         )
