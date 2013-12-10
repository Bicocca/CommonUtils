#include "PUReweighting.h"



void makePUDistributionMC(const std::string& inputTree, const std::string& inputFile, const std::string& inputBranch, const std::string& outputName)
{
  float PUit_TrueNumInteractions;
  
  TChain* ntu_MC = new TChain(inputTree.c_str());
  ntu_MC -> Add(inputFile.c_str());
  ntu_MC->SetBranchStatus("*",0);
  ntu_MC->SetBranchStatus(inputBranch.c_str(),1);
  ntu_MC->SetBranchAddress(inputBranch.c_str(),&PUit_TrueNumInteractions);

  TH1F* h_PUit_TrueNumInteractions = new TH1F("h_PUit_TrueNumInteractions","h_PUit_TrueNumInteractions", 100,-0.5,99.5);
  
  for(int entry = 0; entry < ntu_MC->GetEntries(); entry++)
    {
	ntu_MC->GetEntry(entry);
        h_PUit_TrueNumInteractions->Fill(PUit_TrueNumInteractions);
    }

  TFile* output = new TFile(outputName.c_str(),"RECREATE");

  output->cd();
  h_PUit_TrueNumInteractions->Write();
  output->Close();
}



std::map<float,float>* ComputePUWeights(const std::string& inputFileDA, const std::string& inputFileMC, const std::string& inputHistoNameDA, const std::string& inputHistoNameMC, const bool& verbosity) 
{
  // histograms
  TH1F* h_mc = new TH1F("h_mc","",100,-0.5,99.5);
  TH1F* h_da = new TH1F("h_da","",100,-0.5,99.5);
  TH1F* temp = NULL;
  
  // files
  TFile* f_mc = TFile::Open(inputFileMC.c_str());
  TFile* f_da = TFile::Open(inputFileDA.c_str());
  
  
  
  // fill mc histogram
  std::cout << "\n\n\n>>> MC pileup histogram" << std::endl;
  
  temp = (TH1F*)( f_mc->Get(inputHistoNameMC.c_str()) );
  for(int bin = 1; bin <= temp->GetNbinsX(); ++bin)
  {
    float binCenter  = temp -> GetBinCenter(bin);
    float binContent = temp -> GetBinContent(bin);
    h_mc -> Fill(binCenter,binContent);
  }
  h_mc -> Scale(1./h_mc->Integral());
  if( verbosity )
    std::cout << "MC Integral: " << h_mc -> Integral() << std::endl;
  
  
  // fill da histogram
  std::cout << "\n\n\n>>> DA pileup histogram" << std::endl;
  
  temp = (TH1F*)( f_da->Get(inputHistoNameDA.c_str()) );
  for(int bin = 1; bin <= temp->GetNbinsX(); ++bin)
  {
    float binCenter  = temp -> GetBinCenter(bin) - 0.5;
    float binContent = temp -> GetBinContent(bin);
    h_da -> Fill(binCenter,binContent);
  }
  h_da -> Scale(1./h_da->Integral());
  if( verbosity )
    std::cout << "DA Integral: " << h_da -> Integral() << std::endl;
  
  
  // compute PU weights
  TH1F* h_PUweights = (TH1F*)(h_da->Clone("h_PUweights"));
  h_PUweights -> Divide(h_mc);

  std::map<float,float>* map_PUweights = new std::map<float,float>;
  for(int bin = 1; bin <= h_PUweights->GetNbinsX(); ++bin)
  {
    (*map_PUweights)[h_PUweights->GetBinCenter(bin)] = h_PUweights->GetBinContent(bin);
    if( verbosity )
      std::cout << "bin: " << bin << "   binCenter: " << h_PUweights->GetBinCenter(bin) << "   binContent: " << h_PUweights->GetBinContent(bin) << std::endl;
  }
  
  return map_PUweights;  
}



std::map<float,float>* ComputePUWeights(const std::string& inputFile, const std::string& inputHistoName, const bool& verbosity) 
{
  // histograms
  TH1F* temp = NULL;
  
  // files
  TFile* f = TFile::Open(inputFile.c_str());
  
  // fill weight histogram
  temp = (TH1F*)( f->Get(inputHistoName.c_str()) );
  
  // compute PU weights
  std::map<float,float>* map_PUweights = new std::map<float,float>;
  for(int bin = 1; bin <= temp->GetNbinsX(); ++bin)
  {
    (*map_PUweights)[temp->GetBinCenter(bin)] = temp->GetBinContent(bin);
    if( verbosity )
      std::cout << "bin: " << bin << "   binCenter: " << temp->GetBinCenter(bin) << "   binContent: " << temp->GetBinContent(bin) << std::endl;
  }
  
  return map_PUweights;  
}



void CreatePUWeightsFile(std::string outFileName,
                         std::string inputFileDA, std::string inputFileMC, std::string inputHistoNameDA, std::string inputTreeNameMC, const bool& verbosity)
{
  TFile* outFile = TFile::Open(outFileName.c_str(),"RECREATE");
  outFile -> cd();
  
  // histograms
  TH1F* h_mc = new TH1F("h_mc","",100,-0.5,99.5);
  TH1F* h_da = new TH1F("h_da","",100,-0.5,99.5);
  TH1F* temp = NULL;
  TTree* tree = NULL;
  
  // files
  TFile* f_mc = TFile::Open(inputFileMC.c_str());
  TFile* f_da = TFile::Open(inputFileDA.c_str());
  
  
  
  // fill mc histogram
  std::cout << "\n\n\n>>> MC pileup histogram" << std::endl;
  
  tree = (TTree*)( f_mc->Get(inputTreeNameMC.c_str()) );
  
  outFile -> cd();
  tree -> Draw("nPUtrue >> h_mc","BX == 0","goff");
  
  h_mc -> Scale(1./h_mc->Integral());
  if( verbosity )
    std::cout << "MC Integral: " << h_mc -> Integral() << std::endl;
  
  
  // fill da histogram
  std::cout << "\n\n\n>>> DA pileup histogram" << std::endl;
  
  temp = (TH1F*)( f_da->Get(inputHistoNameDA.c_str()) );
  for(int bin = 1; bin <= temp->GetNbinsX(); ++bin)
  {
    float binCenter  = temp -> GetBinCenter(bin) - 0.5;
    float binContent = temp -> GetBinContent(bin);
    h_da -> Fill(binCenter,binContent);
  }
  h_da -> Scale(1./h_da->Integral());
  if( verbosity )
    std::cout << "DA Integral: " << h_da -> Integral() << std::endl;
  
  
  // compute PU weights
  TH1F* h_PUweights = (TH1F*)(h_da->Clone("h_PUweights"));
  h_PUweights -> Divide(h_mc);
  
  outFile -> cd();
  h_PUweights -> Write();
  outFile -> Close();
}



std::map<float,float>* ComputePUWeights(TTree* t_mc, const std::string& PUFileName_da,
                                        const bool& verbosity)
{
  // histograms
  TH1F* h_mc = new TH1F("h_mc","",100,-0.5,99.5);
  TH1F* h_da = new TH1F("h_da","",100,-0.5,99.5);
  TH1F* temp = NULL;
  
  
  
  // fill mc histogram
  std::cout << "\n>>> ComputePUWeights::MC pileup histogram" << std::endl;
  std::cout << ">>>>>> MC: tree " << t_mc -> GetName() << std::endl;
  
  t_mc -> Draw("PUit_TrueNumInteractions >> h_mc","","goff");
  h_mc -> Scale(1./h_mc->Integral());
  std::cout << ">>>>>> Integral: " << h_mc -> Integral() << std::endl;
  
  
  // fill da histogram
  std::cout << ">>> ComputePUWeights::data pileup histogram" << std::endl;
  std::cout << ">>>>>> DA: file " << PUFileName_da << std::endl;
  
  TFile* f_da = TFile::Open(PUFileName_da.c_str(),"READ");
  temp = (TH1F*)( f_da->Get("pileup") );
  for(int bin = 1; bin <= temp->GetNbinsX(); ++bin)
    {
      float binCenter  = temp -> GetBinCenter(bin) - 0.5;
      float binContent = temp -> GetBinContent(bin);
      if( verbosity )
	std::cout << "bin: " << bin << "   binCenter: " << binCenter << "   binContent: " << binContent << std::endl;
    
      h_da -> Fill(binCenter,binContent);
    }
  h_da -> Scale(1./h_da->Integral());
  std::cout << ">>>>>> Integral: " << h_da -> Integral() << std::endl;
  
  
  // save PU weights
  TH1F* h_PUweights = (TH1F*)(h_da->Clone("h_PUweights"));
  h_PUweights -> Divide(h_mc);
  
  std::map<float,float>* map_PUweights = new std::map<float,float>;
  for(int bin = 1; bin <= h_PUweights->GetNbinsX(); ++bin)
  {
    (*map_PUweights)[h_PUweights->GetBinCenter(bin)] = h_PUweights->GetBinContent(bin);
    if( verbosity )
      std::cout << "bin: " << bin << "   binCenter: " << h_PUweights->GetBinCenter(bin) << "   binContent: " << h_PUweights->GetBinContent(bin) << std::endl;
  }
  
  return map_PUweights;
}



std::map<std::string,TH1F*>* ReadPUWeights(const std::string& MCGen, const bool& runDepFlag, const int& runMin, const int& runMax)
{
  std::map<std::string,TH1F*>* PUWeightsMap = new std::map<std::string,TH1F*>;
  
  std::string PUDir(getenv("COMMONUTILS"));
  std::string PUWeightsFileName;
  TFile* temp;
  
  
  std::string fileBaseName = "";
  if( MCGen == "7TeV" ) fileBaseName = "PUWeights_DYToEE_M20_powheg-Summer11-START53_Run2011AB";
  if( MCGen == "madgraph" ) fileBaseName = "PUWeights_DYJets-Summer12-START53-ZSkim";
  if( MCGen == "powheg" )   fileBaseName = "PUWeights_DYToEE_M20_powheg-Summer12-START53-ZSkim";
  
  
  //-----------------
  // run-dependent MC
  
  if( runDepFlag && runMin == 0 && runMax == 999999 )
  {
    PUWeightsFileName = fileBaseName + "-runDependent_Run2012AB.root";
    temp = TFile::Open((PUDir+"/data/pileup/"+PUWeightsFileName).c_str(),"READ");
    (*PUWeightsMap)["Run2012AB"] = (TH1F*)( temp->Get("h_PUweights") );
    std::cout << ">>>>>> PUReweighting::ReadPUWeights:   PUWeights[Run2012AB] filled from " << PUWeightsFileName << std::endl;
    PUWeightsFileName.clear();
    
    PUWeightsFileName = fileBaseName + "-runDependent_Run2012C.root";
    temp = TFile::Open((PUDir+"/data/pileup/"+PUWeightsFileName).c_str(),"READ");
    (*PUWeightsMap)["Run2012C"] = (TH1F*)( temp->Get("h_PUweights") );
    std::cout << ">>>>>> PUReweighting::ReadPUWeights:   PUWeights[Run2012C]  filled from " << PUWeightsFileName << std::endl;
    PUWeightsFileName.clear();
    
    PUWeightsFileName = fileBaseName + "-runDependent_Run2012D.root";
    temp = TFile::Open((PUDir+"/data/pileup/"+PUWeightsFileName).c_str(),"READ");
    (*PUWeightsMap)["Run2012D"] = (TH1F*)( temp->Get("h_PUweights") );
    std::cout << ">>>>>> PUReweighting::ReadPUWeights:   PUWeights[Run2012D]  filled from " << PUWeightsFileName << std::endl;
    PUWeightsFileName.clear();
  }
  if( runDepFlag && runMin != 0 && runMax != 999999 )
  {
    PUWeightsFileName = Form("%s-runDependent_%d-%d.root",fileBaseName.c_str(),runMin,runMax);
    temp = TFile::Open((PUDir+"/data/pileup/"+PUWeightsFileName).c_str(),"READ");
    (*PUWeightsMap)[Form("%d-%d",runMin,runMax)] = (TH1F*)( temp->Get("h_PUweights") );
    std::cout << ">>>>>> PUReweighting::ReadPUWeights:   PUWeights[" << Form("%d-%d",runMin,runMax) << "] filled from " << PUWeightsFileName << std::endl;
  }
  
  
  //---------------------
  // non run-dependent MC
  
  if( !runDepFlag && runMin == 0 && runMax == 999999 )
  {
    PUWeightsFileName = fileBaseName + "_allRange.root";
    temp = TFile::Open((PUDir+"/data/pileup/"+PUWeightsFileName).c_str(),"READ");
    (*PUWeightsMap)["allRange"] = (TH1F*)( temp->Get("h_PUweights") );
    std::cout << ">>>>>> PUReweighting::ReadPUWeights:   PUWeights[allRange] filled from " << PUWeightsFileName << std::endl;
  }
  if( !runDepFlag && runMin != 0 && runMax != 999999 )
  {
    PUWeightsFileName = Form("%s_%d-%d.root",fileBaseName.c_str(),runMin,runMax);
    temp = TFile::Open((PUDir+"/data/pileup/"+PUWeightsFileName).c_str(),"READ");
    (*PUWeightsMap)[Form("%d-%d",runMin,runMax)] = (TH1F*)( temp->Get("h_PUweights") );
    std::cout << ">>>>>> PUReweighting::ReadPUWeights:   PUWeights[" << Form("%d-%d",runMin,runMax) << "] filled from " << PUWeightsFileName << std::endl;
  }
  
  return PUWeightsMap;
}
