/** \class DTNtupleBaseFiller DTNtupleBaseFiller.cc DTDPGAnalysis/DTNtuples/src/DTNtupleBaseFiller.cc
 *  
 * Helper class defining the generic interface of a filler
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBaseFiller.h" 

DTNtupleBaseFiller::DTNtupleBaseFiller(const std::shared_ptr<DTNtupleConfig> config, 
				       std::shared_ptr<TTree> tree, const std::string & label) : 
  m_config(config), m_tree(tree), m_label(label) 
{ 
  
}

DTNtupleBaseFiller::~DTNtupleBaseFiller()
{ 

}
