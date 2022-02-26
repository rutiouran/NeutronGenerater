#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4VProcess.hh"

//#include "DetectorConstruction.hh"
//#include "PrimaryGeneratorAction.hh"
//#include "Run.hh"
//#include "G4Run.hh"
#include <map>

class DetectorConstruction;
class PrimaryGeneratorAction;
class Run;
class HistoManager;
class G4Run;

class RunAction : public G4UserRunAction
{
  public:
    RunAction(DetectorConstruction*, PrimaryGeneratorAction*);
    ~RunAction() override;

    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;

	virtual G4Run* GenerateRun();
	void SetPrintFlag(G4bool);

private:
    DetectorConstruction*      fDetector;
    PrimaryGeneratorAction*    fPrimary;
	HistoManager*              fHistoManager;
	Run*					   fRun;

	G4bool   fPrint;      //optional printing
};

#endif
