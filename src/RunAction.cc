#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
// #include "Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4AnalysisManager.hh"

namespace B6
{

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim)
:G4UserRunAction(),
 fDetector(det), fPrimary(prim)
{
	// Create analysis manager
	auto analysisManager = G4AnalysisManager::Instance();
	G4cout << "Using" << analysisManager->GetType() << G4endl;

	// Create directories
	analysisManager->SetVerboseLevel(1);

	// merge the ntuple each threads
	analysisManager->SetNtupleMerging(true);

	// Book histograms, ntuples
	//

	// Ntuple
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

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
	//inform the runManager to save random number seed
	//G4RunManager::GetRunManager()->SetRandomNumberStore(true);
	
	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();
	
	// Open an output file
	//
	G4String fileName = "output.root";
	// Other supported output types:
	// G4String fileName = "B4.csv";
	// G4String fileName = "B4.hdf5";
	// G4String fileName = "B4.xml";
	analysisManager->OpenFile(fileName);
	G4cout << "Using " << analysisManager->GetType() << G4endl;
}

void RunAction::EndOfRunAction(const G4Run* run)
{
	auto analysisManager = G4AnalysisManager::Instance();

	analysisManager->Write();
	analysisManager->CloseFile();
}
}
