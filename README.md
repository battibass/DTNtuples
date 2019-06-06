# DTNtuples
Ntuples for the analysis of the CMS drift tubes detector performance

## Preliminary instructions
**Note**: 
In the present days this code is evolving fast, hence the installation recipe may change often. Please keep an eye on this page to check for updates.

### Installation:
```
cmsrel CMSSW_10_6_0
cd CMSSW_10_6_0/src/
cmsenv
git cms-merge-topic battibass:phase2UnpackerFromOscar_10_6_X
git clone https://github.com/battibass/DTNtuples.git -b simulation_workflows DTDPGAnalysis/DTNtuples
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
