#ifndef GetCategories_h
#define GetCategories_h

#include <vector>
#include <string>
#include <cmath>

#include "TString.h"



int GetSingleCategory(const float& scEta1, const float& R91);

int GetStraightCategory(const float& scEta1, const float& R91, const float& scEta2, const float& R92);
int GetHggCiCCategory(const float& scEta1, const float& R91, const float& scEta2, const float& R92);

std::string GetEtaR9CatLabel(const float& eta1, const float& R91, const float& eta2, const float& R92,
                             const std::vector<float>& etaBins, const std::vector<float>& R9Bins,
                             const std::vector<std::string>& categories);

#endif
