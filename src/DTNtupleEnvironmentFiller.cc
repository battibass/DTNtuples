/** \class DTNtupleEnvironmentFiller DTNtupleEnvironmentFiller.cc DTDPGAnalysis/DTNtuples/src/DTNtupleEnvironmentFiller.cc
 *  
 * Helper class : the pile-up, luminosity and reco vertices filler
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleEnvironmentFiller.h"

#include "FWCore/Framework/interface/Event.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

DTNtupleEnvironmentFiller::DTNtupleEnvironmentFiller(edm::ConsumesCollector && collector,
						     const std::shared_ptr<DTNtupleConfig> config, 
						     std::shared_ptr<TTree> tree, const std::string & label) : 
  DTNtupleBaseFiller(config, tree, label)
{

  edm::InputTag iTag = m_config->m_inputTags["puInfoTag"];
  if (iTag.label() != "none") 
    m_puInfoToken = collector.consumes<std::vector<PileupSummaryInfo> >(iTag);

  iTag = m_config->m_inputTags["lumiScalerTag"];
  if (iTag.label() != "none") 
    m_lumiScalerToken = collector.consumes<OnlineLuminosityRecord>(iTag);     // 2018-2022

  iTag = m_config->m_inputTags["lumiScalerTag2017"];
  if (iTag.label() != "none") 
    m_lumiScalerToken_2017 = collector.consumes<LumiScalersCollection>(iTag); // 2017

  iTag = m_config->m_inputTags["primaryVerticesTag"];
  if (iTag.label() != "none") 
    m_primariVerticesToken = collector.consumes<reco::VertexCollection>(iTag);

}

DTNtupleEnvironmentFiller::~DTNtupleEnvironmentFiller() 
{ 

};

void DTNtupleEnvironmentFiller:: initialize()
{
  
  m_tree->Branch((m_label + "_truePileUp").c_str(), &m_truePileUp, (m_label + "_truePileUp/S").c_str());
  m_tree->Branch((m_label + "_actualPileUp").c_str(), &m_actualPileUp, (m_label + "_actualPileUp/S").c_str());

  m_tree->Branch((m_label + "_instLumi").c_str(), &m_instLumi, (m_label + "_instLumi/I").c_str());
  m_tree->Branch((m_label + "_onlinePileUp").c_str(), &m_onlinePileUp, (m_label + "_onlinePileUp/S").c_str());

  m_tree->Branch((m_label + "_nPV").c_str(), &m_nPV, (m_label + "_nPV/S").c_str());

  m_tree->Branch((m_label + "_pv_x").c_str(), &m_pv_x, (m_label + "_pv_x/F").c_str());
  m_tree->Branch((m_label + "_pv_y").c_str(), &m_pv_y, (m_label + "_pv_y/F").c_str());
  m_tree->Branch((m_label + "_pv_z").c_str(), &m_pv_z, (m_label + "_pv_z/F").c_str());

  m_tree->Branch((m_label + "_pv_xxErr").c_str(), &m_pv_xxErr, (m_label + "_pv_xxErr/F").c_str());
  m_tree->Branch((m_label + "_pv_yyErr").c_str(), &m_pv_yyErr, (m_label + "_pv_yyErr/F").c_str());
  m_tree->Branch((m_label + "_pv_zzErr").c_str(), &m_pv_zzErr, (m_label + "_pv_zzErr/F").c_str());
  m_tree->Branch((m_label + "_pv_xyErr").c_str(), &m_pv_xyErr, (m_label + "_pv_xyErr/F").c_str());
  m_tree->Branch((m_label + "_pv_xzErr").c_str(), &m_pv_xzErr, (m_label + "_pv_xzErr/F").c_str());
  m_tree->Branch((m_label + "_pv_yzErr").c_str(), &m_pv_yzErr, (m_label + "_pv_yzErr/F").c_str());

  
}

void DTNtupleEnvironmentFiller::clear()
{

  m_truePileUp   = DTNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_actualPileUp = DTNtupleBaseFiller::DEFAULT_INT_VAL_POS;

  m_instLumi     = DTNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_onlinePileUp = DTNtupleBaseFiller::DEFAULT_INT_VAL_POS;

  m_nPV = 0;

  m_pv_x = DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  m_pv_y = DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  m_pv_z = DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL;

  m_pv_xxErr = DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  m_pv_yyErr = DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  m_pv_zzErr = DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  m_pv_xyErr = DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  m_pv_xzErr = DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  m_pv_yzErr = DTNtupleBaseFiller::DEFAULT_DOUBLE_VAL;
  
}

void DTNtupleEnvironmentFiller::fill(const edm::Event & ev)
{

  clear();
  bool debug = false;

  auto puInfo = conditionalGet<std::vector<PileupSummaryInfo> >(ev, m_puInfoToken, "vector<PileupSummaryInfo>");
  if(debug) std::cout<<"[DTNupleEnvironmentFiller] puInfo.isValid() = "<<puInfo.isValid()<<std::endl;
  if (puInfo.isValid()) 
    {

      for (const auto & puInfoBx : (*puInfo))
	{

	  int bx = puInfoBx.getBunchCrossing();
	       
	  if(bx == 0) 
	    { 
	      m_truePileUp   = puInfoBx.getTrueNumInteractions();
	      m_actualPileUp = puInfoBx.getPU_NumInteractions();
	      break;
	    }
	}
      if(debug) std::cout<<"[DTNupleEnvironmentFiller] actualPU = "<<m_actualPileUp<<" | truePU = "<<m_truePileUp<<std::endl;
    }

  auto lumiScalers     = conditionalGet<OnlineLuminosityRecord>(ev, m_lumiScalerToken, "OnlineLuminosityRecord");     // 2018-2022
  auto lumiScalers2017 = conditionalGet<LumiScalersCollection>(ev,  m_lumiScalerToken_2017, "LumiScalersCollection"); // 2017
  double instLumi_2018=0, instLumi_2017=0;
  int    onlinePileUp_2018=0, onlinePileUp_2017=0;
  if(debug) std::cout<<"[DTNupleEnvironmentFiller] lumiScalers.isValid() = "<<lumiScalers.isValid()<<" lumiScalers2017.isValid() = "<<lumiScalers2017.isValid()<<std::endl;
  if (lumiScalers.isValid()) // 2018-2022
    {
      instLumi_2018 = lumiScalers->instLumi();      // 2018-2022
      onlinePileUp_2018 = lumiScalers->avgPileUp(); // 2018-2022
      std::cout<<"[DTNupleEnvironmentFiller] [lumiScalers 2018-2022] instLumi = "<<instLumi_2018<<" E30 cm-2s-1 | avgPU = "<<onlinePileUp_2018<<std::endl;
    }
  if (lumiScalers2017.isValid()) // 2017
    {
      instLumi_2017 = lumiScalers2017->begin()->instantLumi(); // 2017
      onlinePileUp_2017 = lumiScalers2017->begin()->pileup();  // 2017
      std::cout<<"[DTNupleEnvironmentFiller] [lumiScalers 20XX-2017] instLumi = "<<instLumi_2017<<" E30 cm-2s-1 | avgPU = "<<onlinePileUp_2017<<std::endl;
    }
  if(instLumi_2017>0) { m_instLumi = instLumi_2017; m_onlinePileUp = onlinePileUp_2017; }
  if(instLumi_2018>0) { m_instLumi = instLumi_2018; m_onlinePileUp = onlinePileUp_2018; }
  std::cout<<"[DTNupleEnvironmentFiller] instLumi = "<<m_instLumi<<" E30 cm-2s-1 | avgPU = "<<m_onlinePileUp<<std::endl;

  auto primaryVtx = conditionalGet<reco::VertexCollection>(ev, m_primariVerticesToken,"VertexCollection");
  if(debug) std::cout<<"[DTNupleEnvironmentFiller] primaryVtx.isValid() = "<<primaryVtx.isValid()<<std::endl;
  if (primaryVtx.isValid()) 
    {

      m_nPV = primaryVtx->size();

      if (m_nPV > 0) 
	{
	  const auto& firstPV = (*primaryVtx)[0];
	  
	  m_pv_x = firstPV.position().x();
	  m_pv_x = firstPV.position().x();
	  m_pv_x = firstPV.position().x();
    
	  m_pv_xxErr = firstPV.covariance(0,0);
	  m_pv_yyErr = firstPV.covariance(1,1);
	  m_pv_zzErr = firstPV.covariance(2,2);

	  m_pv_xyErr = firstPV.covariance(0,1);
	  m_pv_xzErr = firstPV.covariance(0,2);
          m_pv_yzErr = firstPV.covariance(1,2);

	}
      if(debug) std::cout<<"[DTNupleEnvironmentFiller] number of primary vertices = "<<m_nPV<<std::endl;
    }
    
  return;

}
