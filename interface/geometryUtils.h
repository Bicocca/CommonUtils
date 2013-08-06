#ifndef geometryUtils_h
#define geometryUtils_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include "TMath.h"



/// EE Geometry
class TEndcapRegions
{
 private:
  int iEndcapRing[100][100][2]; 
  int iLMR[100][100][2];
  
 public:
  TEndcapRegions(); 
  virtual ~TEndcapRegions();
  int GetNRegions(const std::string& type); 
  int GetRegionId(const int,const int,const int,const std::string&);
  int GetEndcapRing(const int,const int,const int,const int, const std::string&);
  int GetEndcapIeta(const int,const int,const int);
  int GetEndcapIphi(const int,const int,const int);
};



/// EB Geometry
class TBarrelRegions
{
 private:
  std::map<int,int> LmrEtaMap;
  int iLMR[171][360];
  int iTT[171][360];
  int iSM[171][360];
  
 public:
  TBarrelRegions();
  virtual ~TBarrelRegions();
  int GetNRegions(const std::string& type); 
  int GetNRegionsIeta(const std::string& type); 
  int GetRegionId(const int,const int,const std::string&);
  int GetRegionIdIeta(const int& regionId, const std::string& type);
};

#endif
