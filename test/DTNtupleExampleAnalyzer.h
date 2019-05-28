#ifndef DTTnPBaseAnalysis_h
#define DTTnPBaseAnalysis_h

#include "DTNtupleBaseAnalyzer.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <map>

class DTNtupleExampleAnalyzer : public DTNtupleBaseAnalyzer 
{

 public:
  
  DTNtupleExampleAnalyzer(const TString & inFileName,
			  const TString & outFileName);
  ~DTNtupleExampleAnalyzer();

  void virtual Loop() override;

 protected:

  void book();
  void fill();
  void endJob();
  
  TFile m_outFile;
  
  std::map<std::string, TH1*> m_plots;
  
};

#endif
