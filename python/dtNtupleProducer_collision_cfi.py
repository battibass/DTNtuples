import FWCore.ParameterSet.Config as cms

dtNtupleProducer = cms.EDAnalyzer("DTNtupleProducer",
                                  genPartTag = cms.untracked.InputTag("none"),

                                  dtFedBxTag = cms.untracked.InputTag("none"),

                                  puInfoTag          = cms.untracked.InputTag("addPileupInfo"),
                                  lumiScalerTag      = cms.untracked.InputTag("onlineMetaDataDigis"),   # 2018-2022
                                  lumiScalerTag2017  = cms.untracked.InputTag("scalersRawToDigi"),      # 2017
                                  primaryVerticesTag = cms.untracked.InputTag("offlinePrimaryVertices"),

                                  ph1DtDigiTag = cms.untracked.InputTag("muonDTDigis"),
                                  ph2DtDigiTag = cms.untracked.InputTag("none"),

                                  ph1TwinMuxInTag  = cms.untracked.InputTag("twinMuxStage2Digis","PhIn"),
                                  ph1TwinMuxOutTag = cms.untracked.InputTag("twinMuxStage2Digis","PhOut"),
                                  ph1BmtfInTag  = cms.untracked.InputTag("bmtfDigis"),

                                  ph1BmtfOutTag  = cms.untracked.InputTag("bmtfDigis", "BMTF"),

                                  ph1TwinMuxInThTag = cms.untracked.InputTag("twinMuxStage2Digis","ThIn"),
                                  ph1BmtfInThTag  = cms.untracked.InputTag("bmtfDigis"),

                                  ph2TPGPhiHwTag  = cms.untracked.InputTag("none"),
                                  ph2TPGPhiEmuHbTag  = cms.untracked.InputTag("none"),
                                  ph2TPGPhiEmuAmTag  = cms.untracked.InputTag("none"),

                                  ph1DtSegmentTag = cms.untracked.InputTag("dt4DSegments"),        
                                  ph2DtSegmentTag = cms.untracked.InputTag("none"),

                                  muonTag = cms.untracked.InputTag("none"),

                                  trigEventTag = cms.untracked.InputTag("none"),
                                  trigResultsTag = cms.untracked.InputTag("none"),

                                  trigName = cms.untracked.string('none'),
                                  isoTrigName = cms.untracked.string('none'),

                                  ph1tTrigMode = cms.untracked.string('DTTTrigSyncFromDB'),
                                  ph1tTrigModeConfig = cms.untracked.PSet(vPropWire = cms.double(24.4),
                                                                          doTOFCorrection = cms.bool(False),
                                                                          tofCorrType = cms.int32(0),
                                                                          doWirePropCorrection = cms.bool(False),
                                                                          wirePropCorrType = cms.int32(0),
                                                                          doT0Correction = cms.bool(True),
                                                                          t0Label = cms.string(''),
                                                                          tTrigLabel = cms.string(''),
                                                                          debug = cms.untracked.bool(False)
                                                                         ),

                                  ph2tTrigMode = cms.untracked.string('DTTTrigSyncFromDB'),
                                  ph2tTrigModeConfig = cms.untracked.PSet(vPropWire = cms.double(24.4),
                                                                          doTOFCorrection = cms.bool(False),
                                                                          tofCorrType = cms.int32(0),
                                                                          doWirePropCorrection = cms.bool(False),
                                                                          wirePropCorrType = cms.int32(0),
                                                                          doT0Correction = cms.bool(True),
                                                                          t0Label = cms.string('ph2'),
                                                                          tTrigLabel = cms.string(''),
                                                                          debug = cms.untracked.bool(False),
                                                                         )

)

