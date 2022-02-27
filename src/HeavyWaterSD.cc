#include "HeavyWaterSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"

HeavyWaterSD::HeavyWaterSD( const G4String& name, 
                            const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHitsCollection(nullptr)
{
  collectionName.insert(hitsCollectionName);
}

HeavyWaterSD::~HeavyWaterSD() 
{}

void HeavyWaterSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection 
    = new HeavyWaterHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  // Create hits
  fHitsCollection->insert(new HeavyWaterHit());
}

G4bool HeavyWaterSD::ProcessHits(G4Step* step, 
                                     G4TouchableHistory*)
{  
	//Energy deposit
	auto edep = step->GetTotalEnergyDeposit();
	
	//Step length
	G4double stepLength = step->GetStepLength();
	
	//if(edep == 0. && stepLength == 0.) return false;
	
	G4int layerNumber = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
	auto hit = (*fHitsCollection)[layerNumber];
	if(!hit)
	{
	  G4ExceptionDescription msg;
	  msg << "Cannot access hit " << "0";
	  G4Exception("B4cCalorimeterSD::ProcessHits()",
	    "MyCode0004", FatalException, msg);
	}

	if(edep != 0. && stepLength != 0. && step->GetTrack()->GetTrackID() == 1)
	hit->Add(edep, stepLength);

	return true;

  //Fill histograms and ntuple
//  auto analysisManager = G4AnalysisManager::Instance();
//
//  G4int pid = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
//  
//  G4int ParentID = step->GetTrack()->GetParentID();
//  if(pid == 2112)
//  G4cout << "pid: " << pid << ", edep: " << edep << ", step: " << step << G4endl; 
//  
//  if(pid<1e9)
//  {
//     analysisManager->FillH1(2, pid);
//     G4cout << "Fill pid" << G4endl;
//  }
//  else
//  {
//     analysisManager->FillH1(3, pid);
//     G4cout << "Fill pid" << G4endl;
//  }
//
//  analysisManager->FillNtupleDColumn(2, 0, pid);
//  analysisManager->AddNtupleRow(2);

  //Avoid double counting
  //if(pid == 2112) step->GetTrack()->SetTrackStatus(fStopAndKill);
}

void HeavyWaterSD::EndOfEvent(G4HCofThisEvent*)
{}
