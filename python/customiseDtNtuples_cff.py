import FWCore.ParameterSet.Config as cms


def customiseForRuningOnMC(process, pathName) :

    if hasattr(process,"dtNtupleProducer") :
        print "[customiseForRuningOnMC]: updating ntuple input tags"

        process.dtNtupleProducer.genPartTag = "prunedGenParticles"
        process.dtNtupleProducer.puInfoTag = ""

        process.dtNtupleProducer.lumiScalerTag = "none"

        process.dtNtupleProducer.ph1TwinMuxInTag = "none"
        process.dtNtupleProducer.ph1TwinMuxOutTag = "none"
        process.dtNtupleProducer.ph1TwinMuxInThTag = "none"

        if hasattr(process,pathName) :
            print "[customiseForRuningOnMC]: adding prunedGenParitcles"

            process.load('DTDPGAnalysis.DTNtuples.prunedGenParticles_cfi')

            getattr(process,pathName).replace(process.dtNtupleProducer,
                                              process.prunedGenParticles
                                              + process.dtNtupleProducer)

    return process
