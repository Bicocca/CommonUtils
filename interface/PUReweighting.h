#ifndef PUReweighting_h
#define PUReweighting_h

#include <iostream>
#include <string>
#include <map>
#include <cstdlib>

#include "TFile.h"
#include "TH1F.h"
#include "TChain.h"



void makePUDistributionMC(std::string, std::string, std::string, std::string);

void CreatePUWeightsFile(std::string outFileName,
                         std::string inputFileDA, std::string inputFileMC, std::string inputHistoNameDA, std::string inputTreeNameMC, const bool& verbosity = false);

std::map<float,float>*       ComputePUWeights(const std::string&, const std::string&, const bool& verbosity = false);
std::map<float,float>*       ComputePUWeights(const std::string&, const std::string&, const std::string&, const std::string&, const bool& verbosity = false);
std::map<float,float>*       ComputePUWeights(TTree* t_mc, const std::string& PUFileName_da, const bool& verbosity = false);
std::map<std::string,TH1F*>* ReadPUWeights(const std::string& MCGen, const bool& runDepFlag, const int& runMin, const int& runMax);

#endif
