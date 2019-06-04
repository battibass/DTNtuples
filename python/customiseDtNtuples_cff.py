import FWCore.ParameterSet.Config as cms


def customiseForRunningOnMC(process, pathName) :

    if hasattr(process,"dtNtupleProducer") :
        print "[customiseForRunningOnMC]: updating ntuple input tags"

        process.dtNtupleProducer.genPartTag = "prunedGenParticles"
        process.dtNtupleProducer.puInfoTag = "addPileupInfo"

        process.dtNtupleProducer.lumiScalerTag = "none"

        process.dtNtupleProducer.ph1TwinMuxInTag = "none"
        process.dtNtupleProducer.ph1TwinMuxOutTag = "none"
        process.dtNtupleProducer.ph1TwinMuxInThTag = "none"

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
        process.dtNtupleProducer.ph1TwinMuxInThTag = "simDtTriggerPrimitiveDigis"

    return process
