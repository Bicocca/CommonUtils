#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>


float GetSmearings(const float& scEta, const float& r9, const std::string& label, const std::string& version)
{ 
  float smear = 1.;

  //2012 
  if( label == "Winter2013" )
  {
    if( version == "shervin" )
    {
      //EB LR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && r9 < 0.94) smear = 0.0107;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && r9 < 0.94) smear = 0.0194;
      //EB HR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && r9 > 0.94) smear = 0.0111;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && r9 > 0.94) smear = 0.0155;
      //EE LR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && r9 < 0.94) smear = 0.0276;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && r9 < 0.94) smear = 0.0371;
      //EE HR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && r9 > 0.94) smear = 0.0295;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && r9 > 0.94) smear = 0.0370;
    }
    if( version == "fabrice" )
    {
      //EB LR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && r9 < 0.94) smear = 0.0086;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && r9 < 0.94) smear = 0.0145;
      //EB HR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && r9 > 0.94) smear = 0.0090;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && r9 > 0.94) smear = 0.0109;
      //EE LR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && r9 < 0.94) smear = 0.0179;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && r9 < 0.94) smear = 0.0212;
      //EE HR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && r9 > 0.94) smear = 0.0176;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && r9 > 0.94) smear = 0.0231;
    }
    if( version == "andrea" )
    {
      //EB LR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && r9 < 0.94) smear = 0.008270;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && r9 < 0.94) smear = 0.014330;
      //EB HR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && r9 > 0.94) smear = 0.008757;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && r9 > 0.94) smear = 0.007113;
      //EE LR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && r9 < 0.94) smear = 0.017270;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && r9 < 0.94) smear = 0.022281;
      //EE HR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && r9 > 0.94) smear = 0.018933;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && r9 > 0.94) smear = 0.022353;
    }
  }
  
  //2012 
  if( label == "Moriond2013" )
  {
    if( version == "shervin" )
    {
      //EB LR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && r9 < 0.94) smear = 0.0107;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && r9 < 0.94) smear = 0.0194;
      //EB HR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && r9 > 0.94) smear = 0.0111;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && r9 > 0.94) smear = 0.0155;
      //EE LR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && r9 < 0.94) smear = 0.0276;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && r9 < 0.94) smear = 0.0371;
      //EE HR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && r9 > 0.94) smear = 0.0295;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && r9 > 0.94) smear = 0.0370;
    }
    if( version == "fabrice" )
    {
      //EB LR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && r9 < 0.94) smear = 0.0114;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && r9 < 0.94) smear = 0.0155;
      //EB HR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && r9 > 0.94) smear = 0.0119;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && r9 > 0.94) smear = 0.0160;
      //EE LR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && r9 < 0.94) smear = 0.0235;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && r9 < 0.94) smear = 0.0326;
      //EE HR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && r9 > 0.94) smear = 0.0272;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && r9 > 0.94) smear = 0.0337;
    }
    if( version == "andrea" )
    {
      //EB LR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && r9 < 0.94) smear = 0.0100;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && r9 < 0.94) smear = 0.0172;
      //EB HR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && r9 > 0.94) smear = 0.0104;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && r9 > 0.94) smear = 0.0157;
      //EE LR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && r9 < 0.94) smear = 0.0246;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && r9 < 0.94) smear = 0.0339;
      //EE HR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && r9 > 0.94) smear = 0.0307;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && r9 > 0.94) smear = 0.0337;
    }
  }
  
  //2011  // from AN2011-426
  if( label == "2011" )
  {
    if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && r9 > 0.94) smear = 0.0089;
    if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && r9 < 0.94) smear = 0.0109;

    if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && r9 > 0.94) smear = 0.0156;
    if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && r9 < 0.94) smear = 0.0203;

    if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && r9 > 0.94) smear = 0.0303;
    if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && r9 < 0.94) smear = 0.0326;

    if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && r9 > 0.94) smear = 0.0318;
    if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && r9 < 0.94) smear = 0.0331;
  }
  
  return smear;
};
