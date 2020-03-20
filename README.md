# DTNtuples
Ntuples for the analysis of the CMS drift tubes detector performance

## Preliminary instructions
**Note**: 
In the present days this code is evolving fast, hence the installation recipe may change often. Please keep an eye on this page to check for updates.

### Installation:
```
cmsrel CMSSW_11_0_1
cd CMSSW_11_0_1/src/
cmsenv
git cms-merge-topic oglez:Phase2_DTAB7Unpacker_v9.2
git cms-merge-topic battibass:Phase2_DTReco_110X
git cms-merge-topic swiedenb:DTCalibOfflineSlice_11_0_X_v2 
git cms-merge-topic -u battibass:HB_From106X_for110X # MTT-CHT emulator (brute force copy from Nicola)
git cms-merge-topic -u dtp2-tpg-am:AM_106X_11_0_1_noDF # AM emulator
git clone https://github.com/battibass/DTNtuples.git DTDPGAnalysis/DTNtuples -b 110X_preparation
scramv1 b -j 5
```

### Ntuple prduction:
```
cd DTDPGAnalysis/DTNtuples/test/
cmsRun dtDpgNtuples_slicetest_cfg.py nEvents=10000
# or
cmsRun dtDpgNtuples_phase2_cfg.py nEvents=10000
```

### Analysis:
```
root -b
root [0] .x loadExampleAnalysis.C

root [1] DTNtupleExampleAnalyzer analysis("DTDPGNtuple_10_6_0_SX5.root","results.root")
// or
root [1] DTNtupleExampleAnalyzer analysis("DTDPGNtuple_10_6_0_Phase2_Simulation.root","results.root")

root [2] analysis.Loop()
```
