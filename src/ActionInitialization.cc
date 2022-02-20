#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

namespace B1
{

ActionInitialization::ActionInitialization(DetectorConstruction* detector)
 : G4VUserActionInitialization(),
   fDetector(detector)
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
  RunAction* runAction = new RunAction(fDectector, 0);
  SetUserAction(runAction);
}

void ActionInitialization::Build() const
{
	PrimaryGeneratorAction* primary = new PrimaryGeneratorAction(fDetector);
	SetUserAction(primary);
	
	RunAction* runAction = new RunAction(fDetector, primary );
	SetUserAction(runAction);

	EventAction* eventAction = new EventAction();
	SetUserAction(eventAction);
	
	SteppingAction* steppingAction = new SteppingAction();
	SetUserAction(steppingAction);
}

}
