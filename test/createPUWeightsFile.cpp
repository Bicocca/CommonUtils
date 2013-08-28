# include "PUReweighting.h"


int main(int argc, char** argv)
{
  std::string runRange(argv[1]);
  std::string period(argv[2]);
  
  CreatePUWeightsFile("pileup/PUWeights_DYJets-Summer12-START53-ZSkim-runDependent_Run2012AB.root",
                      "pileup/pileup_69p3mb_true_Winter2013_Collisions12_Run2012AB.root",
                      "pileup/PUDumper_DYJets-Summer12-START53-ZSkim-runDependent_Run2012AB.root",
                      "pileup",
                      "PUDumper/pileup",false);
  
  CreatePUWeightsFile("pileup/PUWeights_DYJets-Summer12-START53-ZSkim-runDependent_Run2012C.root",
                      "pileup/pileup_69p3mb_true_Winter2013_Collisions12_Run2012C.root",
                      "pileup/PUDumper_DYJets-Summer12-START53-ZSkim-runDependent_Run2012C.root",
                      "pileup",
                      "PUDumper/pileup",false);
  
  CreatePUWeightsFile("pileup/PUWeights_DYJets-Summer12-START53-ZSkim-runDependent_Run2012D.root",
                      "pileup/pileup_69p3mb_true_Winter2013_Collisions12_Run2012D.root",
                      "pileup/PUDumper_DYJets-Summer12-START53-ZSkim-runDependent_Run2012D.root",
                      "pileup",
                      "PUDumper/pileup",false);
  /*
  CreatePUWeightsFile("pileup/PUWeights_DYJets-Summer12-START53-ZSkim-runDependent_"+runRange+".root",
                      "pileup/pileup_69p3mb_true_Winter2013_Collisions12_"+runRange+".root",
                      "pileup/PUDumper_DYJets-Summer12-START53-ZSkim-runDependent_"+period+".root",
                      "pileup",
                      "PUDumper/pileup",false);
  */
}
