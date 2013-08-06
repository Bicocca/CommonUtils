#include "ZPeakFitUtils.h"



const double _invRootPi = 1./sqrt(atan2(0.,-1.)); 



void FitZPeak(const bool& doVoigtianFit, const bool& doCrystalBallFit,
              TF1** fitFunc_voigtian, TF1** fitFunc_crystalBall, const std::string& fitFuncName,
              TH1F* histo,
              const std::string& EBEE)
{
  std::cout << ">>>>>>FitZPeak::Fitting histogram " << histo->GetName() << " with integral " << histo->Integral() << std::endl;
  
  //-------------
  // gaussian fit
  
  TF1* f_gaus = new TF1("f_gaus","gaus",85.,95.);
  f_gaus -> SetParameters(histo->GetEntries(),90.,2.);
  histo -> Fit("f_gaus","QNRS+");
  
  float gaus_mean  = f_gaus -> GetParameter(1);
  float gaus_sigma = f_gaus -> GetParameter(2);
  
  
  //--------------------
  // triple-voigtian fit
  
  if( EBEE == "EB" )
  {
    (*fitFunc_voigtian) = new TF1((fitFuncName+"_voigtian").c_str(),myTripleVoigt,gaus_mean-1.20*gaus_sigma,gaus_mean+1.50*gaus_sigma,9);
    (*fitFunc_voigtian) -> SetParameters(0.7,0.,1.5,0.2,-2.,3.,2.,3.,histo->GetEntries());
  }
  else
  {
    (*fitFunc_voigtian) = new TF1((fitFuncName+"_voigtian").c_str(),myDoubleVoigt,gaus_mean-1.5*gaus_sigma,gaus_mean+2.0*gaus_sigma,6);
    (*fitFunc_voigtian) -> SetParameters(0.7,0.,1.5,-2.,3.,histo->GetEntries());
  }
  (*fitFunc_voigtian) -> SetNpx(10000);
  (*fitFunc_voigtian) -> SetLineStyle(1);
  (*fitFunc_voigtian) -> SetLineWidth(1);
  (*fitFunc_voigtian) -> SetLineColor(histo->GetMarkerColor());
  if( doVoigtianFit ) histo -> Fit((fitFuncName+"_voigtian").c_str(),"QNRS+");
  
  
  //--------------------
  // crystal-ball fit
    
  (*fitFunc_crystalBall) = new TF1((fitFuncName+"_crystalBall").c_str(),breitWigner_crystalBallLow,gaus_mean-3.*gaus_sigma,gaus_mean+2.*gaus_sigma,7);
  (*fitFunc_crystalBall) -> SetParameters(histo->GetEntries()/500.,90.1876,2.4952,0.159,1.449,1.178,1.934);
  (*fitFunc_crystalBall) -> FixParameter(1,90.1876);
  (*fitFunc_crystalBall) -> FixParameter(2,2.4952);
  (*fitFunc_crystalBall) -> SetParName(0,"N");
  (*fitFunc_crystalBall) -> SetParName(3,"#Deltam");
  (*fitFunc_crystalBall) -> SetParName(4,"#sigma_{CB}");
  (*fitFunc_crystalBall) -> SetParName(5,"#alpha");
  (*fitFunc_crystalBall) -> SetParName(6,"n");
  (*fitFunc_crystalBall) -> SetNpx(10000);
  (*fitFunc_crystalBall) -> SetLineColor(histo->GetMarkerColor());
  (*fitFunc_crystalBall) -> SetLineStyle(2);
  (*fitFunc_crystalBall) -> SetLineWidth(1);
  if( doCrystalBallFit) histo -> Fit((fitFuncName+"_crystalBall").c_str(),"QNRS+");
  
  
  delete f_gaus;
  
  return;
}



/*** breit-wigner ***/
double breitWigner(double* x, double* par)
{
  //[0] = N
  //[1] = mass
  //[2] = width
  
  double xx = x[0];
  double M = par[1];
  double G = par[2];
  
  double norm = M*M*G*G;
  
  return par[0] * norm / ( pow((xx*xx-M*M),2) + M*M*G*G );
}

/*** crystall ball with low tail ***/
double crystalBallLow(double* x, double* par)
{
  //[0] = N
  //[1] = mean
  //[2] = sigma
  //[3] = alpha
  //[4] = n
  
  double xx = x[0];
  double mean = par[1];
  double sigma = par[2];
  double alpha = par[3];
  double n = par[4];
  
  if( (xx-mean)/sigma <= -1.*fabs(alpha) )  
  {
    double A = pow(n/fabs(alpha), n) * exp(-0.5 * alpha*alpha);
    double B = n/fabs(alpha) - fabs(alpha);
    
    return par[0] * A * pow(B - (xx-mean)/sigma, -1.*n);
  }
  
  else
  {
    return par[0] * exp(-1. * (xx-mean)*(xx-mean) / (2*sigma*sigma) );
  } 
  
}

/*** breit-wigner convoluted with crystalBall ***/
double breitWigner_crystalBallLow(double* x, double* par)
{
  //[0] = N
  //[1] = b.w. - mass
  //[2] = b.w. - width
  //[3] = c.b. - mean
  //[4] = c.b. - sigma
  //[5] = c.b. - alpha
  //[6] = c.b. - n
  
  double xx = x[0];
  double M = par[1];
  double G = par[2];
  double mean = par[3];
  double sigma = par[4];
  double alpha = par[5];
  double n = par[6];
  
  TF1* bw = new TF1("temp_bw",breitWigner,0.,200.,3);
  bw -> FixParameter(0,1.);
  bw -> FixParameter(1,M);
  bw -> FixParameter(2,G);
  
  TF1* cb = new TF1("temp_cb",crystalBallLow,0.,200.,5);
  cb -> FixParameter(0,par[0]);
  cb -> FixParameter(1,mean);
  cb -> FixParameter(2,sigma);
  cb -> FixParameter(3,alpha);
  cb -> FixParameter(4,n);
  
  
  // convolute
  double xMin = 60.;
  double xMax = 120.;
  int nSteps = 1200;
  double stepWidth = (xMax-xMin)/nSteps;
  
  double val = 0.;
  for(int i = 0; i < nSteps; ++i)
  {
    double yy = xMin+i*stepWidth;
    val += bw->Eval(yy) * cb->Eval(xx-yy);
  }
  
  delete bw;
  delete cb;
  
  return val;
}



/*** double crystall ball ***/
double crystalBallLowHigh(double* x, double* par)
{
  //[0] = N
  //[1] = mean
  //[2] = sigma
  //[3] = alpha
  //[4] = n
  //[5] = alpha2
  //[6] = n2
  //[7] = scale
  
  double xx = x[0] * par[7];
  double mean = par[1];
  double sigma = par[2];
  double alpha = par[3];
  double n = par[4];
  double alpha2 = par[5];
  double n2 = par[6];
  
  if( (xx-mean)/sigma > fabs(alpha) )  
    {
      double A = pow(n/fabs(alpha), n) * exp(-0.5 * alpha*alpha);
      double B = n/fabs(alpha) - fabs(alpha);
    
      return par[0] * par[7] * A * pow(B + (xx-mean)/sigma, -1.*n);
    }
  
  else if( (xx-mean)/sigma < -1.*fabs(alpha2) )
    {
      double A = pow(n2/fabs(alpha2), n2) * exp(-0.5 * alpha2*alpha2);
      double B = n2/fabs(alpha2) - fabs(alpha2);
    
      return par[0] * par[7] * A * pow(B - (xx-mean)/sigma, -1.*n2);
    }
  
  else
    {
      return par[0] * par[7] * exp(-1. * (xx-mean)*(xx-mean) / (2*sigma*sigma) );
    } 
  
}



// *************************************************************
// Simple Voigtian with fixed sigma
// PDF - BW x Gaus = 'Voigtian' 
// Fast implementation adapted from 
// http://root.cern.ch/root/html/src/RooVoigtian.cxx.html
// *************************************************************
double mySimpleVoigt(double *xx, double *s)
{ 
  double x = xx[0];
  double xm = s[0]; // normalization (1. for pdf) 
  double dm = s[1]; // scale
  double sm = s[2]; // sigma
  
  const double mZ = 91.188;
  const double wZ = 2.4952; 
  
  double coef= -0.5/(sm*sm);
  double arg = x - (mZ+dm);

  // return constant for zero width and sigma
  if( sm == 0. && wZ == 0. ) return 1.;
  // Gauss for zero width
  if( wZ == 0. ) return exp(coef*arg*arg);
  
  // Breit-Wigner for zero sigma
  if( sm == 0. ) return (1./(arg*arg+0.25*wZ*wZ));
  
  // actual Voigtian for non-trivial width and sigma
  double c = 1. / (sqrt(2.)*sm);
  double a = 0.5*c*wZ;
  double u = c*arg;
  RooComplex z(u,a);
  RooComplex v(0.);
  
  v = RooMath::FastComplexErrFunc(z);
  
  return xm * c * v.re() * _invRootPi;
}



// *************************************************************
// Double Voigtian with diffrent sigmas and different shifts
// PDF - BW x (Gaus1 + Gaus2) = 'Voigtian1 + Voigtian2' 
// Fast implementation adapted from 
// http://root.cern.ch/root/html/src/RooVoigtian.cxx.html
// Parameters: 0 - Relative amplitude of Gaus1
//             1 - Relative amplitude of Gaus1
//             2 - Relative amplitude of Gaus1
//             3 - Relative amplitude of Gaus1
//             4 - Relative amplitude of Gaus1
// *************************************************************
double myDoubleVoigt(double *xx, double *s)
{
  const double mZ = 91.188;
  const double wZ = 2.4952; 

  double x = xx[0];
  double A1 = s[0];
  double x1 = s[1];
  double s1 = s[2];
  double A2 = 1.-A1;
  double x2 = s[3];
  double s2 = s[4];
  double AT = s[5]; 
  
  double arg1 = x - (mZ+x1);
  double arg2 = x - (mZ+x2);

  double vo1=1.;
  double vo2=1.;
  RooComplex v1(0.);
  RooComplex v2(0.);

  // Breit-Wigner for zero sigma1
  if( s1==0. ) vo1 = (1./(arg1*arg1+0.25*wZ*wZ));
  else
  {
    // actual Voigtian for non-trivial width and sigma
    double c1 = 1. /(sqrt(2.)*s1);
    double a1 = 0.5*c1*wZ;
    double u1 = c1*arg1;
    RooComplex z1(u1,a1) ;
    v1 = RooMath::FastComplexErrFunc(z1);  
    vo1 = c1 * v1.re() * _invRootPi;
  }
  
  // Breit-Wigner for zero sigma2
  if( s2==0. ) vo2=(1./(arg1*arg1+0.25*wZ*wZ));
  else
  {
    double c2 = 1. /(sqrt(2.)*s2);
    double a2 = 0.5*c2*wZ;
    double u2 = c2*arg2;
    RooComplex z2(u2,a2) ;
    v2 = RooMath::FastComplexErrFunc(z2);  
    vo2 = c2 * v2.re() * _invRootPi;
  }
  
  return AT*(A1*vo1 + A2*vo2); 
}



// *************************************************************
// Triple Voigtian with diffrent sigmas and different shifts
// PDF - BW x (Gaus1 + Gaus2 + Gaus3) = 'Voigtian1 + Voigtian2 + Voigtian3' 
// Fast implementation adapted from 
// http://root.cern.ch/root/html/src/RooVoigtian.cxx.html
// Parameters: 0 - Relative amplitude of Gaus1
//             1 - Relative amplitude of Gaus1
//             2 - Relative amplitude of Gaus1
//             3 - Relative amplitude of Gaus1
//             4 - Relative amplitude of Gaus1
// *************************************************************
double myTripleVoigt(double *xx, double *s)
{
  const double mZ = 91.188;
  const double wZ = 2.4952; 

  double x = xx[0];
  double A1 = s[0];
  double x1 = s[1];
  double s1 = s[2];
  double A2 = s[3];
  double x2 = s[4];
  double s2 = s[5];
  double A3 = 1.-A1-A2;
  double x3 = s[6];
  double s3 = s[7];
  double AT = s[8]; 
  
  double arg1 = x - (mZ+x1);
  double arg2 = x - (mZ+x2);
  double arg3 = x - (mZ+x3);

  double vo1=1.;
  double vo2=1.;
  double vo3=1.;
  RooComplex v1(0.);
  RooComplex v2(0.);
  RooComplex v3(0.);

  // Breit-Wigner for zero sigma1
  if( s1==0. ) vo1 = (1./(arg1*arg1+0.25*wZ*wZ));
  else
  {
    // actual Voigtian for non-trivial width and sigma
    double c1 = 1. /(sqrt(2.)*s1);
    double a1 = 0.5*c1*wZ;
    double u1 = c1*arg1;
    RooComplex z1(u1,a1) ;
    v1 = RooMath::FastComplexErrFunc(z1);  
    vo1 = c1 * v1.re() * _invRootPi;
  }
  
  // Breit-Wigner for zero sigma2
  if( s2==0. ) vo2=(1./(arg1*arg1+0.25*wZ*wZ));
  else
  {
    double c2 = 1. /(sqrt(2.)*s2);
    double a2 = 0.5*c2*wZ;
    double u2 = c2*arg2;
    RooComplex z2(u2,a2) ;
    v2 = RooMath::FastComplexErrFunc(z2);  
    vo2 = c2 * v2.re() * _invRootPi;
  }
  
  // Breit-Wigner for zero sigma3
  if( s3==0. ) vo3=(1./(arg3*arg3+0.25*wZ*wZ));
  else
  {
    double c3 = 1. /(sqrt(2.)*s3);
    double a3 = 0.5*c3*wZ;
    double u3 = c3*arg3;
    RooComplex z3(u3,a3) ;
    v3 = RooMath::FastComplexErrFunc(z3);
    vo3 = c3 * v3.re() * _invRootPi;
  }
    
  return AT*(A1*vo1 + A2*vo2 + A3*vo3); 
}



void GetOLSigma(double& peak, double& sigmaOL,
                TF1* f, const double& prec, TArrow** line, TArrow** line2)
{
  double xMax = f -> GetMaximumX();
  double yMax = f -> GetMaximum();
  //std::cout << "x_max: " << xMax << "   y_max: " << yMax << std::endl;
  
  float yHalf = 0.5 * yMax;
  float xLow = xMax;
  float xHig = xMax;
  
  while( f->Eval(xLow) > yHalf)
  {
    xLow = xLow - prec;
  }
  while( f->Eval(xHig) > yHalf)
  {
    xHig = xHig + prec;
  }
  
  if( line != NULL )
  {
    (*line) = new TArrow(xLow,yHalf,xHig,yHalf);
    (*line) -> SetLineWidth(2);
    (*line) -> SetLineColor(f->GetLineColor());
  }
  
  if( line2 != NULL )
  {
    (*line2) = new TArrow(xMax,0.,xMax,yMax);
    (*line2) -> SetLineWidth(2);
    (*line2) -> SetLineColor(f->GetLineColor());
  }
  
  float a = 0.5346;
  float b = 0.2166;
  float fl = 2.4952;
  
  peak = xMax;
  float FWHM = xHig - xLow;
  sigmaOL = sqrt( (FWHM-a*fl)*(FWHM-a*fl) - b*fl*fl )/(2*sqrt(2*log(2)));
}
