#ifndef ScaleEstimators_h
#define ScaleEstimators_h

#include "histoFunc.h"

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

#include "TH1F.h"
#include "TF1.h"



void FindSmallestInterval(double& mean, double& meanErr, double& min, double& max,
                          std::vector<double>& vals, std::vector<double>& weights,
                          const double& fraction, const bool& verbosity = false);

void FindRecursiveMean(double& mean, double& meanErr,
                       std::vector<double>& vals, std::vector<double>& weights,
                       const double& window, const double& tolerance,
                       const bool& verbosity = false);

void FindGausFit(double& mean, double& meanErr,
                 std::vector<double>& vals, std::vector<double>& weights,
                 const int& nBins, const double& xMin, const double& xMax,
                 TF1** f_gausFit, const std::string& name,
                 const double& startingMean,
                 const bool& verbosity = false);

void FindMean(double& mean, double& meanErr,
              std::vector<double>& vals, std::vector<double>& weights,
              const bool& verbosity = false);

void FindTemplateFit(double& scale, double& scaleErr,
                     TH1F* h_MC, TH1F* h_DA,
                     const bool& verbosity = false);

#endif
