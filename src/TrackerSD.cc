#include "TrackerSD.hh"
#include "TrackerHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"

TrackerSD::TrackerSD(const G4String& name,
                         const G4String& hitsCollectionName) 
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

TrackerSD::~TrackerSD() 
{}

void TrackerSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection 
    = new TrackerHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool TrackerSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
//  if(pid==2112 && step->IsFirstStepInVolume())
//  {
//  TrackerHit* newHit = new TrackerHit();
//
//  newHit->SetPos(step->GetPostStepPoint()->GetPosition());
//  newHit->SetKineticEnergy(step->GetTrack()->GetKineticEnergy());
//
//  fHitsCollection->insert(newHit);
//  }

	G4int pid = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
	//G4String particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
	G4ThreeVector localPos = step->GetTrack()->GetPosition();

	// AnalysisManager
	//

	auto analysisManager = G4AnalysisManager::Instance();
	
	if(pid == 2112 && step->IsFirstStepInVolume()) // Neutron
	{
		analysisManager->FillNtupleDColumn(1, 0, step->GetTrack()->GetKineticEnergy());
		analysisManager->FillNtupleDColumn(1, 1, localPos.x()/CLHEP::m);
		analysisManager->FillNtupleDColumn(1, 2, localPos.y()/CLHEP::m);
		analysisManager->FillNtupleDColumn(1, 3, localPos.z()/CLHEP::m);
		analysisManager->AddNtupleRow(1);
	}

	if(pid == 22 && step->IsFirstStepInVolume()) // Gamma
	{
		analysisManager->FillNtupleDColumn(2, 0, step->GetTrack()->GetKineticEnergy());
        analysisManager->FillNtupleDColumn(2, 1, localPos.x()/CLHEP::m);
        analysisManager->FillNtupleDColumn(2, 2, localPos.y()/CLHEP::m);
        analysisManager->FillNtupleDColumn(2, 3, localPos.z()/CLHEP::m);
        analysisManager->AddNtupleRow(2);
	}

	return true;
}

void TrackerSD::EndOfEvent(G4HCofThisEvent*)
{}
