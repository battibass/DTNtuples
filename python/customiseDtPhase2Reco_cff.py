import FWCore.ParameterSet.Config as cms

def customiseForPhase2Reco(process, pathName, tTrigFile, t0File) :

    process.dt1DRecHitsPh2 = process.dt1DRecHits.clone()
    process.dt1DRecHitsPh2.dtDigiLabel = cms.InputTag("dtAB7unpacker")

    process.dt4DSegmentsPh2 = process.dt4DSegments.clone()
    process.dt4DSegmentsPh2.recHits1DLabel = cms.InputTag("dt1DRecHitsPh2")

    process.dt4DSegmentsT0SegPh2 = process.dt4DSegmentsT0Seg.clone()
    process.dt4DSegmentsT0SegPh2.recHits4DLabel = cms.InputTag("dt4DSegmentsPh2")

    process.dtlocalrecoT0SegPh2 = cms.Sequence( process.dt1DRecHitsPh2
                                                + process.dt4DSegmentsPh2
                                                + process.dt4DSegmentsT0SegPh2 )



    if tTrigFile != '' :
        process.dt1DRecHitsPh2.recAlgoConfig.tTrigModeConfig.tTrigLabel  = cms.string('cosmics_ph2')
        process.dt4DSegmentsPh2.Reco4DAlgoConfig.Reco2DAlgoConfig.recAlgoConfig.tTrigModeConfig.tTrigLabel  = cms.string('cosmics_ph2')
        process.dt4DSegmentsPh2.Reco4DAlgoConfig.recAlgoConfig.tTrigModeConfig.tTrigLabel  = cms.string('cosmics_ph2')
        process.dt4DSegmentsT0SegPh2.recAlgoConfig.tTrigModeConfig.tTrigLabel  = cms.string('cosmics_ph2')

    if t0File != '' :
        process.dt1DRecHitsPh2.recAlgoConfig.tTrigModeConfig.t0Label  = cms.string('ph2')
        process.dt4DSegmentsPh2.Reco4DAlgoConfig.Reco2DAlgoConfig.recAlgoConfig.tTrigModeConfig.t0Label  = cms.string('ph2')
        process.dt4DSegmentsPh2.Reco4DAlgoConfig.recAlgoConfig.tTrigModeConfig.t0Label  = cms.string('ph2')
        process.dt4DSegmentsT0SegPh2.recAlgoConfig.tTrigModeConfig.t0Label  = cms.string('ph2')

    if hasattr(process,"dtNtupleProducer") and hasattr(process,pathName) :

        print "[customiseForPhase2Reco]: including phase-2 local reco in ntuples"

        process.dtNtupleProducer.ph2DtSegmentTag = cms.untracked.InputTag("dt4DSegmentsPh2")

        getattr(process,pathName).replace(process.dtNtupleProducer,
                                          process.dtlocalrecoT0SegPh2
                                          + process.dtNtupleProducer)

    return process
