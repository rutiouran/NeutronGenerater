#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "HistoManager.hh"
// #include "Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

namespace B6
{

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim)
:G4UserRunAction(),
 fDetector(det), fPrimary(prim), fHistoManager(0)
{
	fHistoManager = new HistoManager();
}

RunAction::~RunAction()
{
	delete fHistoManager;
}

void RunAction::BeginOfRunAction(const G4Run*)
{
	//histograms
	//
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	analysisManager->OpenFile();

//	//inform the runManager to save random number seed
//	auto analysisManager = G4AnalysisManager::Instance();
}

void RunAction::EndOfRunAction(const G4Run* run)
{
	//save histograms      
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	analysisManager->Write();
	analysisManager->CloseFile();
	
	// show Rndm status
	//if (isMaster) G4Random::showEngineStatus();
}
}
