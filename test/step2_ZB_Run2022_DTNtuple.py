# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --conditions auto:run3_data_prompt -s RAW2DIGI,L1Reco,RECO,ENDJOB --datatier RECO --eventcontent RECO --data --process RECO --scenario pp --customise Configuration/DataProcessing/RecoTLR.customisePrompt --era Run3 -n 100 --filein filelist:step1_dasquery.log --fileout file:step2.root
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_cff import Run3

process = cms.Process('RECO',Run3)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('DTDPGAnalysis.DTNtuples.dtNtupleProducer_collision_cfi')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100),
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#                                 '/store/data/Run2022B/ZeroBias/RAW/v1/000/355/404/00000/01899c98-9e6a-41e1-9033-1cd2875f5fea.root',
#                                 '/store/data/Run2022B/ZeroBias/RAW/v1/000/355/404/00000/08031773-f70e-4330-919a-0b7051b71620.root',
#                                 '/store/data/Run2022B/ZeroBias/RAW/v1/000/355/404/00000/1095a92d-78ab-4cdc-afcb-c2e1c15a70b3.root',
#                                 '/store/data/Run2022B/ZeroBias/RAW/v1/000/355/404/00000/14934cf2-c2d4-4547-ac4a-bb7684a1eec5.root',
#                                 '/store/data/Run2022B/ZeroBias/RAW/v1/000/355/404/00000/358943dc-3ad9-4254-a272-7faa29483311.root',
                                '/store/data/Run2022D/ZeroBias/RAW/v1/000/357/803/00000/01601098-44e1-482f-adb1-043454a0e819.root',
                                '/store/data/Run2022D/ZeroBias/RAW/v1/000/357/803/00000/1ba494f8-290d-4367-974b-58026d4a4738.root',
                                '/store/data/Run2022D/ZeroBias/RAW/v1/000/357/803/00000/20a93f53-e322-428e-bd66-5f891a84c155.root',
                                '/store/data/Run2022D/ZeroBias/RAW/v1/000/357/803/00000/2f78ea24-c209-4cc5-9485-3b5838414f0b.root',
                                '/store/data/Run2022D/ZeroBias/RAW/v1/000/357/803/00000/2fe35a19-f656-4ec4-b1de-1c180e0d7e19.root',
                            ),
                            secondaryFileNames = cms.untracked.vstring()
                        )
# print(process.source.fileNames)


process.options = cms.untracked.PSet(
    FailPath = cms.untracked.vstring(),
    IgnoreCompletely = cms.untracked.vstring(),
    Rethrow = cms.untracked.vstring(),
    SkipEvent = cms.untracked.vstring(),
    accelerators = cms.untracked.vstring('*'),
    allowUnscheduled = cms.obsolete.untracked.bool,
    canDeleteEarly = cms.untracked.vstring(),
    deleteNonConsumedUnscheduledModules = cms.untracked.bool(True),
    dumpOptions = cms.untracked.bool(False),
    emptyRunLumiMode = cms.obsolete.untracked.string,
    eventSetup = cms.untracked.PSet(
        forceNumberOfConcurrentIOVs = cms.untracked.PSet(
            allowAnyLabel_=cms.required.untracked.uint32
        ),
        numberOfConcurrentIOVs = cms.untracked.uint32(0)
    ),
    fileMode = cms.untracked.string('FULLMERGE'),
    forceEventSetupCacheClearOnNewRun = cms.untracked.bool(False),
    makeTriggerResults = cms.obsolete.untracked.bool,
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(0),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfStreams = cms.untracked.uint32(0),
    numberOfThreads = cms.untracked.uint32(1),
    printDependencies = cms.untracked.bool(False),
    sizeOfStackForThreadsInKB = cms.optional.untracked.uint32,
    throwIfIllegalParameter = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:step2_run357803_ZeroBias_RECO.root'),
    outputCommands = process.RECOEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition
process.TFileService = cms.Service('TFileService',
#     fileName = cms.string('./step2_run357803_ZeroBias_DTDPGNtuple.root')
    fileName = cms.string('./DTDPGNtuple_124X_Run3_2022_RAW_RECO.root')
)


# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run3_data_prompt', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.dtNtupleProducer_step = cms.Path(process.dtNtupleProducer)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# Schedule definition
# process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.dtNtupleProducer_step,process.endjob_step,process.RECOoutput_step)
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.dtNtupleProducer_step,process.endjob_step) # without process.RECOoutput_step
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# customisation of the process.

# Automatic addition of the customisation function from Configuration.DataProcessing.RecoTLR
from Configuration.DataProcessing.RecoTLR import customisePrompt 

#call to customisation function customisePrompt imported from Configuration.DataProcessing.RecoTLR
process = customisePrompt(process)

# End of customisation functions


# Customisation from command line

#Have logErrorHarvester wait for the same EDProducers to finish as those providing data for the OutputModule
from FWCore.Modules.logErrorHarvester_cff import customiseLogErrorHarvesterUsingOutputCommands
process = customiseLogErrorHarvesterUsingOutputCommands(process)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
