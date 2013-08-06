#ifndef ntpleUtils_h
#define ntpleUtils_h

#include "ConfigParser.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <cmath>
#include <algorithm>
#include <iomanip>

#include "TFile.h"
#include "TChain.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TVector3.h"
#include "TMath.h"

#include "Math/Vector4D.h"






/** get the number of events from a list of files */
std::map<int, int> GetTotalEvents(const std::string& histoName, const std::string& inputFileList);

/** fill a chain from a list of files */
bool FillChain(TChain* chain, const std::string& inputFileList);

/** get the parameters from a congiguration file */
int parseConfigFile (const TString& config);






/** define sort function */
bool maggiore(double i, double j);
bool   minore(double i, double j);

/** compute delta phi */
double deltaPhi(const double& phi1, const double& phi2);

/** compute delta eta */
double deltaEta(const double& eta1, const double& eta2);

/** compute delta R */
double deltaR(const double& eta1, const double& phi1,
              const double& eta2, const double& phi2);

/** print 4-vector */
void Print4V(const ROOT::Math::XYZTVector& p);

/** Do Kolmogorov test */
double DoKolmogorovTest(std::vector<double>& v1, std::vector<double>& v2);






/** Set binning of an histogram */
template <class T>
void MySetBins(T* h, std::vector<double>& bins)
{
  double* binEdges = new double[bins.size()];
  for(unsigned int i = 0; i < bins.size(); ++i)
    binEdges[i] = bins.at(i);
  
  h -> SetBins(bins.size()-1,binEdges);
}

/** Find bin number given bin edges */
int MyFindBin(const double& val, const std::vector<double>* binEdges);

/** Find bin number given bin size */
int MyFindBin(const double& val, const double& min, const double& max, const double& invWidth);

/** Interpolate a TGraph */
double MyEval(TGraph* g, const double& x);






/*** time sort a tree ***/
struct Sorter
{
  int time;
  int entry;
  
  bool operator() (const Sorter& s1, const Sorter& s2)
  {
    return s1.time < s2.time;
  }
};

/*** time sort a tree ***/
struct myEvent
{
  int runId;
  int timeStampHigh;

  int region;
  int regionSM;
  int regionLMR;
  
  float scE;
  float P;

  float scLaserCorr;
  float seedLaserAlpha;
  
  bool operator() (const myEvent& s1, const myEvent& s2)
  {
    return s1.timeStampHigh < s2.timeStampHigh;
  }
};

/*** time sort a tree ***/
struct SorterLC
{
  float laserCorr;
  int entry;
  
  bool operator() (const SorterLC& s1, const SorterLC& s2)
  {
    return s1.laserCorr < s2.laserCorr;
  }
};






/** define the map of all possible matching */
template <class T1, class T2>
std::map<float, std::pair<int, int> > MatchingDRMap(const std::vector<T1>& collection1,
                                                    const std::vector<T2>& collection2)
{
  // define map of all possible DRs
  typedef std::map<float, std::pair<int, int> > DRMap;
  DRMap myDRMap;
  myDRMap.clear();  
  // compute all DRs 
  for(unsigned int it1 = 0; it1 <  collection1.size(); ++it1)
  {
    for(unsigned int it2 = 0; it2 < collection2.size(); ++it2)
    {
      float DR = deltaR((collection1.at(it1)).Eta(), (collection1.at(it1)).Phi(),
                        (collection2.at(it2)).Eta(), (collection2.at(it2)).Phi());
      std::pair<int, int> dummy(it1, it2);
      myDRMap[DR] = dummy;
    }
  }
  return myDRMap;
}

/** define the map of all possible matching */
template <class T1, class T2>
int GetMatching(const std::vector<T1>& collection1, //---- RECO
                const std::vector<T2>& collection2, //---- MC
                const float& DRMax,
                float ptRatioMin,
                float ptRatioMax,
                std::vector<int>* matchIt1 = 0) //---- index from RECO that matches with MC

{
  // get the DR map between two collection of particles
  typedef std::map<float, std::pair<int, int> > DRMap;
  DRMap myDRMap = MatchingDRMap(collection1, collection2);
  
    // define boolean flags to avoid double usage of the same particle
  std::vector<bool> isUsed1;
  for(unsigned int i = 0; i < collection1.size(); ++ i)
    isUsed1.push_back(false);
  
  std::vector<bool> isUsed2;
  for(unsigned int i = 0; i < collection2.size(); ++ i)
    isUsed2.push_back(false);
  
  
  // intialize the vector which will store the result of the matching
  if(matchIt1 != 0)
  {
    (*matchIt1).clear();
    for(unsigned int i = 0; i < collection2.size(); ++i)
      (*matchIt1).push_back(-1);
  }
  
  // loop over the DRmap to get the smallest DR matchings
  unsigned int nMatching = 0;
  
  for(DRMap::const_iterator mapIt = myDRMap.begin();
      mapIt != myDRMap.end(); ++mapIt)
  {
    int it1 = (mapIt -> second).first;
    int it2 = (mapIt -> second).second;
    
    float DR = mapIt -> first;
    
    TVector3 momentum1(collection1.at(it1).Px(),
                       collection1.at(it1).Py(),
                       collection1.at(it1).Pz());
    TVector3 momentum2(collection2.at(it2).Px(),
                       collection2.at(it2).Py(),
                       collection2.at(it2).Pz());
    
    if( 1. * collection1.at(it1).Pt() / (collection2.at(it2)).Pt() < ptRatioMin) 
      continue;
    if( 1. * collection1.at(it1).Pt() / (collection2.at(it2)).Pt() > ptRatioMax) 
      continue;
    
    if( (DR <= DRMax) && (isUsed1.at(it1) == false) && (isUsed2.at(it2) == false) )
    {
      isUsed1.at(it1) = true; 
      isUsed2.at(it2) = true; 
      ++nMatching;
      
      if(matchIt1 != 0)
        (*matchIt1).at(it2) = it1;
    }
    
    if( (nMatching == collection2.size()) || (DR > DRMax) )
      break;    
  }
  
  return nMatching;
}









/** get electron flag/severity level */
bool GetElectronFlag(const std::string& flag);
bool GetElectronSeverityLevel(const std::string& SeverityLevel);

#endif
