#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "HeavyWaterHit.hh"

/// Event action class
///

namespace B6
{

class RunAction;

class EventAction : public G4UserEventAction
{
public:
    EventAction(RunAction* runAction);
    ~EventAction() override;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

private:
	HeavyWaterHitsCollection* GetHeavyWaterHitsCollection(G4int hcID, const G4Event* event) const;

	G4int fheavywaterHCID;

private:
    RunAction* fRunAction = nullptr;
};

}

#endif
