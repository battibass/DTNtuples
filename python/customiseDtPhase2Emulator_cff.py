import FWCore.ParameterSet.Config as cms

def customiseForPhase2Emulator(process, pathName, isMC = False) :

    process.load("L1Trigger.DTTriggerPhase2.CalibratedDigis_cfi")
    process.CalibratedDigis.dtDigiTag = cms.InputTag('muonDTDigis' if isMC else 'dtAB7unpacker')

    process.load("L1Trigger.DTTriggerPhase2.dtTriggerPhase2PrimitiveDigis_cfi")

    #Scenario -> 2 (SliceTest only)
    process.CalibratedDigis.scenario = 2
    process.dtTriggerPhase2PrimitiveDigis.scenario = 2

    if not isMC:
        #tTrig and t0 from phase-2 db
        process.CalibratedDigis.tTrigModeConfig.t0Label = 'ph2'
        process.CalibratedDigis.tTrigModeConfig.tTrigLabel = 'cosmics_ph2'

    #SL TanPhi cut
    process.dtTriggerPhase2PrimitiveDigis.tanPhiTh = cms.double(1.5)

    #Chi2 -> Changing a lot lately
    process.dtTriggerPhase2PrimitiveDigis.chi2Th = cms.double(0.01)
    process.dtTriggerPhase2PrimitiveDigis.chi2corTh = cms.double(0.1) #in cm^2

    #Correlate with BX
    process.dtTriggerPhase2PrimitiveDigis.useBX_correlation = True
    process.dtTriggerPhase2PrimitiveDigis.dBX_correlate_TP = 1

    #Correlate with tanPsi
    #process.dtTriggerPhase2PrimitiveDigis.dTanPsi_correlate_TP = cms.double(9999./4096.)

    #Confirmation forbidden
    process.dtTriggerPhase2PrimitiveDigis.allow_confirmation = False

    if hasattr(process,"dtNtupleProducer") and hasattr(process,pathName) :

        print("[customiseForPhase2Reco]: including phase-2 emulator in ntuples")

        process.dtNtupleProducer.ph2TPGPhiEmuAmTag = 'dtTriggerPhase2PrimitiveDigis'
        process.dtNtupleProducer.ph2TPGThEmuAmTag = 'dtTriggerPhase2PrimitiveDigis'

        getattr(process,pathName).replace(process.dtNtupleProducer,
                                          process.CalibratedDigis
                                          + process.dtTriggerPhase2PrimitiveDigis
                                          + process.dtNtupleProducer)

    return process
