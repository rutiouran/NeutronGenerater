#include "EventAction.hh"
#include "RunAction.hh"

#include "G4SDManager.hh"
#include "HeavyWaterHit.hh"
#include "HeavyWaterSD.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4UnitsTable.hh"
#include "G4AnalysisManager.hh"

namespace B6
{

EventAction::EventAction(RunAction* runAction)
: fRunAction(runAction),
  fheavywaterHCID(-1)
{}

EventAction::~EventAction()
{}

HeavyWaterHitsCollection* EventAction::GetHeavyWaterHitsCollection(G4int hcID, const G4Event* event) const
{
	auto hitsCollection
		= static_cast<HeavyWaterHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));
	
	if (!hitsCollection)
	{
	  G4ExceptionDescription msg;
	  msg << "Cannot access hitsCollection ID " << hcID;
	  G4Exception("B4cEventAction::GetHitsCollection()",
	    "MyCode0003", FatalException, msg);
	}
	
	return hitsCollection;
}

void EventAction::BeginOfEventAction(const G4Event*)
{}

void EventAction::EndOfEventAction(const G4Event* event)
{
	// Get hits collections IDs (only once)
	if(fheavywaterHCID == -1)
	{
		fheavywaterHCID = G4SDManager::GetSDMpointer()->GetCollectionID("HeavyWaterHitsCollection");
	}
	
	// Get hits collection
	auto heavywaterHC = GetHeavyWaterHitsCollection(fheavywaterHCID, event);

	// Fill histograms, ntuple
	//
	
	// get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();
	
	// fill ntuple
	analysisManager->FillNtupleDColumn(0, 0, (*heavywaterHC)[0]->GetEdep());
	analysisManager->FillNtupleDColumn(0, 1, (*heavywaterHC)[0]->GetTrackLength());
	analysisManager->AddNtupleRow(0);
}
}
