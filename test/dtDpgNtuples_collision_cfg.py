import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from Configuration.StandardSequences.Eras import eras

import subprocess
import sys

options = VarParsing.VarParsing()

options.register('globalTag',
                 '122X_mcRun3_2021_realistic_v5', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Global Tag")

options.register('nEvents',
                 -1, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Maximum number of processed events")

options.register('inputFolder',
                 '/eos/cms/store/relval/CMSSW_12_2_0/RelValZMM_14/GEN-SIM-RECO/122X_mcRun3_2021_realistic_v5-v2/2580000/', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "EOS folder with input files")

options.register('secondaryInputFolder',
                 '/eos/cms/store/relval/CMSSW_12_2_0/RelValZMM_14/GEN-SIM-DIGI-RAW/122X_mcRun3_2021_realistic_v5-v2/2580000/', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "EOS folder with input files for secondary files")

options.register('ntupleName',
                 './DTDPGNtuple_12_2_1_ZMM.root', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Folder and name ame for output ntuple")

options.register('runOnMC',
                 True, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Apply customizations to run on MC")

options.parseArguments()

process = cms.Process("DTNTUPLES",eras.Run3)

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.nEvents))

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = cms.string(options.globalTag)

process.source = cms.Source("PoolSource",
                            
        fileNames = cms.untracked.vstring(),
        secondaryFileNames = cms.untracked.vstring()

)

files = subprocess.check_output(["ls", options.inputFolder])
process.source.fileNames = ["file://" + options.inputFolder + "/" + f.decode("utf-8") for f in files.split()]

if options.secondaryInputFolder != "" :
    files = subprocess.check_output(["ls", options.secondaryInputFolder])
    process.source.secondaryFileNames = ["file://" + options.secondaryInputFolder + "/" + f.decode("utf-8") for f in files.split()]

process.TFileService = cms.Service('TFileService',
        fileName = cms.string(options.ntupleName)
    )

process.load('Configuration/StandardSequences/GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('RecoLocalMuon.Configuration.RecoLocalMuon_cff')

process.load('DTDPGAnalysis.DTNtuples.dtNtupleProducer_collision_cfi')

process.p = cms.Path(process.muonDTDigis 
                     + process.bmtfDigis
                     + process.twinMuxStage2Digis
                     + process.scalersRawToDigi
                     + process.dtlocalreco
                     + process.dtNtupleProducer)

if options.runOnMC :
    from DTDPGAnalysis.DTNtuples.customiseDtNtuples_cff import customiseForRunningOnMC
    customiseForRunningOnMC(process,"p")


