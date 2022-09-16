# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --conditions auto:run2_data -s RAW2DIGI,L1Reco,RECO,ENDJOB --datatier RECO --eventcontent RECO --data --process RECO --scenario pp --era Run2_2018 --customise Configuration/DataProcessing/RecoTLR.customisePostEra_Run2_2018 -n 100 --filein filelist:step1_dasquery_2018.log --fileout file:step2.root --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run2_2018_cff import Run2_2018

process = cms.Process('RECO',Run2_2018)

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
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/088D0E79-8497-E811-AF16-FA163EB2BB26.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/0CF43330-8897-E811-A6ED-02163E01A167.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/1E4BC5A5-8697-E811-89CB-FA163E373C99.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/20CAEE43-8997-E811-808C-FA163E63C228.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/226C0DAF-8897-E811-94A9-FA163EC6A3B2.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/24B87198-8897-E811-AB14-FA163E8B9DC9.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/2A1F9C93-8797-E811-AD68-02163E01A0CF.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/2ABDE8A1-8497-E811-A635-FA163EC95E7D.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/32FF2EE6-8797-E811-B228-FA163ED6DF53.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/3E6504CB-8897-E811-82B4-02163E01A047.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/3E7DC1A7-8697-E811-8003-FA163E51B2B9.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/42872587-8597-E811-BDE7-FA163E6788A6.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/48335ABE-8597-E811-B632-FA163E54F658.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/4AAAD260-8897-E811-A8E5-FA163E651871.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/585D8B9A-8497-E811-83A2-FA163EE15E25.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/5A956C87-8697-E811-A6DF-FA163E8E784C.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/685A29EB-8897-E811-BD50-FA163E1A6759.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/6CAAD49A-8497-E811-B34D-FA163E9556FC.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/783AA587-8597-E811-BDD3-FA163EE797E1.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/88B8B573-8797-E811-B644-FA163E4413A6.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/8C76DC8C-8597-E811-A169-FA163EB6E388.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/9C2F129E-8497-E811-A7CB-FA163E726F24.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/A4FDF4B6-8597-E811-A6EE-FA163E84B095.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/A81AF1A7-8697-E811-8B61-FA163ECBB7E7.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/AE1DBB9A-8497-E811-B3D7-02163E019FCC.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/AE4EEB82-8697-E811-A762-FA163E659E42.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/BC80D52F-8797-E811-827C-FA163EF06BB3.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/CC41391E-8797-E811-9FF5-02163E01A063.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/D07DC6AB-8697-E811-87A1-02163E0164AA.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/DEE8DD76-8597-E811-84D9-FA163E054896.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/E26FD820-8897-E811-AA57-02163E010CAA.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/E42C2C01-8697-E811-A30F-FA163EE70CD0.root',
        '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/E4A2F187-8797-E811-A785-02163E017748.root'
    ),
    secondaryFileNames = cms.untracked.vstring()
)

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
    fileName = cms.untracked.string('file:step2.root'),
    outputCommands = process.RECOEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)


# Additional output definition
process.TFileService = cms.Service('TFileService',
#    fileName = cms.string('./step2_run320822_ZeroBias_DTDPGNtuple.root')
     fileName = cms.string('./DTDPGNtuple_124X_Run2_2018_RAW_RECO.root')
)

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

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
from Configuration.DataProcessing.RecoTLR import customisePostEra_Run2_2018 

#call to customisation function customisePostEra_Run2_2018 imported from Configuration.DataProcessing.RecoTLR
process = customisePostEra_Run2_2018(process)

# End of customisation functions


# Customisation from command line

#Have logErrorHarvester wait for the same EDProducers to finish as those providing data for the OutputModule
from FWCore.Modules.logErrorHarvester_cff import customiseLogErrorHarvesterUsingOutputCommands
process = customiseLogErrorHarvesterUsingOutputCommands(process)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
