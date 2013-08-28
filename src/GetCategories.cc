#include "GetCategories.h"



//------------------------
// Hgg analysis categories

int GetStraightCategory(const float& scEta1, const float& R91, const float& scEta2, const float& R92)
{
  if( (fabs(scEta1) < 1.4442) && (fabs(scEta2) < 1.4442) )
  {
    if( (R91 > 0.94) && (R92 > 0.94) ) return 0;
    if( (R91 < 0.94) && (R92 < 0.94) ) return 1;
  }
  
  if( (fabs(scEta1) > 1.560) && (fabs(scEta2) > 1.560) )
  {
    if( (R91 > 0.94) && (R92 > 0.94) ) return 2;
    if( (R91 < 0.94) && (R92 < 0.94) ) return 3;
  }
  
  return -1;
}



int GetHggCiCCategory(const float& scEta1, const float& R91, const float& scEta2, const float& R92)
{
  if( (fabs(scEta1) < 1.4442) && (fabs(scEta2) < 1.4442) )
  {
    if( (R91 > 0.94) && (R92 > 0.94) ) return 0;
    else                               return 1;
  }
  
  else
  {
    if( (R91 > 0.94) && (R92 > 0.94) ) return 2;
    else                               return 3;
  }
  
  return -1;
}



//--------------------------------
// calibration-oriented categories

std::string GetEtaR9CatLabel(const float& eta1, const float& R91, const float& eta2, const float& R92,
                             const std::vector<float>& etaBins, const std::vector<float>& R9Bins,
                             const std::vector<std::string>& categories)
{
  std::string catEle1 = "undefined";
  std::string catEle2 = "undefined";
  
  for(unsigned int etaBin = 0; etaBin < etaBins.size()-1; ++etaBin)
    if( (eta1 >= etaBins.at(etaBin)) && (eta1 < etaBins.at(etaBin+1)) )
      for(unsigned int R9Bin = 0; R9Bin < R9Bins.size()-1; ++R9Bin)
	if( (R91 >= R9Bins.at(R9Bin)) && (R91 < R9Bins.at(R9Bin+1)) )
	  catEle1 = Form("eta%1.1f-%1.1f_R9%1.2f-%1.2f",etaBins.at(etaBin),etaBins.at(etaBin+1),R9Bins.at(R9Bin),R9Bins.at(R9Bin+1));
  
  for(unsigned int etaBin = 0; etaBin < etaBins.size()-1; ++etaBin)
    if( (eta2 >= etaBins.at(etaBin)) && (eta2 < etaBins.at(etaBin+1)) )
      for(unsigned int R9Bin = 0; R9Bin < R9Bins.size()-1; ++R9Bin)
	if( (R92 >= R9Bins.at(R9Bin)) && (R92 < R9Bins.at(R9Bin+1)) )
	  catEle2 = Form("eta%1.1f-%1.1f_R9%1.2f-%1.2f",etaBins.at(etaBin),etaBins.at(etaBin+1),R9Bins.at(R9Bin),R9Bins.at(R9Bin+1));

  for(unsigned int cat = 0; cat < categories.size(); ++cat)
  {
    if( categories.at(cat) == (catEle1 + "__" + catEle2) ) return catEle1 + "__" + catEle2;
    if( categories.at(cat) == (catEle2 + "__" + catEle1) ) return catEle2 + "__" + catEle1;
  }
  
  return "undefined__undefined";
}
