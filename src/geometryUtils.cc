#include "geometryUtils.h"



// --- EE ---
// default constructor, reading the map from file                                                                                                                          
TEndcapRegions::TEndcapRegions()
{
  //initializing the matrix
  for(int ii=0; ii<100; ++ii)
    for(int jj=0; jj<100; ++jj)
    {
      iEndcapRing[ii][jj][0] = -1;
      iEndcapRing[ii][jj][1] = -1;
      iLMR[ii][jj][0] = -1;
      iLMR[ii][jj][1] = -1;
    }
  
  FILE *fRing;
  std::string COMMONUTILS(getenv("COMMONUTILS"));
  std::string fileName = COMMONUTILS + "/data/eerings.dat";
  fRing = fopen(fileName.c_str(),"r");
  std::cout << "Inizializing endcap geometry from file " << fileName << std::endl;
  
  int ix,iy,iz,ir,lmr;
  while(fscanf(fRing,"(%d,%d,%d) %d %d\n",&ix,&iy,&iz,&lmr,&ir) !=EOF )
  {
    if( iz<0 ) iz = 0;
    iEndcapRing[ix-1][iy-1][iz] = ir;
    iLMR[ix-1][iy-1][iz] = lmr;
  }
  
  return;
}


// dtor
TEndcapRegions::~TEndcapRegions() { return; }


//methods
int TEndcapRegions::GetNRegions(const std::string& type)
{
  if( type == "ALL" )  return 1;
  if( type == "RING" ) return 24;
  if( type == "LMR" )  return 38;

  return -1;
}

int TEndcapRegions::GetRegionId(const int ix, const int iy, const int iz, const std::string& type)
{
  int iSide = iz;
  if( iSide < 0 ) iSide=0;
  if( type == "LMR" )
  {
    return iLMR[ix-1][iy-1][iSide] + 19*iSide;
  }
  
  return -1;
}

int TEndcapRegions::GetEndcapRing(const int ix, const int iy, const int iz, const int nRings, const std::string& type)
{
  if( type == "ALL" )
  {
    return 0;
  }
  
  int iSide = iz;
  if( iSide < 0 ) iSide = 0;
  float ringWidth = 39./nRings;
  
  if(iEndcapRing[ix-1][iy-1][iSide] == -1) return -1;
  else return int(((float)iEndcapRing[ix-1][iy-1][iSide]/(float)ringWidth)) + nRings*iSide;
}

int TEndcapRegions::GetEndcapIphi(int ix, int iy, int iz)
{
  double iX = ix-50.;
  double iY = iy-50.;
  int iPhi;
  if( iY >= 0 ) iPhi =  90 + int( TMath::ATan(iY/iX)*360. / (2.*TMath::Pi()) );
  else          iPhi = 270 + int( TMath::ATan(iY/iX)*360. / (2.*TMath::Pi()) );

  return iPhi;
}



// --- EB ---
// default constructor
TBarrelRegions::TBarrelRegions()
{
  //initializing the matrix
  for(int ii=0; ii<171; ++ii)
    for(int kk=0; kk<360; ++kk)
    {
      iLMR[ii][kk] = -1;
      iSM[ii][kk] = -1;
      iTT[ii][kk] = -1;
    }
  
  FILE *fRegion;
  std::string COMMONUTILS(getenv("COMMONUTILS"));
  std::string fileName = COMMONUTILS + "/data/ebregions.dat";
  fRegion = fopen(fileName.c_str(),"r");
  std::cout << "Inizializing endcap geometry from file " << fileName << std::endl;
  
  int ieta,iphi,sm,tt,lmr;
  std::map<int,int> dummyMap;
  while(fscanf(fRegion,"(%d ,%d) %d %d %d\n",&ieta,&iphi,&sm,&tt,&lmr) !=EOF ) 
  {
    iLMR[ieta+85][iphi-1] = lmr;
    iSM[ieta+85][iphi-1] = sm;
    iTT[ieta+85][iphi-1] = tt;
    
    //inverse map containing the ietaMin in that region
    if(dummyMap.find(lmr) == dummyMap.end()) dummyMap[lmr] = ieta;
    else if(abs(ieta) < dummyMap[lmr]) dummyMap[lmr] = ieta;
  }
  
  int counter=0;
  std::map<int,int> dummyMapEtaCount;
  for(std::map<int,int>::const_iterator itr = dummyMap.begin(); itr != dummyMap.end(); ++itr)
  {
    if(dummyMapEtaCount.find(itr->second) == dummyMapEtaCount.end()) 
    {
      dummyMapEtaCount[itr->second] = counter;
      ++counter;
    }
    LmrEtaMap[itr->first] = dummyMapEtaCount[itr->second];
  }
  
  return;
}


//dtor                                                                                                                                                                     
TBarrelRegions::~TBarrelRegions() { return; }


//methods
int TBarrelRegions::GetNRegions(const std::string& type)
{
  if( type == "ALL" ) return 1;
  if( type == "SM" )  return 36;
  if( type == "LMR" ) return 324;
  if( type == "TT" )  return 2448;

  return -1;
}

int TBarrelRegions::GetRegionId(const int iEta, const int iPhi, const std::string& type)
{
  if( (iEta < -85) || (iEta == 0) || (iEta > 85) ) return -1;
  if( (iPhi < 1) || (iPhi > 360) ) return -1;
  
  if( type == "ALL" ) return 0;
  if( type == "SM"  ) return iSM[iEta+85][iPhi-1];
  if( type == "LMR" ) return iLMR[iEta+85][iPhi-1];
  if( type == "TT"  ) return iTT[iEta+85][iPhi-1];
  
  return -1;
}

int TBarrelRegions::GetNRegionsIeta(const std::string& type)
{
  if( type == "SM"  ) return 1;
  if( type == "LMR" ) return 10;
  if( type == "TT"  ) return 17;

  return -1;
}

int TBarrelRegions::GetRegionIdIeta(const int& regionId, const std::string& type)
{
  if( type == "LMR" ) return LmrEtaMap[regionId];
  if( type == "SM" )  return 0;
  
  return -1;
}
