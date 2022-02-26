#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "HistoManager.hh"

#include "G4RunManager.hh"
#include "Run.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim)
:G4UserRunAction(),
 fDetector(det), fPrimary(prim), fHistoManager(0),
 fRun(0), fPrint(true)
{
	fHistoManager = new HistoManager();
}

RunAction::~RunAction()
{
	delete fHistoManager;
}

G4Run* RunAction::GenerateRun()
{
  fRun = new Run(fDetector);
  return fRun;
}

void RunAction::BeginOfRunAction(const G4Run*)
{
	// show Rndm status
	if (isMaster) G4Random::showEngineStatus();
	
	// keep run condition
	if(fPrimary)
	{
	  G4ParticleDefinition* particle
	    = fPrimary->GetParticleGun()->GetParticleDefinition();
	  G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
	  fRun->SetPrimary(particle, energy);
	}

	//histograms
	//
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	analysisManager->OpenFile();

//	//inform the runManager to save random number seed
//	auto analysisManager = G4AnalysisManager::Instance();
}

void RunAction::EndOfRunAction(const G4Run* run)
{
	if (isMaster) fRun->EndOfRun(fPrint);

	//save histograms      
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	analysisManager->Write();
	analysisManager->CloseFile();
	
	// show Rndm status
	//if (isMaster) G4Random::showEngineStatus();
}
