#include "PUReweighting.h"



void makePUDistributionMC(std::string inputTree, std::string inputFile, std::string inputBranch, std::string outputName)
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



std::map<float,float>* ComputePUweights(std::string inputFileDA, std::string inputFileMC, std::string inputHistoNameDA, std::string inputHistoNameMC, const bool& verbosity) 
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



std::map<float,float>* ComputePUweights(TTree* t_mc, const std::string& PUFileName_da,
                                        const bool& verbosity)
{
  // histograms
  TH1F* h_mc = new TH1F("h_mc","",100,-0.5,99.5);
  TH1F* h_da = new TH1F("h_da","",100,-0.5,99.5);
  TH1F* temp = NULL;
  
  
  
  // fill mc histogram
  std::cout << "\n>>> ComputePUweights::MC pileup histogram" << std::endl;
  std::cout << ">>>>>> MC: tree " << t_mc -> GetName() << std::endl;
  
  t_mc -> Draw("PUit_TrueNumInteractions >> h_mc","","goff");
  h_mc -> Scale(1./h_mc->Integral());
  std::cout << ">>>>>> Integral: " << h_mc -> Integral() << std::endl;
  
  
  // fill da histogram
  std::cout << ">>> ComputePUweights::data pileup histogram" << std::endl;
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
