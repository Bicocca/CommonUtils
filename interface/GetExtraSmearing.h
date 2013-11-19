#include "ParseIJazZFile.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <map>

#include "TH2F.h"


class Smearer
{
 public:
  
  //! ctor
  Smearer()
  {
    ShervinExtraSmearingMap = NULL;
    ShervinExtraSmearingMap_Err = NULL;
    ShervinEtExtraSmearingMap = NULL;
    ShervinEtExtraSmearingMap_Err = NULL;
    IJazZExtraSmearingHisto = NULL;
  }
  
  //! dtor
  ~Smearer()
  {
    delete ShervinExtraSmearingMap;
    delete ShervinExtraSmearingMap_Err;
    delete ShervinEtExtraSmearingMap;
    delete ShervinEtExtraSmearingMap_Err;
    delete IJazZExtraSmearingHisto;
  }
  
  //! methods
  void SetShervinExtraSmearingMap(const std::string& fileName);
  void SetShervinEtExtraSmearingMap(const std::string& fileName);
  void SetIJazZExtraSmearingHisto(const std::string& directory);
  
  float GetExtraSmearing(const float& scEta, const float& R9, const std::string& label, const std::string& version, const float& timesError);
  float GetEtExtraSmearing(const float& scEta, const float& R9, const std::string& label, const float& Et, const std::string& version, const float& timesError);


 private:

  std::map<std::string,float>* ShervinExtraSmearingMap;
  std::map<std::string,float>* ShervinExtraSmearingMap_Err;
  std::map<std::string,std::pair<float,float> >* ShervinEtExtraSmearingMap;
  std::map<std::string,std::pair<float,float> >* ShervinEtExtraSmearingMap_Err;
  TH2F* IJazZExtraSmearingHisto;
};



float Smearer::GetExtraSmearing(const float& scEta, const float& R9, const std::string& label, const std::string& version, const float& timesError)
{ 
  //2011  // from AN2011-426
  if( label == "2011" )
  {
    if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && R9 > 0.94) return 0.0089;
    if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && R9 < 0.94) return 0.0109;

    if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && R9 > 0.94) return 0.0156;
    if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && R9 < 0.94) return 0.0203;

    if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && R9 > 0.94) return 0.0303;
    if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && R9 < 0.94) return 0.0326;

    if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && R9 > 0.94) return 0.0318;
    if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && R9 < 0.94) return 0.0331;
  }
  
  
  //2012 
  if( version == "andrea" )
  {
    if( label == "Winter2013" )
    {
      //EB LR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && R9 < 0.94) return 0.008270;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && R9 < 0.94) return 0.014330;
      //EB HR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && R9 > 0.94) return 0.008757;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && R9 > 0.94) return 0.007113;
      //EE LR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && R9 < 0.94) return 0.017270;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && R9 < 0.94) return 0.022281;
      //EE HR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && R9 > 0.94) return 0.018933;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && R9 > 0.94) return 0.022353;
    }
    
    if( label == "Moriond2013" )
    {
      //EB LR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && R9 < 0.94) return 0.0100;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && R9 < 0.94) return 0.0172;
      //EB HR9
      if( fabs(scEta) < 1.479 && fabs(scEta) < 1. && R9 > 0.94) return 0.0104;
      if( fabs(scEta) < 1.479 && fabs(scEta) > 1. && R9 > 0.94) return 0.0157;
      //EE LR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && R9 < 0.94) return 0.0246;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && R9 < 0.94) return 0.0339;
      //EE HR9
      if( fabs(scEta) > 1.479 && fabs(scEta) < 2. && R9 > 0.94) return 0.0307;
      if( fabs(scEta) > 1.479 && fabs(scEta) > 2. && R9 > 0.94) return 0.0337;
    }
  }
  
  
  if( version == "shervin" && ShervinExtraSmearingMap == NULL )
    std::cout << ">>>>>> Smearer::ERROR: ShervinExtraSmearingMap not defined, returning 0. as extra-smearing" << std::endl;
  if( version == "shervin" && ShervinExtraSmearingMap != NULL )
  {
    // find eta/R9 bin
    std::string label = "";
    if( (fabs(scEta) >= 0.0000) && (fabs(scEta) < 1.0000) && (R9  < 0.94) ) label = "EBlowEtaBad";
    if( (fabs(scEta) >= 0.0000) && (fabs(scEta) < 1.0000) && (R9 >= 0.94) ) label = "EBlowEtaGold";
    if( (fabs(scEta) >= 1.0000) && (fabs(scEta) < 1.4442) && (R9  < 0.94) ) label = "EBhighEtaBad";
    if( (fabs(scEta) >= 1.0000) && (fabs(scEta) < 1.4442) && (R9 >= 0.94) ) label = "EBhighEtaGold";
    
    if( (fabs(scEta) >= 1.5660) && (fabs(scEta) < 2.0000) && (R9  < 0.94) ) label = "EElowEtaBad";
    if( (fabs(scEta) >= 1.5660) && (fabs(scEta) < 2.0000) && (R9 >= 0.94) ) label = "EElowEtaGold";
    if( (fabs(scEta) >= 2.0000) && (fabs(scEta) < 2.5000) && (R9  < 0.94) ) label = "EEhighEtaBad";
    if( (fabs(scEta) >= 2.0000) && (fabs(scEta) < 2.5000) && (R9 >= 0.94) ) label = "EEhighEtaGold";
    
    // get value
/*     std::cout << " >>> timesError = " << timesError << std::endl; */
/*     std::cout << " >>> (*ShervinExtraSmearingMap)[label] = " << (*ShervinExtraSmearingMap)[label] << std::endl; */
/*     std::cout << " >>> (*ShervinExtraSmearingMap_Err)[label] = " << (*ShervinExtraSmearingMap_Err)[label] << std::endl; */

    return ((*ShervinExtraSmearingMap)[label] + timesError * (*ShervinExtraSmearingMap_Err)[label]);
  }
  
  
  if( version == "fabrice" && IJazZExtraSmearingHisto == NULL )
    std::cout << ">>>>>> ScaleCorrector::ERROR: IJazZExtraSmearingHisto not defined, returning 0. as extra-smearing" << std::endl;
  if( version == "fabrice" && IJazZExtraSmearingHisto != NULL )
  {
    // find eta/R9 bin
    if( IJazZExtraSmearingHisto != NULL )
    {
      int etaBin = IJazZExtraSmearingHisto -> GetXaxis() -> FindBin(fabs(scEta));
      int R9Bin  = IJazZExtraSmearingHisto -> GetYaxis() -> FindBin(fabs(R9));
      return IJazZExtraSmearingHisto -> GetBinContent(etaBin,R9Bin);
    }
  }
  
  
  return 0.;
};


float Smearer::GetEtExtraSmearing(const float& scEta, const float& R9, const std::string& label, const float& Et, const std::string& version, const float& timesError)
{ 
  if( version == "shervin" && ShervinEtExtraSmearingMap == NULL )
    std::cout << ">>>>>> Smearer::ERROR: ShervinEtExtraSmearingMap not defined, returning 0. as extra-smearing" << std::endl;
  if( version == "shervin" && ShervinEtExtraSmearingMap != NULL )
  {
    // find eta/R9 bin
    std::string label = "";
    float smearValue = 0.;
    float smearValueErr = 0.;

    if( (fabs(scEta) >= 0.0000) && (fabs(scEta) < 1.0000) && (R9  < 0.94) ) {label = "EBlowEtaBad"; smearValue = 0.0086; smearValueErr = 0.000181;}
    if( (fabs(scEta) >= 0.0000) && (fabs(scEta) < 1.0000) && (R9 >= 0.94) ) {label = "EBlowEtaGold"; smearValue = 0.0075; smearValueErr = 0.000300;}
    if( (fabs(scEta) >= 1.0000) && (fabs(scEta) < 1.4442) && (R9  < 0.94) ) {label = "EBhighEtaBad"; smearValue = 0.0188; smearValueErr = 0.000188;}
    if( (fabs(scEta) >= 1.0000) && (fabs(scEta) < 1.4442) && (R9 >= 0.94) ) {label = "EBhighEtaGold"; smearValue = 0.0122; smearValueErr = 0.000992;}

    if( (fabs(scEta) >= 1.5660) && (fabs(scEta) < 2.0000) && (R9  < 0.94) ) label = "EElowEtaBad";
    if( (fabs(scEta) >= 1.5660) && (fabs(scEta) < 2.0000) && (R9 >= 0.94) ) label = "EElowEtaGold";
    if( (fabs(scEta) >= 2.0000) && (fabs(scEta) < 2.5000) && (R9  < 0.94) ) label = "EEhighEtaBad";
    if( (fabs(scEta) >= 2.0000) && (fabs(scEta) < 2.5000) && (R9 >= 0.94) ) label = "EEhighEtaGold";
    
    //    if(fabs(scEta) >= 1.5660) label = "EE";
    
    // get value
    /*     std::cout << " >>> timesError = " << timesError << std::endl; */
    
    /*      std::cout << " >>> Et = " << Et << " label = " << label << std::endl;   */
    /*      std::cout << " >>> smearValue = " << smearValue << std::endl;   */
    /*      std::cout << " >>> smearValueErr = " << smearValueErr << std::endl;   */

    /*      if(label == "EE"){ */
    /*        if( (fabs(scEta) >= 1.5660) && (fabs(scEta) < 2.0000) && (R9  < 0.94) ) {smearValue = 0.0086; smearValueErr = 0.000181;} */
    /*        if( (fabs(scEta) >= 1.5660) && (fabs(scEta) < 2.0000) && (R9 >= 0.94) ) {smearValue = 0.0075; smearValueErr = 0.000300;} */
    /*        if( (fabs(scEta) >= 2.0000) && (fabs(scEta) < 2.5000) && (R9  < 0.94) ) {smearValue = 0.0188; smearValueErr = 0.000188;} */
    /*        if( (fabs(scEta) >= 2.0000) && (fabs(scEta) < 2.5000) && (R9 >= 0.94) ) {smearValue = 0.0122; smearValueErr = 0.000992;} */
    /*      } */

    if(label != "EElowEtaBad" && label != "EElowEtaGold" && label != "EEhighEtaBad" && label != "EEhighEtaGold" && Et >= 100.) 
      return smearValue + timesError * smearValueErr;
    //    else if(label == "EE" ) return smearValue + timesError * smearValueErr;
    else {

      /*
       float DS = ((*ShervinEtExtraSmearingMap)[label]).first; 
       float DS_Err = ((*ShervinEtExtraSmearingMap_Err)[label]).first; 
       float DC = ((*ShervinEtExtraSmearingMap)[label]).second;
       float DC_Err = ((*ShervinEtExtraSmearingMap_Err)[label]).second; 
       smearValue = sqrt(pow( DS/sqrt(Et),2.) + pow(DC, 2.)); 
       smearValueErr = 1./fabs(smearValue) * sqrt(pow( (DS/sqrt(Et)*DS_Err), 2.) + pow( (DC*DC_Err), 2.) ); 
       return smearValue +timesError * smearValueErr;
      */

      float DS = ((*ShervinEtExtraSmearingMap)[label]).first; 
      float DS_Err = ((*ShervinEtExtraSmearingMap_Err)[label]).first; 
      float DC = ((*ShervinEtExtraSmearingMap)[label]).second; 
      float DC_Err = ((*ShervinEtExtraSmearingMap_Err)[label]).second; 
      smearValue = sqrt(pow( ( float(DS + DS_Err * float(timesError))/sqrt(Et)),2.) + pow( float(DC + float(timesError) * DC_Err), 2.) ); 
      //      smearValueErr = 1./fabs(smearValue) * sqrt(pow( (DS/sqrt(Et)*DS_Err), 2.) + pow( (DC*DC_Err), 2.) );
      return smearValue;

/*      std::cout << " >>> smearValue = " << smearValue << std::endl;   */
/*      std::cout << " >>> smearValueErr = " << smearValueErr << std::endl;   */


    }
  }
  return 0.;
};

void Smearer::SetShervinExtraSmearingMap(const std::string& fileName)
{
  std::cout << ">>>>>> Smearer::SetShervinExtraSmearingMap begin" << std::endl;
  
  std::map<std::string,float>* MapOfSmearings = new std::map<std::string,float>;
  std::map<std::string,float>* MapOfSmearings_Err = new std::map<std::string,float>;

  //fill the map with file data
  std::string evtType;
  float smear;
  float errSmear;

  std::ifstream inFile(fileName.c_str(),std::ios::in);
  while(1)
  {
    inFile >> evtType >> smear >> errSmear;
    if( !inFile.good() ) break;
    
    (*MapOfSmearings)[evtType] = smear;
    (*MapOfSmearings_Err)[evtType] = errSmear;
  }
  inFile.close();
  
  ShervinExtraSmearingMap = MapOfSmearings;
  ShervinExtraSmearingMap_Err = MapOfSmearings_Err;
  
  std::cout << ">>>>>> Smearer::SetShervinExtraSmearingMap end" << std::endl;
}


void Smearer::SetShervinEtExtraSmearingMap(const std::string& fileName)
{
  std::cout << ">>>>>> Smearer::SetShervinEtExtraSmearingMap begin" << std::endl;
  
  std::map<std::string,std::pair<float,float> >* MapOfSmearings = new std::map<std::string,std::pair<float,float> >;
  std::map<std::string,std::pair<float,float> >* MapOfSmearings_Err = new std::map<std::string,std::pair<float,float> >;

  //fill the map with file data
  std::string evtType;
  float stocatic;
  float stocasticErr;
  float constant;
  float constantErr;

  std::ifstream inFile(fileName.c_str(),std::ios::in);
  while(1)
  {
    inFile >> evtType >> stocatic >> stocasticErr >> constant >> constantErr;
    if( !inFile.good() ) break;
    
    std::pair<float,float> dummy(stocatic,constant);
    std::pair<float,float> dummyErr(stocasticErr,constantErr);
    (*MapOfSmearings)[evtType] = dummy;
    (*MapOfSmearings_Err)[evtType] = dummyErr;
  }
  inFile.close();
  
  ShervinEtExtraSmearingMap = MapOfSmearings;
  ShervinEtExtraSmearingMap_Err = MapOfSmearings_Err;
  
  std::cout << ">>>>>> Smearer::SetShervinEtExtraSmearingMap end" << std::endl;
}



void Smearer::SetIJazZExtraSmearingHisto(const std::string& directory)
{
  std::cout << ">>>>>> Smearer::SetIJazZExtraSmearingHisto begin" << std::endl;
  
  // open files
  TH2F* histoResoDA = ParseIJazZFile(directory+"/0-999999/IJazZ_EE_Data_ResoHgg_Iter1.root.fittedReso");
  
  std::vector<TH2F*> histoResoMC;
  histoResoMC.push_back( ParseIJazZFile(directory+"/0-999999/IJazZ_EE_MC_ResoHgg_Iter3.root.fittedReso") );
  histoResoMC.push_back( ParseIJazZFile(directory+"/0-999999/IJazZ_EE_MC_ResoHgg_Iter2.root.fittedReso") );
  histoResoMC.push_back( ParseIJazZFile(directory+"/0-999999/IJazZ_EE_MC_ResoHgg_Iter1.root.fittedReso") );
  
  TH2F* histoExtraSmearing = (TH2F*)( histoResoDA->Clone() );
  
  for(int binR9 = 1; binR9 <= histoResoDA->GetNbinsY(); ++binR9)
  {
    for(int binEta = 1; binEta <= histoResoDA->GetNbinsX(); ++binEta)
    {
      double y1, y2;
      float es2 = 0;
      y1 = histoResoDA -> GetBinContent(binEta,binR9);
      
      for(int imc = int(histoResoMC.size()-1); imc >= 0; --imc)
      {
        y2 = histoResoMC[imc] -> GetBinContent(binEta,binR9);
        es2 += y1*y1 - y2*y2;
      }
      
      if( es2 > 0 )  es2 = +sqrt(fabs(es2));
      else           es2 = -sqrt(fabs(es2));
      
      histoExtraSmearing -> SetBinContent(binEta,binR9,es2);
    }
  }
  
  IJazZExtraSmearingHisto = histoExtraSmearing;
  
  std::cout << ">>>>>> Smearer::SetIJazZExtraSmearingHisto begin" << std::endl;
}
