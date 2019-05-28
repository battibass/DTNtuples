#include "TROOT.h"

void loadExampleAnalysis()
{
  
  gROOT->ProcessLine(".L DTNtupleBaseAnalyzer.C++");
  gROOT->ProcessLine(".L DTNtupleExampleAnalyzer.C++");

}
