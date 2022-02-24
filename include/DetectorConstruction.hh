#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

namespace B6
{

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
	virtual void ConstructSDandField();

private:
	void DefineMaterials(); // Define Materials
	G4Material* Galactic;
	G4Material* D2O;
	G4Material* Air;
	G4Material* Water;
	G4Material* Al;
	G4Material* Be;
	G4Material* Pb;
	G4Material* B4C;
	G4Material* Polythelene;
	G4Material* BoronCantainingPolythelene;

  protected:
    G4LogicalVolume* fScoringVolume = nullptr;
};

}

#endif
