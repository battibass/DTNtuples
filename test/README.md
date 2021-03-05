# Production of DTNtuples and useful tools

## Running ntuple production

Three basic scripts to produce ntuples are provided:

1. `dtDpgNtuples_collision_cfg.py` : suitable for running on "present run" (e.g. Run 2) samples;
2. `dtDpgNtuples_slicetest_cfg.py` : aimed at processing DT slice test data;
3. `dtDpgNtuples_phase2_cfg.py` : designed for processing phase-2 samples (e.g. the ones for L1T TDR).

Each of them can be configured by means of command-line arguments that can be listed by issuing:

```bash
python dtDpgNtuples_[collision,slicetest,phase2]_cfg.py --help
```

To run a given script is enough to issue:
```bash
cmsRun dtDpgNtuples_[collision,slicetest,phase2]_cfg.py [command line parameters]

# E.g.
cmsRun dtDpgNtuples_slicetest_cfg.py runNumber=333510
```

## Scripts working on ntuples

A few tools are also provided to manipulate existing DTNtuples :

1. `skimTree` : allwos to apply cut-based skimming of a tree (as the one a person would use to make a cut in Tree::Draw()) and potentially drop a few of the three branches;
2. `printTree` : prints the size of each of the tree branches, of the fillers and of the whole tree;
3. `jsonToCut` : takes in input a data certification JSON and generates a cut string that can be used by the `skimTree` program.

Each script can accepts command-line arguments. Their use can be listed by issuing:

```bash
[skimTree,printTree,jsonToCut] --help
```

An example, you can issue a skimming command as :

```bash
skimTree -c "event_lumiBlock >=2" -r "seg_*" DTDPGNtuple_run333510.root DTDPGNtuple_skim.root
```

and you can compare the size of the ntuple before and after the skim by issuing :

```bash
printTree DTDPGNtuple_run333510.root
printTree DTDPGNtuple_skim.root
```
