#include "ParseIJazZFile.h"



TH2F* ParseIJazZFile(const std::string &fname)
{
  // read IJazZ reso file and savie it in a TH2F
  std::vector<MyAxis*> myAxes = GetBinning(fname.c_str());
  float reso,reso_err;
  TH2F* resoMatrix = new TH2F(Form("h_%s",fname.c_str()),"", myAxes.at(0)->nPoints, myAxes.at(0)->xMins[0], myAxes.at(0)->xMaxs[myAxes.at(0)->nPoints-1],
                                                             myAxes.at(1)->nPoints, myAxes.at(1)->xMins[0], myAxes.at(1)->xMaxs[myAxes.at(1)->nPoints-1]);
  
  // set bins
  resoMatrix -> SetBins(myAxes.at(0)->nPoints,&(myAxes.at(0)->edges[0]),myAxes.at(1)->nPoints,&(myAxes.at(1)->edges[0]));

  // fill histo
  std::fstream in(fname.c_str());
  
  std::string buffer;
  for(int i = 0; i < 3; ++i) getline(in,buffer);

  for(int binR9 = 1; binR9 <= resoMatrix -> GetNbinsY(); ++binR9)
    for(int binEta = 1; binEta <= resoMatrix -> GetNbinsX(); ++binEta)
    {
      in >> reso >> reso_err;
      resoMatrix -> SetBinContent(binEta,binR9,reso);
    }
  return resoMatrix;
}

//---------------------------------------------------------------------------------------


std::vector<MyAxis*> GetBinning(const std::string& fileName)
{
  std::ifstream inFile(fileName.c_str(),std::ios::in);
  
  std::vector<MyAxis*> myAxes;
  
  std::string line;
  std::string buffer;
  int axisIt = 0;
  
  while(1)
  {
    std::getline(inFile,line);
    if( !inFile.good() ) break;
    
    std::stringstream ss(line);
    unsigned int found = ss.str().find("axis");
    if( found != 4294967295 )
    {
      myAxes.push_back(new MyAxis);
      MyAxis* axis = myAxes.at(axisIt);
      
       ss >> buffer;
       ss >> buffer;
       
       axis -> axisName = buffer;      
       
       ss >> buffer;
       ss >> buffer;
       ss >> buffer;
       ss >> buffer;
       
       axis -> nPoints = atoi(buffer.c_str()) - 1;
       
       ss >> buffer;
       
       for(int point = 0; point < axis->nPoints+1; ++point)
       {
         ss >> buffer;
         (axis->edges).push_back( atof(buffer.c_str()) );
       }
       
       axis -> DefinePoints();
       //axis -> Print();
       
        ++axisIt;
      }
    }
  
  inFile.close();
  
  return myAxes;
}

//---------------------------------------------------------------------------------------


void MyAxis::DefinePoints()
{
  for(int point = 0; point < nPoints; ++point)
  {
    float xMin = edges.at(point);
    float xMax = edges.at(point+1);
    float x = 0.5 * (xMin + xMax);
    
    xs.push_back(x);
    xMins.push_back(xMin);
    xMaxs.push_back(xMax);
  }
}

//---------------------------------------------------------------------------------------


void MyAxis::Print()
{
  std::cout << ">>> MyAxis::Print() <<<" << std::endl;
  std::cout << ">>>>>> axisName: " << axisName << " " << std::endl;;
  for(int point = 0; point < nPoints; ++point)
  {
    float xMin = xMins.at(point);
    float xMax = xMaxs.at(point);
    float x    =    xs.at(point);
    
    std::cout << ">>>>>> x: " << x << "   [" << xMin << "," << xMax << "]" << std::endl;
  }
}
