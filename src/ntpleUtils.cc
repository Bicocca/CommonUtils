#include "ntpleUtils.h"



std::map<int, int> GetTotalEvents(const std::string& histoName, const std::string& inputFileList)
{
  std::ifstream inFile(inputFileList.c_str());
  std::string buffer;
  std::map<int, int> totalEvents;
  
  if(!inFile.is_open())
  {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return totalEvents;
  }
  
  while(1)
  {
    inFile >> buffer;
    if(!inFile.good()) break;

    TFile* f = TFile::Open(buffer.c_str());
    TH1F* histo = NULL;
    f -> GetObject(histoName.c_str(), histo);
    if(histo == NULL)
    {
      std::cout << ">>>ntpleUtils::Error in getting object " << histoName << std::endl;
      exit(-1);
    }
    
    
    for(int bin = 1; bin <= histo -> GetNbinsX(); ++bin)
      totalEvents[bin] += int(histo -> GetBinContent(bin));
    
    f -> Close();
    
    delete f;
  }

  return totalEvents;
}

//  ------------------------------------------------------------


bool FillChain(TChain* chain, const std::string& inputFileList)
{
  std::ifstream inFile(inputFileList.c_str());
  std::string buffer;
  std::string bufferbeg;
  
  if(!inFile.is_open())
  {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return false;
  }
  
  while(1)
  {
    inFile >> buffer;
    bufferbeg = buffer.substr(0,1);    
    
    if(!inFile.good()) break;
    if( bufferbeg == "#" ) continue;
    
    chain -> Add(buffer.c_str());
    //std::cout << ">>> ntupleUtils::FillChain - treeName = " << chain.GetName() << " from file " << buffer << std::endl;
  }

  return true;
}

//  ------------------------------------------------------------

int parseConfigFile (const TString& config)
{
  std::cout << ">>> Parsing " << config << " file" << std::endl ;
  
  if( gConfigParser ) delete gConfigParser;
  gConfigParser = new ConfigParser();
  
  if( !(gConfigParser -> init(config)) )
  {
    std::cout << ">>> parseConfigFile::Could not open configuration file "
              << config << std::endl;
     return -1;
  }
  
  //gConfigParser -> print();
  
  return 0 ;
}

//  ------------------------------------------------------------






bool maggiore(double i, double j)
{
  return i > j;
}

bool  minore(double i, double j)
{
 return i < j;
}

double deltaPhi(const double& phi1, const double& phi2)
{ 
  double deltaphi = fabs(phi1 - phi2);
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi;
  return deltaphi;
}

//  ------------------------------------------------------------

double deltaEta(const double& eta1, const double& eta2)
{ 
  double deltaeta = fabs(eta1 - eta2);
  return deltaeta;
}

//  ------------------------------------------------------------

double deltaR(const double& eta1, const double& phi1,
              const double& eta2, const double& phi2)
{ 
 double deltaphi = deltaPhi(phi1, phi2);
 double deltaeta = deltaEta(eta1, eta2);
 double deltar = sqrt(deltaphi*deltaphi + deltaeta*deltaeta);
 return deltar;
}

//  ------------------------------------------------------------

void Print4V(const ROOT::Math::XYZTVector& p)
{
  std::cout << std::fixed << std::setprecision(2)
            << "(E,px,py,pz) = " 
            << "(" << std::setw(8) << p.energy() 
            << "," << std::setw(8) << p.px()
            << "," << std::setw(8) << p.py()
            << "," << std::setw(8) << p.pz()
            << ")" 
            << "\tpt = " 
            << std::setw(7) << p.pt()
            << "\teta = " 
            << std::setw(6) << p.eta()
            << "\tphi = " 
            << std::setw(5) << p.phi()
            << std::endl;
}

//  ------------------------------------------------------------

double DoKolmogorovTest(std::vector<double>& v1, std::vector<double>& v2)
{
  std::sort(v1.begin(),v1.end());
  std::sort(v2.begin(),v2.end());

  double* a1 = new double[v1.size()];
  double* a2 = new double[v2.size()];

  for(unsigned int i = 0; i < v1.size(); ++i) a1[i] = v1.at(i);
  for(unsigned int i = 0; i < v2.size(); ++i) a2[i] = v2.at(i);

  return TMath::KolmogorovTest(v1.size(),a1,v2.size(),a2,"");
}

//  ------------------------------------------------------------






int MyFindBin(const double& val, const std::vector<double>* binEdges)
{
  for(unsigned int bin = 0; bin < binEdges->size()-1; ++bin)
  {
    if( (val >= binEdges->at(bin)) && (val < binEdges->at(bin+1)) )
      return bin;
  }
  
  return -1;
}

//  ------------------------------------------------------------

int MyFindBin(const double& val, const double& min, const double& max, const double& invWidth)
{
  if( val < min || val >= max ) return -1;
  return int( (val - min) * invWidth );
}

//  ------------------------------------------------------------

double MyEval(TGraph* g, const double& x)
{
  double x1,x2,y1,y2;
  g -> GetPoint(0,x1,y1);
  g -> GetPoint(g->GetN()-1,x2,y2);
  
  if( x <  x1 ) return y1;
  if( x >= x2 ) return y2;
  
  for(int point = 0; point < g->GetN()-1; ++point)
  {
    g -> GetPoint(point,  x1,y1);
    g -> GetPoint(point+1,x2,y2);
    
    if( x >= x1 && x < x2 )
    {
      return y1 + (y2-y1)/(x2-x1)*(x-x1);
    }
  }
  
  return 1.;
}
