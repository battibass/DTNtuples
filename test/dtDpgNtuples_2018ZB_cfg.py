import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from Configuration.StandardSequences.Eras import eras

import subprocess
import sys

options = VarParsing.VarParsing()

# options.register('globalTag',
#                  '124X_dataRun3_Prompt_v4', #default value
#                  # 'auto:run2_data',
#                  # 2022 :: 124X_dataRun3_Prompt_v4
#                  # 2018 :: 103X_dataRun2_Prompt_v3
#                  # 2017 :: 92X_dataRun2_Prompt_v11
#                  VarParsing.VarParsing.multiplicity.singleton,
#                  VarParsing.VarParsing.varType.string,
#                  "Global Tag")

options.register('nEvents',
                 -1, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Maximum number of processed events")

options.register('inputFolder',
                 '/eos/cms/store/data/Run2022B/ZeroBias/RAW/v1/000/355/404/00000/', #default value EOS
                 # '/store/data/Run2022B/ZeroBias/RAW/v1/000/355/404/00000/',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "EOS folder with input files")

options.register('secondaryInputFolder',
                 '/eos/cms/store/data/Run2022B/ZeroBias/RAW/v1/000/355/404/00000/', #default value EOS
                 # '/store/data/Run2022B/ZeroBias/RAW/v1/000/355/404/00000/',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "EOS folder with input files for secondary files")

options.register('ntupleName',
                 './DTDPGNtuple_124X_Run2_2018_ZeroBias.root', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Folder and name ame for output ntuple")

options.register('runOnMC',
                 False, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Apply customizations to run on MC")

options.parseArguments()


# process = cms.Process("DTNTUPLES",eras.Run3)
process = cms.Process("DTNTUPLES",eras.Run2_2018)
# process = cms.Process("DTNTUPLES",eras.Run2_2017)


process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.nEvents))

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# process.GlobalTag.globaltag = cms.string(options.globalTag)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

# from Configuration.Eras.Era_Run3_cff import Run3
# process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
# process.load('Configuration.StandardSequences.L1Reco_cff')
# process.load('Configuration.StandardSequences.Reconstruction_Data_cff')

# works only on LXPLUS with access to EOS -- not possible su T2_IT_Bari
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# process.source = cms.Source("PoolSource",
#         fileNames = cms.untracked.vstring(),
#         secondaryFileNames = cms.untracked.vstring()
# )
# files = subprocess.check_output(["ls", options.inputFolder])
# process.source.fileNames = ["file://" + options.inputFolder + "/" + f.decode("utf-8") for f in files.split()]
# if options.secondaryInputFolder != "" :
#     files = subprocess.check_output(["ls", options.secondaryInputFolder])
#     process.source.secondaryFileNames = ["file://" + options.secondaryInputFolder + "/" + f.decode("utf-8") for f in files.split()]

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
#                                 - - - - - - - - - - - - - - -
#                                 '/store/data/Run2022B/ZeroBias/RAW/v1/000/355/404/00000/01899c98-9e6a-41e1-9033-1cd2875f5fea.root',
#                                 '/store/data/Run2022B/ZeroBias/RAW/v1/000/355/404/00000/08031773-f70e-4330-919a-0b7051b71620.root',
#                                 '/store/data/Run2022B/ZeroBias/RAW/v1/000/355/404/00000/1095a92d-78ab-4cdc-afcb-c2e1c15a70b3.root',
#                                 '/store/data/Run2022B/ZeroBias/RAW/v1/000/355/404/00000/14934cf2-c2d4-4547-ac4a-bb7684a1eec5.root',
#                                 '/store/data/Run2022B/ZeroBias/RAW/v1/000/355/404/00000/358943dc-3ad9-4254-a272-7faa29483311.root',
#                                 - - - - - - - - - - - - - - -
#                                 '/store/data/Run2022D/ZeroBias/RAW/v1/000/357/803/00000/01601098-44e1-482f-adb1-043454a0e819.root',
#                                 '/store/data/Run2022D/ZeroBias/RAW/v1/000/357/803/00000/1ba494f8-290d-4367-974b-58026d4a4738.root',
#                                 '/store/data/Run2022D/ZeroBias/RAW/v1/000/357/803/00000/20a93f53-e322-428e-bd66-5f891a84c155.root',
#                                 '/store/data/Run2022D/ZeroBias/RAW/v1/000/357/803/00000/2f78ea24-c209-4cc5-9485-3b5838414f0b.root',
#                                 '/store/data/Run2022D/ZeroBias/RAW/v1/000/357/803/00000/2fe35a19-f656-4ec4-b1de-1c180e0d7e19.root',
#                                 - - - - - - - - - - - - - - -
#                                 'file:/lustre/cms/store/data/Commissioning2018/ZeroBias/RAW/v1/000/314/890/00000/66593389-A346-E811-9D77-FA163E83B392.root',
                                '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/088D0E79-8497-E811-AF16-FA163EB2BB26.root',
                                '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/0CF43330-8897-E811-A6ED-02163E01A167.root',
                                '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/1E4BC5A5-8697-E811-89CB-FA163E373C99.root',
                                '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/20CAEE43-8997-E811-808C-FA163E63C228.root',
                                '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/226C0DAF-8897-E811-94A9-FA163EC6A3B2.root',
                                '/store/data/Run2018D/ZeroBias/RAW/v1/000/320/822/00000/24B87198-8897-E811-AB14-FA163E8B9DC9.root',
#                                 - - - - - - - - - - - - - - -
#                                 - - - - - - - - - - - - - - -
                            ),
                            secondaryFileNames = cms.untracked.vstring()
                        )
print(process.source.fileNames)


process.TFileService = cms.Service('TFileService',
        fileName = cms.string(options.ntupleName)
    )

process.load('Configuration/StandardSequences/GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('RecoLocalMuon.Configuration.RecoLocalMuon_cff')
# process.load('Configuration.StandardSequences.Reconstruction_Data_cff.py')

process.load('DTDPGAnalysis.DTNtuples.dtNtupleProducer_collision_cfi')

process.p = cms.Path(process.muonDTDigis 
                     + process.bmtfDigis
                     + process.twinMuxStage2Digis
                     # + process.scalersRawToDigi      # probably can be removed?
                     # + process.tcdsDigis             # necessary?
                     + process.onlineMetaDataDigis   # likely this is needed for online lumi
                     + process.dtlocalreco
                     + process.dtNtupleProducer)
if options.runOnMC :
    from DTDPGAnalysis.DTNtuples.customiseDtNtuples_cff import customiseForRunningOnMC
    customiseForRunningOnMC(process,"p")


# process.raw2digi_step = cms.Path(process.RawToDigi)
# process.L1Reco_step = cms.Path(process.L1Reco)
# process.reconstruction_step = cms.Path(process.reconstruction)

# Schedule definition
# process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.dtNtupleProducer)
