#ifndef ParseIJazZFile_h
#define ParseIJazZFile_h

//----------------------------------------------------------//
// Project:   ParseIJazZFile
// Author:    Andrea Benaglia (andrea.benaglia@cern.ch)
// Modified:  03/05/2013
// Admins:    Andrea Benaglia (andrea.benaglia@cern.ch)
//---------------------------------------------------------//

/*
  Parse a standard IJazZ resolution output file and put values into a TH2F
*/
  

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cmath>

#include "TH2F.h"
#include "TAxis.h"



struct MyAxis
{
  std::string axisName;
  int nPoints;
  std::vector<double> edges;
  std::vector<float> xs;
  std::vector<float> xMins;
  std::vector<float> xMaxs;
  std::vector<float> bins;

  void DefinePoints();
  void Print();
};



TH2F* ParseIJazZFile(const std::string &fname);

std::vector<MyAxis*> GetBinning(const std::string& fileName);

#endif
