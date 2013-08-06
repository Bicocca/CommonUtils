#include "ScaleEstimators.h"



void FindSmallestInterval(double& mean, double& meanErr, double& min, double& max,
                          std::vector<double>& vals, std::vector<double>& weights,
                          const double& fraction, const bool& verbosity)
{
  if( verbosity )
    std::cout << ">>>>>> FindSmallestInterval" << std::endl;
  
  
  std::sort(vals.begin(),vals.end());
  double lowEdge = vals.at(0) - 0.01*(vals.at(vals.size()-1)-vals.at(0));
  double higEdge = vals.at(vals.size()-1) + 0.01*(vals.at(vals.size()-1)-vals.at(0));;
  
  unsigned int nPoints = vals.size();
  unsigned int maxPoints = (unsigned int)(fraction * nPoints);
  
  unsigned int minPoint = 0;
  unsigned int maxPoint = 0;
  double delta = 999999.;
  for(unsigned int point = 0; point < nPoints-maxPoints; ++point)
  {
    double tmpMin = vals.at(point);
    double tmpMax = vals.at(point+maxPoints-1);
    if( tmpMax-tmpMin < delta )
    {
      delta = tmpMax - tmpMin;
      min = tmpMin;
      max = tmpMax;
      minPoint = point;
      maxPoint = point + maxPoints - 1;
    }
  }
  
  
  TH1F* h_temp = new TH1F("h_temp","",100,lowEdge,higEdge);
  h_temp -> Sumw2();
  
  for(unsigned int point = minPoint; point < maxPoint; ++point)
    h_temp -> Fill( vals.at(point),weights.at(point) );
  
  mean    = h_temp -> GetMean();
  meanErr = h_temp -> GetMeanError();
  
  delete h_temp;
}



void FindRecursiveMean(double& mean, double& meanErr,
                       std::vector<double>& vals, std::vector<double>& weights,
                       const double& window, const double& tolerance,
                       const bool& verbosity)
{
  if( verbosity )
    std::cout << ">>>>>> FindRecursiveMean" << std::endl;
  
  
  std::sort(vals.begin(),vals.end());
  double lowEdge = vals.at(0) - 0.01*(vals.at(vals.size()-1)-vals.at(0));
  double higEdge = vals.at(vals.size()-1) + 0.01*(vals.at(vals.size()-1)-vals.at(0));
  
  TH1F* h_start = new TH1F("h_start","",100,lowEdge,higEdge);
  h_start -> Sumw2();
  for(unsigned int point = 0; point < vals.size(); ++point)
    h_start -> Fill(vals.at(point),weights.at(point));
  
  
  int trial = 0;
  double oldMean = h_start->GetMean();
  double delta = 999999;
  
  while( delta > tolerance )
  {
    TH1F* h_temp = new TH1F("h_temp","",100,lowEdge,higEdge);
    h_temp -> Sumw2();
    
    for(unsigned int point = 0; point < vals.size(); ++point)
    {
      if( (trial == 0) && (fabs(vals.at(point) - oldMean) > 2.*window) ) continue;
      if( (trial  > 0) && (fabs(vals.at(point) - oldMean) > 1.*window) ) continue;
      
      h_temp -> Fill( vals.at(point),weights.at(point) );
    }
    
    mean    = h_temp -> GetMean();
    meanErr = h_temp -> GetMeanError();
    delete h_temp;
    
    if( fabs(mean-oldMean) > tolerance )
    {
      oldMean = mean;
      ++trial;
    }
    else
    {
      return;
    }
  }
}



void FindGausFit(double& mean, double& meanErr,
                 std::vector<double>& vals, std::vector<double>& weights,
                 const int& nBins, const double& xMin, const double& xMax,
                 TF1** f_gausFit, const std::string& name,
                 const double& startingMean,
                 const bool& verbosity)
{
  if( verbosity )
    std::cout << ">>>>>> FindGausFit" << std::endl;
  
  
  TH1F* h_temp = new TH1F("h_temp","",nBins,xMin,xMax);
  h_temp -> Sumw2();
  
  for(unsigned int point = 0; point < vals.size(); ++point)
  {
    h_temp -> Fill( vals.at(point),weights.at(point) );
  }
  h_temp -> Scale(1./h_temp->Integral());
  
  mean    = h_temp -> GetMean();
  meanErr = h_temp -> GetRMS();
  
  TF1* f_temp = new TF1("f_temp","[0]*exp(-1.*(x-[1])*(x-[1])/2/[2]/[2])",0.75,1.25);
  f_temp -> SetParameters(h_temp->GetMaximum(),1.,meanErr);
  f_temp -> FixParameter(0,h_temp->GetMaximum());
  f_temp -> FixParameter(1,startingMean);
  f_temp -> SetParLimits(2,0.,1.);
  h_temp -> Fit("f_temp","QNLR","");
  
  mean    = f_temp -> GetParameter(1);
  meanErr = f_temp -> GetParameter(2);
  
  TF1* f_temp2 = new TF1("f_temp2","[0]*exp(-1.*(x-[1])*(x-[1])/2/[2]/[2])",startingMean-1.5*meanErr,startingMean+1.5*meanErr);
  f_temp2 -> SetParameters(0.1,mean,meanErr);
  f_temp2 -> FixParameter(1,startingMean);
  f_temp2 -> SetParLimits(0,0.,1.);
  f_temp2 -> SetParLimits(2,0.,1.);
  h_temp -> Fit(name.c_str(),"QNLR","");
  
  mean    = f_temp2 -> GetParameter(1);
  meanErr = f_temp2 -> GetParameter(2);
  
  (*f_gausFit) = new TF1(name.c_str(),"[0]*exp(-1.*(x-[1])*(x-[1])/2/[2]/[2])",mean-meanErr,mean+meanErr);
  (*f_gausFit) -> SetParameters(0.1,mean,meanErr);
  (*f_gausFit) -> SetParLimits(0,0.,1.);
  (*f_gausFit) -> SetParLimits(1,0.5,1.5);
  (*f_gausFit) -> SetParLimits(2,0.,1.);
  h_temp -> Fit(name.c_str(),"QNLR","");
  
  mean    = (*f_gausFit) -> GetParameter(1);
  meanErr = (*f_gausFit) -> GetParError(1);
  
  delete f_temp;
  delete h_temp;
}



void FindMean(double& mean, double& meanErr,
              std::vector<double>& vals, std::vector<double>& weights,
              const bool& verbosity)
{
  if( verbosity )
    std::cout << ">>>>>> FindMean" << std::endl;
  
  
  std::sort(vals.begin(),vals.end());
  double lowEdge = vals.at(0) - 0.01*(vals.at(vals.size()-1)-vals.at(0));
  double higEdge = vals.at(vals.size()-1) + 0.01*(vals.at(vals.size()-1)-vals.at(0));
  
  TH1F* h_temp = new TH1F("h_temp","",100,lowEdge,higEdge);
  h_temp -> Sumw2();
  
  for(unsigned int point = 0; point < vals.size(); ++point)
  {
    h_temp -> Fill( vals.at(point),weights.at(point) );
  }
  
  mean    = h_temp -> GetMean();
  meanErr = h_temp -> GetMeanError();
  
  delete h_temp;
}



void FindTemplateFit(double& scale, double& scaleErr,
                     TH1F* h_MC, TH1F* h_DA,
                     const bool& verbosity)
{
  if( verbosity )
    std::cout << ">>>>>> FindTemplateFit" << std::endl;
  
  
  float xNorm = h_DA->Integral() / h_MC->Integral() * h_DA->GetBinWidth(1) / h_MC->GetBinWidth(1);  
  h_MC -> Scale(xNorm);
  
  
  histoFunc* templateHistoFunc = new histoFunc(h_MC);
  char funcName[50];
  sprintf(funcName,"f_template");
  
  TF1* f_template = new TF1(funcName,templateHistoFunc,0.9,1.1,3,"histoFunc");
  
  f_template -> SetParName(0,"Norm"); 
  f_template -> SetParName(1,"Scale factor"); 
  f_template -> SetLineWidth(1); 
  f_template -> SetNpx(10000);
  f_template -> SetLineColor(kRed+2); 
  
  f_template->FixParameter(0,1.);
  f_template->SetParameter(1,0.99);
  f_template->FixParameter(2,0.);
  
  TFitResultPtr rp = h_DA -> Fit(funcName,"QERLS+");
  int fStatus = rp;
  int nTrials = 0;
  while( (fStatus != 0) && (nTrials < 10) )
  {
    rp = h_DA -> Fit(funcName,"QERLS+");
    fStatus = rp;
    if( fStatus == 0 ) break;
    ++nTrials;
  }
  
  double k   = f_template->GetParameter(1);
  double eee = f_template->GetParError(1); 
  
  scale = 1/k;
  scaleErr = eee/k/k;
  
  delete f_template;
}
