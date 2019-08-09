import FWCore.ParameterSet.Config as cms


def customiseForRunningOnMC(process, pathName) :

    if hasattr(process,"dtNtupleProducer") :
        print "[customiseForRunningOnMC]: updating ntuple input tags"

        process.dtNtupleProducer.genPartTag = "prunedGenParticles"
        # process.dtNtupleProducer.puInfoTag = "addPileupInfo"

        process.dtNtupleProducer.lumiScalerTag = "none"

        # process.dtNtupleProducer.ph1TwinMuxInTag = "none"
        # process.dtNtupleProducer.ph1TwinMuxOutTag = "none"
        # process.dtNtupleProducer.ph1TwinMuxInThTag = "none"

        if hasattr(process,pathName) :
            print "[customiseForRunningOnMC]: adding prunedGenParitcles"

            process.load('DTDPGAnalysis.DTNtuples.prunedGenParticles_cfi')

            getattr(process,pathName).replace(process.dtNtupleProducer,
                                              process.prunedGenParticles
                                              + process.dtNtupleProducer)

    return process

def customiseForPhase2Simulation(process) :

    if hasattr(process,"dtNtupleProducer") :
        print "[customiseForPhase2Simulation]: updating ntuple input tags"

        process.dtNtupleProducer.puInfoTag = "none"
        process.dtNtupleProducer.ph1BmtfInTag = "none"
        process.dtNtupleProducer.ph1BmtfInThTag = "none"
        process.dtNtupleProducer.primaryVerticesTag = "none"

        process.dtNtupleProducer.ph1DtDigiTag = "simMuonDTDigis"
        process.dtNtupleProducer.ph1TwinMuxInTag = "simDtTriggerPrimitiveDigis"
        process.dtNtupleProducer.ph1TwinMuxOutTag = "simDtTriggerPrimitiveDigis"
        process.dtNtupleProducer.ph1TwinMuxInThTag = "simDtTriggerPrimitiveDigis"

        process.dtNtupleProducer.ph2TPGPhiEmuAmTag = "dtTriggerPhase2AmPrimitiveDigis"
        process.dtNtupleProducer.ph2TPGPhiEmuHbTag = "dtTriggerPhase2HbPrimitiveDigis:MMTCHT:"

    return process

def customiseForFakePhase2Info(process) :

    if hasattr(process,"dtNtupleProducer") :
        print "[customiseForFakePhase2Info]: updating ntuple input tags"

        process.dtNtupleProducer.ph2DtDigiTag = process.dtNtupleProducer.ph1DtDigiTag
        process.dtNtupleProducer.ph2DtSegmentTag = process.dtNtupleProducer.ph1DtSegmentTag
        process.dtNtupleProducer.ph2TPGPhiHwTag = process.dtNtupleProducer.ph2TPGPhiEmuAmTag


    return process

def customiseForAgeing(process, pathName, segmentAgeing, triggerAgeing) :

    if segmentAgeing or triggerAgeing :

        if hasattr(process,"dt1DRecHits") :
            print "[customiseForAgeing]: prepending ageing before dt1DRecHits and adding ageing to RandomNumberGeneratorService"

            from SimMuon.DTDigitizer.dtChamberMasker_cfi import dtChamberMasker as _dtChamberMasker

            process.agedDtDigis = _dtChamberMasker.clone()

            getattr(process,pathName).replace(process.dt1DRecHits,
                                              process.agedDtDigis + process.dt1DRecHits)

            if hasattr(process,"RandomNumberGeneratorService") :
                process.RandomNumberGeneratorService.agedDtDigis = cms.PSet( initialSeed = cms.untracked.uint32(789342),
                                                                             engineName = cms.untracked.string('TRandom3') )
            else :
                process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                                                                    agedDtDigiss = cms.PSet( initialSeed = cms.untracked.uint32(789342),
                                                                                             engineName = cms.untracked.string('TRandom3') )
                )

    if segmentAgeing :
        print "[customiseForAgeing]: switching dt1DRecHits input to agedDtDigis"
        process.dt1DRecHits.dtDigiLabel = "agedDtDigis"

    if triggerAgeing :
        print "[customiseForAgeing]: switching emulatros input to agedDtDigis"
        process.CalibratedDigis.dtDigiTag = "agedDtDigis"
        

    return process
