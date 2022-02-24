#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "DetectorConstruction.hh"

/// Action initialization class.

//class DetectorConstruction;

namespace B6
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
