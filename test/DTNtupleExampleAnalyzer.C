#include "DTNtupleExampleAnalyzer.h"

DTNtupleExampleAnalyzer::DTNtupleExampleAnalyzer(const TString & inFileName,
						 const TString & outFileName) :
  m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName)  
{ 

}

DTNtupleExampleAnalyzer::~DTNtupleExampleAnalyzer() 
{ 

}


void DTNtupleExampleAnalyzer::Loop()
{

  book();

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) 
    {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEvent(jentry);   nbytes += nb;

      if(jentry % 100 == 0) 
	std::cout << "[DTNtupleExampleAnalyzer::Loop] processed : " 
		  << jentry << " entries\r" << std::flush;

      fill();

    }

  std::cout << std::endl; 

  endJob();

}

void DTNtupleExampleAnalyzer::book()
{

  m_outFile.cd();

  m_plots["hTimeBox"] = new TH1F("hTimeBox",
				 "Example time box; time (ns); entries/5 ns",
				 250,0.,1250.); 
}

void DTNtupleExampleAnalyzer::fill()
{

  for (std::size_t iDigi = 0; iDigi < digi_nDigis; ++iDigi)
    m_plots["hTimeBox"]->Fill(digi_time->at(iDigi));

}

void DTNtupleExampleAnalyzer::endJob()
{

  m_outFile.cd();

  m_outFile.Write();
  m_outFile.Close();

}

