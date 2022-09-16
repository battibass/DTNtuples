# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --conditions auto:run2_data -s RAW2DIGI,L1Reco,RECO,ENDJOB --datatier RECO --eventcontent RECO --data --process RECO --scenario pp --era Run2_2017 --customise Configuration/DataProcessing/RecoTLR.customisePostEra_Run2_2017 -n 100 --lumiToProcess step1_lumiRanges_2017.log --filein filelist:step1_dasquery_2017.log --fileout file:step2_2017.root --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run2_2017_cff import Run2_2017

process = cms.Process('RECO',Run2_2017)

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
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/08CF8873-F6B0-E711-ADC5-02163E011B75.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/0C2A0D6D-F6B0-E711-BE46-02163E01A400.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/1A3BE91F-F8B0-E711-A5CE-02163E0123C7.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/1A888D27-FAB0-E711-80C5-02163E014258.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/40448627-F7B0-E711-9501-02163E01372E.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/425EAEB1-FBB0-E711-B1E8-02163E011C08.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/4882F1A8-FBB0-E711-9D79-02163E019BB5.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/52B35364-F8B0-E711-A85F-02163E01425A.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/5C4C68A3-F7B0-E711-AF88-02163E013449.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/606FFE9E-F8B0-E711-A4E9-02163E013775.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/62AF9978-F6B0-E711-8871-02163E01A702.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/668E023D-FAB0-E711-8C7F-02163E011CB2.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/68891FAE-F6B0-E711-8E20-02163E01255C.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/6AF5AA64-F6B0-E711-9CB8-02163E01A3B4.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/78093231-F9B0-E711-AC48-02163E011D08.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/84BCD7C6-F9B0-E711-A719-02163E0140F7.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/8EDC3131-F7B0-E711-A8C9-02163E013903.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/961501ED-F8B0-E711-8F0C-02163E019CB8.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/9A41620E-FBB0-E711-8BA8-02163E011E3E.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/A05FF35A-F9B0-E711-B053-02163E011AB9.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/A83D9F06-F8B0-E711-8F32-02163E01454C.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/B64D5C42-FAB0-E711-924B-02163E014303.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/CC634F7B-F7B0-E711-AD5E-02163E014542.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/CCD7BC80-F7B0-E711-B9B9-02163E01A560.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/DCE5F59C-F7B0-E711-9F4C-02163E01A696.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/EC0F00E5-F7B0-E711-9E90-02163E0144E3.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/EC152EA5-F9B0-E711-BC08-02163E01A629.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/ECB0A7A3-FAB0-E711-B041-02163E01420B.root',
        '/store/data/Run2017F/ZeroBias/RAW/v1/000/305/064/00000/ECB74FD1-FAB0-E711-97FA-02163E014280.root'
    ),
    lumisToProcess = cms.untracked.VLuminosityBlockRange("305064:2-305064:101"),
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
    fileName = cms.untracked.string('file:step2_2017.root'),
    outputCommands = process.RECOEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition
process.TFileService = cms.Service('TFileService',
      fileName = cms.string('./step2_run305064_ZeroBias_DTDPGNtuple.root')
#     fileName = cms.string('./DTDPGNtuple_124X_Run2_2017_RAW_RECO.root')
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
from Configuration.DataProcessing.RecoTLR import customisePostEra_Run2_2017 

#call to customisation function customisePostEra_Run2_2017 imported from Configuration.DataProcessing.RecoTLR
process = customisePostEra_Run2_2017(process)

# End of customisation functions


# Customisation from command line

#Have logErrorHarvester wait for the same EDProducers to finish as those providing data for the OutputModule
from FWCore.Modules.logErrorHarvester_cff import customiseLogErrorHarvesterUsingOutputCommands
process = customiseLogErrorHarvesterUsingOutputCommands(process)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
