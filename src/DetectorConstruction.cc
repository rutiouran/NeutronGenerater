#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"

#include "G4SDManager.hh"
#include "HeavyWaterSD.hh"

#include "G4Box.hh"
#include "G4EllipticalTube.hh"

#include "G4VisAttributes.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

namespace B1
{

DetectorConstruction::DetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

void DetectorConstruction::DefineMaterials()
{
	G4double z, a, density, fractionmass;
    G4String name, symbol;
    G4int ncomponents, natoms;

    G4NistManager* nist = G4NistManager::Instance();

	// Element
    a = 10.811*g/mole;
    G4Element* elB = new G4Element(name="Boron", symbol="B", z=5., a);

    a = 12.01*g/mole;
    G4Element* elC = new G4Element(name="Carbon", symbol="C", z=6., a);

    a = 1.01*g/mole;
    G4Element* elH = new G4Element(name="Hydrogen", symbol="H" , z= 1., a);

    a = 16.00*g/mole;
    G4Element* elO = new G4Element(name="oxygen", symbol="O", z=8., a);

	Galactic = nist->FindOrBuildMaterial("G4_Galactic");
	Air 	 = nist->FindOrBuildMaterial("G4_AIR");
	Water	 = nist->FindOrBuildMaterial("G4_WATER");
	Al		 = nist->FindOrBuildMaterial("G4_Al");
	Pb		 = nist->FindOrBuildMaterial("G4_Pb");
	Be		 = nist->FindOrBuildMaterial("G4_Be");

	// B4C  
    density = 2.52*g/cm3;
    B4C = new G4Material(name="B4C", density, ncomponents=2);
    B4C->AddElement(elB, natoms=4);
    B4C->AddElement(elC, natoms=1);

    // Polythelene
    density = 0.94*g/cm3;
    Polythelene = new G4Material(name="Polythelene", density, ncomponents=2);
    Polythelene->AddElement(elC, natoms=2);
    Polythelene->AddElement(elH, natoms=4);

    //// Boron cantaining polythelene
    double BRate = 0.08;
    density = (BRate*2.52 + (1-BRate)*0.94)*g/cm3;
    BoronCantainingPolythelene = new G4Material("Boron cantaining polythelene", density, ncomponents=2);
    BoronCantainingPolythelene->AddMaterial(Polythelene, fractionmass = (1-BRate)*100.*perCent);
    BoronCantainingPolythelene->AddMaterial(B4C, 		 fractionmass = BRate*100.*perCent);

	//D2O
    G4Isotope* H2 = new G4Isotope("H2",1,2);
    G4Element* D  = new G4Element("TS_D_of_Heavy_Water", "D", 1);
    D->AddIsotope(H2, 100*perCent);
    D2O = new G4Material("HeavyWater", 1.11*g/cm3, ncomponents=2,
    //D2O = new G4Material("HeavyWater", 0.00018*g/cm3, ncomponents=1,
                        kStateLiquid, 293.15*kelvin, 1*atmosphere);
    D2O->AddElement(D, natoms=2);
    D2O->AddElement(elO, natoms=1);

	std::cout << *(G4Material::GetMaterialTable()) << std::endl;    //Get material table
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	// Define the materials
	DefineMaterials();	

	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();
	
	// Envelope parameters
	//
	G4double env_sizeXY = 3.0*m, env_sizeZ = 3.0*m;
	
	// Option to switch on/off checking of volumes overlaps
	//
	G4bool checkOverlaps = true;
	
	//
	// World
	//
	G4double world_sizeXY = 1.2*env_sizeXY;
	G4double world_sizeZ  = 1.2*env_sizeZ;
	
	G4Box* solidWorld =
	  new G4Box("World",                       //its name
	     0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
	
	G4LogicalVolume* logicWorld =
	  new G4LogicalVolume(solidWorld,          //its solid
	                      Galactic,            //its material
	                      "World");            //its name
	
	G4VPhysicalVolume* physWorld =
	  new G4PVPlacement(0,                     //no rotation
	                    G4ThreeVector(),       //at (0,0,0)
	                    logicWorld,            //its logical volume
	                    "World",               //its name
	                    0,                     //its mother  volume
	                    false,                 //no boolean operation
	                    0,                     //copy number
	                    checkOverlaps);        //overlaps checking
	
	//
	// Envelope
	//
	G4Box* solidEnv =
	  new G4Box("Envelope",                    //its name
	      0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
	
	G4LogicalVolume* logicEnv =
	  new G4LogicalVolume(solidEnv,            //its solid
	                      Galactic,            //its material
	                      "Envelope");         //its name
	
	new G4PVPlacement(0,                       //no rotation
	                  G4ThreeVector(),         //at (0,0,0)
	                  logicEnv,                //its logical volume
	                  "Envelope",              //its name
	                  logicWorld,              //its mother  volume
	                  false,                   //no boolean operation
	                  0,                       //copy number
	                  checkOverlaps);          //overlaps checking

	//
    //Heavy Water
    //

    G4EllipticalTube* HeavyWaterS =
		new G4EllipticalTube("HeavyWater", 1.5*mm, 3.0*mm, 0.4*mm);

    G4LogicalVolume* HeavyWaterLV =
		new G4LogicalVolume(HeavyWaterS,		//its solid
							D2O,				//its material
							"HeavyWater");		//its name

		new G4PVPlacement(0,									//no rotation
						G4ThreeVector(0.*mm, 150.*mm, -4.6*mm),	//at (0, 150.*mm, -4.6*mm)
						HeavyWaterLV,							//its logical
						"HeavyWater",							//its name
						logicEnv,								//its mother  volume
						false,									//no boolean operation
						0,										//copy numble
						checkOverlaps);							//overlaps checking

	//
	//Get Color
	//
	G4VisAttributes* heavywaterVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,0.0));

	HeavyWaterLV ->SetVisAttributes(heavywaterVisAtt);

	//
	//always return the physical World
	//
	return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
	G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

    //
    //Sensitive detectors
    //
    auto heavyWaterSD
        = new HeavyWaterSD("NeutronGenerator/HeavyWaterSD", "HeavyWaterHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(heavyWaterSD);
    SetSensitiveDetector("HeavyWater", heavyWaterSD);
}
}
