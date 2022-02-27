#include "HistoManager.hh"
#include "G4UnitsTable.hh"

HistoManager::HistoManager()
  : fFileName("output.root")
{
  Book();
}

HistoManager::~HistoManager()
{
}

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);     //enable inactivation of histograms


	// merge the ntuple each threads
  analysisManager->SetNtupleMerging(true);
  
  // Define histograms start values
	analysisManager->CreateNtuple("HeavyWaterTarget", "HeavyWaterTarget");
    analysisManager->CreateNtupleDColumn("Edep");
    analysisManager->CreateNtupleDColumn("TrackLength");
    analysisManager->FinishNtuple();

    analysisManager->CreateNtuple("Neutron", "Neutron");
    analysisManager->CreateNtupleDColumn("neutronEnergy");
    analysisManager->CreateNtupleDColumn("neutronfluxx");
    analysisManager->CreateNtupleDColumn("neutronfluxy");
    analysisManager->CreateNtupleDColumn("neutronfluxz");
    analysisManager->FinishNtuple();

    analysisManager->CreateNtuple("Gamma", "Gamma");
    analysisManager->CreateNtupleDColumn("gammaEnergy");
    analysisManager->CreateNtupleDColumn("gammafluxx");
    analysisManager->CreateNtupleDColumn("gammafluxy");
    analysisManager->CreateNtupleDColumn("gammafluxz");
    analysisManager->FinishNtuple();
}
