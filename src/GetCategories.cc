#include "GetCategories.h"



int GetStraightCategory(const float& scEta1, const float& R91, const float& scEta2, const float& R92)
{
  if( (fabs(scEta1) < 1.4442) && (fabs(scEta2) < 1.4442) )
  {
    if( (R91 > 0.94) && (R92 > 0.94) ) return 0;
    if( (R91 < 0.94) && (R92 < 0.94) ) return 1;
  }
  
  if( (fabs(scEta1) > 1.560) && (fabs(scEta2) > 1.560) )
  {
    if( (R91 > 0.94) && (R92 > 0.94) ) return 2;
    if( (R91 < 0.94) && (R92 < 0.94) ) return 3;
  }
  
  return -1;
}



int GetHggCiCCategory(const float& scEta1, const float& R91, const float& scEta2, const float& R92)
{
  if( (fabs(scEta1) < 1.4442) && (fabs(scEta2) < 1.4442) )
  {
    if( (R91 > 0.94) && (R92 > 0.94) ) return 0;
    else                               return 1;
  }
  
  else
  {
    if( (R91 > 0.94) && (R92 > 0.94) ) return 2;
    else                               return 3;
  }
  
  return -1;
}
