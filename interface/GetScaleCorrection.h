#include "ParseIJazZFile.h"

#include <iostream>
#include <fstream>
#include <utility>
#include <map>

#include "TH2F.h"



class ScaleCorrector
{
 public:
  
  //! ctor
  ScaleCorrector(const std::string& runRangeFileName)
  {
    ShervinRunDepScaleMap = NULL;
    ShervinRunDepScaleMap_Err = NULL;
    IJazZGlobalScaleHisto = NULL;
    IJazZRunDepScaleHistoMap = NULL;
    
    SetRunRangeMap(runRangeFileName);
  }
  
  //! dtor
  ~ScaleCorrector()
  {
    delete ShervinRunDepScaleMap;
    delete ShervinRunDepScaleMap_Err;
    delete IJazZGlobalScaleHisto;
    delete IJazZRunDepScaleHistoMap;  
  }
  
  //! methods
  void SetRunRangeMap(const std::string& inFileName);
  
  void SetShervinRunDepScaleMap(const std::string& fileName);
  void SetIJazZGlobalScaleHisto(const std::string& directory);
  void SetIJazZRunDepScaleHistoMap(const std::string& directory,
                                   const std::string& fileName = "IJazZ_EE_Data_EscaleHgg-runDependent-eleRegr-coarse.root.fittedResp.etaScaleDataOverMC");
  
  float GetScaleCorrection(const float& scEta, const float& R9, const int& runId, 
			   const std::string& label, const std::string& version, const float& timesError);
  
  
 private:
  
  std::map<std::string,std::pair<int,int> > runRangeMap;
  
  std::map<std::pair<std::string,std::pair<int,int> >,float>* ShervinRunDepScaleMap;
  std::map<std::pair<std::string,std::pair<int,int> >,float>* ShervinRunDepScaleMap_Err;
  TH2F* IJazZGlobalScaleHisto;
  std::map<std::string,TH2F*>* IJazZRunDepScaleHistoMap;
};



void ScaleCorrector::SetRunRangeMap(const std::string& inFileName)
{
  std::ifstream inFile(inFileName.c_str(),std::ios::in);
  while(1)
  {
    std::string runRange, buffer1, buffer2;
    inFile >> runRange >> buffer1 >> buffer2;
    if( !inFile.good() ) break;
    
    std::stringstream ss(runRange);
    std::string runMin, runMax;
    getline(ss,runMin,'-');
    getline(ss,runMax,'-');
    std::pair<int,int> dummy (atoi(runMin.c_str()),atoi(runMax.c_str()));
    runRangeMap[runRange] = dummy;
  }  
  
  inFile.close();
}



float ScaleCorrector::GetScaleCorrection(const float& scEta, const float& R9, const int& runId, 
					 const std::string& label, const std::string& version, const float& timesError)
{
  //read run range map
  std::string runRange = "";
  int runMin = -1;
  int runMax = -1;
  std::map<std::string,std::pair<int,int> >::const_iterator mapIt;
  for(mapIt = runRangeMap.begin(); mapIt != runRangeMap.end(); ++mapIt)
  {
    if( (runId >= (mapIt->second).first) && (runId <= (mapIt->second).second) )
    {
      runRange = (mapIt->first);
      runMin   = (mapIt->second).first;
      runMax   = (mapIt->second).second;
      break;
    }
  }  
  
  
  if( version == "andrea" )
  {
    if( label == "Winter2013" )
    {
      if( runId >= 190645 && runId <= 208686 )
      {
        if( (fabs(scEta) >= 0.0000) && (fabs(scEta) < 1.0000) && (R9 > 0.94) ) return 1./1.0076;
        if( (fabs(scEta) >= 1.0000) && (fabs(scEta) < 1.4442) && (R9 > 0.94) ) return 1./1.0101;
        if( (fabs(scEta) >= 0.0000) && (fabs(scEta) < 1.0000) && (R9 < 0.94) ) return 1./1.0036;
        if( (fabs(scEta) >= 1.0000) && (fabs(scEta) < 1.4442) && (R9 < 0.94) ) return 1./0.9986;
        
        if( (fabs(scEta) >= 1.5660) && (fabs(scEta) < 2.0000) && (R9 > 0.94) ) return 1./1.0086;
        if( (fabs(scEta) >= 2.0000) && (fabs(scEta) < 2.5000) && (R9 > 0.94) ) return 1./1.0118;
        if( (fabs(scEta) >= 1.5660) && (fabs(scEta) < 2.0000) && (R9 < 0.94) ) return 1./1.0038;
        if( (fabs(scEta) >= 2.0000) && (fabs(scEta) < 2.5000) && (R9 < 0.94) ) return 1./1.0053;
      }
    }
  }
  
  
  if( version == "shervin" && ShervinRunDepScaleMap == NULL )
    std::cout << ">>>>>> ScaleCorrector::ERROR: ShervinRunDepScaleMap not defined, returning 1. as correction" << std::endl;
  if( version == "shervin" && ShervinRunDepScaleMap != NULL )
  {
    // find eta/R9 bin
    std::string label = "";
    if( (fabs(scEta) >= 0.0000) && (fabs(scEta) < 1.0000) && (R9  < 0.94) ) label = "EB-absEta_0_1-bad";
    if( (fabs(scEta) >= 0.0000) && (fabs(scEta) < 1.0000) && (R9 >= 0.94) ) label = "EB-absEta_0_1-gold";
    if( (fabs(scEta) >= 1.0000) && (fabs(scEta) < 1.4442) && (R9  < 0.94) ) label = "EB-absEta_1_1.4442-bad";
    if( (fabs(scEta) >= 1.0000) && (fabs(scEta) < 1.4442) && (R9 >= 0.94) ) label = "EB-absEta_1_1.4442-gold";
    
    if( (fabs(scEta) >= 1.5660) && (fabs(scEta) < 2.0000) && (R9  < 0.94) ) label = "EE-absEta_1.566_2-bad";
    if( (fabs(scEta) >= 1.5660) && (fabs(scEta) < 2.0000) && (R9 >= 0.94) ) label = "EE-absEta_1.566_2-gold";
    if( (fabs(scEta) >= 2.0000) && (fabs(scEta) < 2.5000) && (R9  < 0.94) ) label = "EE-absEta_2_2.5-bad";
    if( (fabs(scEta) >= 2.0000) && (fabs(scEta) < 2.5000) && (R9 >= 0.94) ) label = "EE-absEta_2_2.5-gold";
    
    // get value
    std::pair<int,int> dummy1(runMin,runMax);
    std::pair<std::string,std::pair<int,int> > dummy2(label,dummy1);
    
    return ((*ShervinRunDepScaleMap)[dummy2] + timesError * (*ShervinRunDepScaleMap_Err)[dummy2]);
  }
  
  
  if( version == "fabrice" && IJazZGlobalScaleHisto == NULL )
    std::cout << ">>>>>> ScaleCorrector::ERROR: IJazZGlobalScaleHisto not defined, returning 1. as correction" << std::endl;
  if( version == "fabrice" && IJazZRunDepScaleHistoMap == NULL )
    std::cout << ">>>>>> ScaleCorrector::ERROR: IJazZRunDepScaleHistoMap not defined, returning 1. as correction" << std::endl;
  if( version == "fabrice" )
  {
    float scaleCorr = 1.;
    
    // find eta/R9 bin
    if( IJazZGlobalScaleHisto != NULL )
    {
      int etaBin = IJazZGlobalScaleHisto -> GetXaxis() -> FindBin(fabs(scEta));
      int R9Bin  = IJazZGlobalScaleHisto -> GetYaxis() -> FindBin(fabs(R9));
      scaleCorr *= IJazZGlobalScaleHisto -> GetBinContent(etaBin,R9Bin);
    }
    
    if( IJazZRunDepScaleHistoMap != NULL )
    {
      int etaBin = (*IJazZRunDepScaleHistoMap)[runRange] -> GetXaxis() -> FindBin(fabs(scEta));
      int R9Bin  = (*IJazZRunDepScaleHistoMap)[runRange] -> GetYaxis() -> FindBin(fabs(R9));
      scaleCorr /= (*IJazZRunDepScaleHistoMap)[runRange] -> GetBinContent(etaBin,R9Bin);
    }
    
    return scaleCorr;
  }
  
  
  return 1.;
};






void ScaleCorrector::SetShervinRunDepScaleMap(const std::string& fileName)
{
  std::cout << ">>>>>> ScaleCorrector::SetShervinRunDepScaleMap begin" << std::endl;
  
  std::map<std::pair<std::string,std::pair<int,int> >,float>* MapOfScales = new std::map<std::pair<std::string,std::pair<int,int> >,float>;
  std::map<std::pair<std::string,std::pair<int,int> >,float>* MapOfScales_Err = new std::map<std::pair<std::string,std::pair<int,int> >,float>;
  
  //fill the map with file data
  std::string evtType;
  std::string dummy;
  int runMin;
  int runMax;
  float scale;
  float errScale;

  std::ifstream inFile(fileName.c_str(),std::ios::in);
  while(1)
  {
     inFile >> evtType >> dummy >> runMin >> runMax >> scale >> errScale;
     if( !inFile.good() ) break;
     
     std::pair<int,int> range(runMin,runMax);
     std::pair<std::string,std::pair <int,int> > typeRange(evtType,range);
     (*MapOfScales)[typeRange] = scale;
     (*MapOfScales_Err)[typeRange] = errScale;
  }
  inFile.close();
  
  ShervinRunDepScaleMap = MapOfScales;
  ShervinRunDepScaleMap_Err = MapOfScales_Err;
  
  std::cout << ">>>>>> ScaleCorrector::SetShervinRunDepScaleMap end" << std::endl;
}



void ScaleCorrector::SetIJazZGlobalScaleHisto(const std::string& directory)
{
  std::cout << ">>>>>> ScaleCorrector::SetIJazZGlobalScaleHisto begin" << std::endl;
  
  TH2F* histoRespDA = ParseIJazZFile(directory+"/0-999999/IJazZ_EE_Data_ResoHgg_Iter1.root.fittedResp");
  TH2F* histoRespMC = ParseIJazZFile(directory+"/0-999999/IJazZ_EE_MC_ResoHgg_Iter3.root.fittedResp");

  TH2F* histoRespRatio = (TH2F*)( histoRespMC->Clone() );
  histoRespRatio -> Divide(histoRespDA);

  IJazZGlobalScaleHisto = histoRespRatio;
 
   std::cout << ">>>>>> ScaleCorrector::SetIJazZGlobalScaleHisto end" << std::endl;
}



void ScaleCorrector::SetIJazZRunDepScaleHistoMap(const std::string& directory, const std::string& fileName)
{
  std::cout << ">>>>>> ScaleCorrector::SetIJazZRunDepScaleHistoMap begin" << std::endl;
  
  std::map<std::string,TH2F*>* histoRespRatio = new std::map<std::string,TH2F*>;
  
  std::map<std::string,std::pair<int,int> >::const_iterator mapIt;
  for(mapIt = runRangeMap.begin(); mapIt != runRangeMap.end(); ++mapIt)
  {
    std::string runRange = mapIt->first;
    (*histoRespRatio)[runRange] = ParseIJazZFile(Form("%s/%s/%s",directory.c_str(),runRange.c_str(),fileName.c_str()));
  }
  
  IJazZRunDepScaleHistoMap = histoRespRatio;
  
  std::cout << ">>>>>> ScaleCorrector::SetIJazZRunDepScaleHistoMap end" << std::endl;
}
