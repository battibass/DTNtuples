import FWCore.ParameterSet.Config as cms

dtNtupleProducer = cms.EDAnalyzer("DTNtupleProducer",
                                  genPartTag = cms.untracked.InputTag("prunedGenParticles"),
                                  
                                  dtFedBxTag = cms.untracked.InputTag("none"),

                                  puInfoTag = cms.untracked.InputTag("none"),
                                  lumiScalerTag = cms.untracked.InputTag("none"),
                                  primaryVerticesTag = cms.untracked.InputTag("offlineSlimmedPrimaryVertices"),

                                  ph1DtDigiTag = cms.untracked.InputTag("simMuonDTDigis"),
                                  ph2DtDigiTag = cms.untracked.InputTag("simMuonDTDigis"),

                                  ph1TwinMuxInTag = cms.untracked.InputTag("simDtTriggerPrimitiveDigis"),
                                  ph1TwinMuxOutTag = cms.untracked.InputTag("simDtTriggerPrimitiveDigis"),
                                  ph1BmtfInTag = cms.untracked.InputTag("simDtTriggerPrimitiveDigis"),

                                  ph1BmtfOutTag  = cms.untracked.InputTag("simBmtfDigis", "BMTF"),

                                  ph1TwinMuxInThTag = cms.untracked.InputTag("simDtTriggerPrimitiveDigis"),
                                  ph1BmtfInThTag = cms.untracked.InputTag("simDtTriggerPrimitiveDigis"),

                                  ph2TPGPhiHwTag = cms.untracked.InputTag("dtTriggerPhase2AmPrimitiveDigis"),
                                  ph2TPGPhiEmuHbTag = cms.untracked.InputTag("none"),
                                  ph2TPGPhiEmuAmTag = cms.untracked.InputTag("dtTriggerPhase2AmPrimitiveDigis"),

                                  ph2TPGThHwTag  = cms.untracked.InputTag("none"),
                                  ph2TPGThEmuAmTag  = cms.untracked.InputTag("none"),

                                  ph1DtSegmentTag = cms.untracked.InputTag("dt4DSegments"),
                                  ph2DtSegmentTag = cms.untracked.InputTag("dt4DSegments"),

                                  muonTag = cms.untracked.InputTag("slimmedMuons"),

                                  trigEventTag = cms.untracked.InputTag("none"),
                                  trigResultsTag = cms.untracked.InputTag("none"),

                                  trigName = cms.untracked.string('none'),
                                  isoTrigName = cms.untracked.string('none'),

                                  ph1tTrigMode = cms.untracked.string('DTTTrigSyncFromDB'),
                                  ph1tTrigModeConfig = cms.untracked.PSet(vPropWire = cms.double(24.4),
                                                                          doTOFCorrection = cms.bool(False),
                                                                          tofCorrType = cms.int32(2),
                                                                          doWirePropCorrection = cms.bool(False),
                                                                          wirePropCorrType = cms.int32(0),
                                                                          doT0Correction = cms.bool(True),
                                                                          t0Label = cms.string(''),
                                                                          tTrigLabel = cms.string(''),
                                                                          debug = cms.untracked.bool(False),
                                                                      ),

                                  ph2tTrigMode = cms.untracked.string('DTTTrigSyncFromDB'),
                                  ph2tTrigModeConfig = cms.untracked.PSet(vPropWire = cms.double(24.4),
                                                                          doTOFCorrection = cms.bool(False),
                                                                          tofCorrType = cms.int32(2),
                                                                          doWirePropCorrection = cms.bool(False),
                                                                          wirePropCorrType = cms.int32(0),
                                                                          doT0Correction = cms.bool(True),
                                                                          t0Label = cms.string(''),
                                                                          tTrigLabel = cms.string(''),
                                                                          debug = cms.untracked.bool(False),
                                                                      )

)
