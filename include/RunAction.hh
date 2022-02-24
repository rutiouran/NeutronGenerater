#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

class G4Run;

class DetectorConstruction;
class PrimaryGeneratorAction;

namespace B6
{

class RunAction : public G4UserRunAction
{
  public:
    RunAction(DetectorConstruction*, PrimaryGeneratorAction*);
    ~RunAction() override;

    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;

private:
    DetectorConstruction*      fDetector;
    PrimaryGeneratorAction*    fPrimary;

//    void AddEdep (G4double edep);
//
//  private:
//    G4Accumulable<G4double> fEdep = 0.;
//    G4Accumulable<G4double> fEdep2 = 0.;
};

}

#endif

