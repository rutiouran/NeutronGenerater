#ifndef B1ActionInitialization_h
#define B1ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class DetectorConstruction;

namespace B1
{

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(DetectorConstruction* detector);
    ~ActionInitialization() override;

    void BuildForMaster() const override;
    void Build() const override;

private:
    DetectorConstruction* fDetector;
};

}

#endif
