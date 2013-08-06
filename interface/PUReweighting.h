#ifndef PUReweighting_h
#define PUReweighting_h

#include <iostream>
#include <string>
#include <map>

#include "TFile.h"
#include "TH1F.h"
#include "TChain.h"



void makePUDistributionMC(std::string, std::string, std::string, std::string);

std::map<float,float>* ComputePUweights(std::string, std::string, std::string, std::string, const bool& verbosity = false);
std::map<float,float>* ComputePUweights(TTree* t_mc, const std::string& PUFileName_da, const bool& verbosity = false);

#endif
