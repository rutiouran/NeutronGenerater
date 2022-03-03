#include "DetectorConstruction.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"

// Sensitive Detector
#include "G4SDManager.hh"
#include "HeavyWaterSD.hh"
#include "TrackerSD.hh"

// Biasing
//#include "GB01BOptrMultiParticleChangeCrossSection.hh"

// Geometry
#include "G4Box.hh"
#include "G4EllipticalTube.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"

// Color
#include "G4VisAttributes.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),
 fMaterial(0)
{
	SetMaterial("HeavyWater");
}

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
    D2O = new G4Material("HeavyWater", 1.10*g/cm3, ncomponents=2,
                        kStateLiquid, 293.15*kelvin, 1*atmosphere);
    D2O->AddElement(D  , natoms=2);
    D2O->AddElement(elO, natoms=1);
	fMaterial = D2O;

//	//D2
//	//D2O = new G4Material("HeavyWater", 0.000167*g/cm3, ncomponents=1,
//	D2O = new G4Material("HeavyWater", 0.1*g/cm3, ncomponents=1);
//    //D2O = new G4Material("HeavyWater", 0.00018*g/cm3, ncomponents=1,
//    D2O->AddElement(D, natoms=1);
//	fMaterial = D2O;

	std::cout << *(G4Material::GetMaterialTable()) << std::endl;    //Get material table
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	// Define the materials
	DefineMaterials();	

	// Get nist material manager
	//G4NistManager* nist = G4NistManager::Instance();
	
	// Envelope parameters
	//
	G4double env_sizeXY = 3.0*m, env_sizeZ = 3.0*m;
	
	// Option to switch on/off checking of volumes overlaps
	//
	G4bool checkOverlaps = true;

	//The parameters of the shielding external layer: Pb
	G4double R_Pb         = 10.*cm;
	G4double DZ_Pb_front  = 10.*cm;
	G4double DZ_Pb_behind = 10.*cm;
	
	//The parameters of the shielding internal layer: HDPE
	G4double R_Po         = 30.*cm;
	G4double DZ_Po_front  = 30.*cm;
	G4double DZ_Po_behind = 40.*cm;
	
	//The parameter of the vacuum chamber 
	G4double vach_pRMin = 0.*mm; 
	G4double vach_pRMax = 250.*mm;
	G4double vach_pDZ = 200.*mm;
	G4double vach_pSshi = 0.*deg;
	G4double vach_pDshi = 360.*deg;
	
	G4Tubs* VacuumChanberS =		//Soild vacuum chamber
			 new G4Tubs("VacuumChanber",
						vach_pRMin, vach_pRMax, vach_pDZ/2., vach_pSshi, vach_pDshi);
	
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
    //Tracker
    //
    G4double Trac_RMax = 1.00001*m;
    G4double Trac_RMin = 1.00000*m;
    G4double Trac_SPhi = 0.*deg;
    G4double Trac_STheta = 0.*deg;
    G4double Trac_DPhi = 360.*deg;
    G4double Trac_DTheta = 360.*deg;

    G4ThreeVector tracker_pos = G4ThreeVector(0.*mm, 150.*mm, -4.6*mm);

    G4Sphere* TrackerS =
        new G4Sphere("Tracker",
        Trac_RMin, Trac_RMax, Trac_SPhi, Trac_DPhi, Trac_STheta, Trac_DTheta);

    G4LogicalVolume* TrackerLV =
        new G4LogicalVolume(TrackerS,	          //its solid
                            Galactic,             //its material
                            "Tracker");           //its name

          new G4PVPlacement(0,                    //no rotation
              				tracker_pos,
                            TrackerLV,	          //its logical volume
                            "Tracker",            //its name
                            logicEnv,  		      //its mather volume
                            false,                //no boolean operation
                            0,                    //copy number
                            checkOverlaps);       //overlaps checking
	/*
	//
	//Shield(material:Boron containing polyethylene + Pb)
	//
	//shie_Po_out
	G4double shie_Po_out_pRMin = 0.00*mm;
	G4double shie_Po_out_pRMax = 250.0*mm+R_Po;
	G4double shie_Po_out_pDZ   = 200.0*mm+DZ_Po_front+DZ_Po_behind;
	G4double shie_Po_out_PSshi = 0.*deg;
	G4double shie_Po_out_PDshi = 360.*deg;
	
	G4Tubs* solidShie_Po_out = 
	  new G4Tubs("Shie_Po_out",	//its name
	  shie_Po_out_pRMin, shie_Po_out_pRMax, 0.5*shie_Po_out_pDZ, shie_Po_out_PSshi, shie_Po_out_PDshi);		//its size
	
	//shie_Po_windows
	G4double shie_Po_win_DX = 1.5*mm;
	G4double shie_Po_win_DY = 3.0*mm;
	G4double shie_Po_win_DZ = DZ_Po_front/2;
	
	G4EllipticalTube* solidShie_Po_Wind =
	       new G4EllipticalTube("Shie_Po_Wind",					//its name
	       shie_Po_win_DX, shie_Po_win_DY, shie_Po_win_DZ);		//its size
	
	G4ThreeVector shie_Po_wind_pos = G4ThreeVector(0., 150.*mm, -100.*mm-DZ_Po_behind/2);
	
	//Shie_Po
	G4ThreeVector shie_Po_in_pos = G4ThreeVector(0., 0., (DZ_Po_front-DZ_Po_behind)/2.);
	G4ThreeVector shie_Po_pos = G4ThreeVector(0., 0., (DZ_Po_behind-DZ_Po_front)/2.);
	
	G4SubtractionSolid* solidShie_Po1 =
	  new G4SubtractionSolid("Shie_Po1",					//its solid name
	  		       solidShie_Po_out,						//solid1
	  		       solidvach,								//solid2
	  		       0,										//solid2_rotation
	  		       shie_Po_in_pos);							//solid2_tansition
	
	G4SubtractionSolid* solidShie_Po =
	       new G4SubtractionSolid("Shie_Po",				//its solid name
	                              solidShie_Po1,			//solid1
	                              solidShie_Po_Wind,		//solid2
								  0,						//solid2_rotation
	  		       				  shie_Po_wind_pos);		//solid2_tansition
	
	G4LogicalVolume* logicShie_Po =
	  new G4LogicalVolume(solidShie_Po,	//its solid
						  shie_mat,		//its material
						  "Shie_Po");	//its name
	
	 new G4PVPlacement(0,							//no rotation
	                   shie_Po_pos,					//at (0., 0., (DZ_Po_behind-DZ_Po_front)/2.)
	                   logicShie_Po,				//its logical volume
	                   "Shie_Po",					//its name
	                   logicWorld,					//its mather volume
	                   true,						//boolean operation:subtraction
	                   0,							//copy number
	                   checkOverlaps);				//overlaps checking
  
	//shie_Pb_out
	G4double shie_Pb_out_pRMin = 0.00*mm;
	G4double shie_Pb_out_pRMax = 250.0*mm+R_Pb+R_Po;
	G4double shie_Pb_out_pDZ   = 200.0*mm+DZ_Po_front+DZ_Po_behind+DZ_Pb_front+DZ_Pb_behind;
	G4double shie_Pb_out_PSshi = 0.*deg;
	G4double shie_Pb_out_PDshi = 360.*deg;
	
	G4Tubs* solidShie_Pb_out =
	       new G4Tubs("Shie_Pb_out",         //it name
	       shie_Pb_out_pRMin, shie_Pb_out_pRMax, 0.5*shie_Pb_out_pDZ, shie_Pb_out_PSshi, shie_Pb_out_PDshi); //its size
	
	//shie_Pb_windows
	G4double shie_Pb_win_DX = 1.5*mm;
	G4double shie_Pb_win_DY = 3.0*mm;
	G4double shie_Pb_win_DZ = DZ_Pb_front/2;
	
	G4EllipticalTube* solidShie_Pb_Wind =
	       new G4EllipticalTube("Shie_Pb_Wind",       //its name
	       shie_Pb_win_DX, shie_Pb_win_DY, shie_Pb_win_DZ);               //its size
	
	G4ThreeVector shie_Pb_wind_pos = G4ThreeVector(0., 150.*mm, -100.*mm-DZ_Po_front/2-DZ_Po_behind/2-DZ_Pb_behind/2); 
	
	//Shie_Pb
	G4ThreeVector shie_Pb_in_pos = G4ThreeVector(0., 0., (DZ_Pb_front - DZ_Pb_behind)/2.);
	G4ThreeVector shie_Pb_pos = G4ThreeVector(0., 0., (DZ_Po_behind + DZ_Pb_behind - DZ_Po_front - DZ_Pb_front)/2.);
	
	G4SubtractionSolid* solidShie_Pb1 =
	       new G4SubtractionSolid("Shie_Pb1",              //its solid name
	                              solidShie_Pb_out,        //solid1
	                              solidShie_Po_out,        //solid2
	  		       0,			//solid2_rotation
	  		       shie_Pb_in_pos);		//solid2_transition
	
	G4SubtractionSolid* solidShie_Pb =
	       new G4SubtractionSolid("Shie_Pb",               //its solid name
	                              solidShie_Pb1,           //solid1
	                              solidShie_Pb_Wind,       //solid2
	                              0,			//solid2_rotation
	                              shie_Pb_wind_pos);	//solid2_transition
	
	  G4LogicalVolume* logicShie_Pb =
	         new G4LogicalVolume(solidShie_Pb,       //its solid
	                             shie_mat2_Pb,       //its material
	                             "Shie_Pb");         //its name
	 
	   new G4PVPlacement(0,                          //no rotation
	                     shie_Pb_pos,                //at (0., 0., (DZ_Po_front + DZ_Pb_front - DZ_Po_behind - DZ_Pb_behind)/2.)
	                     logicShie_Pb,               //its logical volume
	                     "Shie_Pb",  	        //its name
	                     logicWorld,                 //its mather volume
	                     true,                       //boolean operation:subtraction
	                     0,                          //copy number
	                     checkOverlaps);             //overlaps checking
*/
	//
	//vacuum chanber
	//
	
	G4LogicalVolume* VacuumChanberLV =
	     new G4LogicalVolume(VacuumChanberS,		//its solid
							 Galactic,				//its material
							 "VacuumChanber");		//its name
	 
	new G4PVPlacement(0,				//no rotation
	  	    G4ThreeVector(),			//at(0,0,0)
	        VacuumChanberLV,			//its logical volume
	  	    "VacuumChanber",			//its name
	  	    logicEnv,					//its mather volume
	  	    false,						//no boolean operation
	  	    0,							//copy number
	  	    checkOverlaps);				//overlaps checking
	
	//
	//hermetic window
	//
	
	G4EllipticalTube* HermeticWindowS =
	  new G4EllipticalTube("WermeticWindow", 
							1.5*mm, 3.0*mm, 0.05*mm);
	
	G4LogicalVolume* HermeticWindowLV =
	  new G4LogicalVolume(HermeticWindowS,				//its solid
						  Be,							//its material
						  "WermeticWindow");			//its name
	
	  new G4PVPlacement(0,											//no rotation
	  	    		    G4ThreeVector(0, 150*mm, -5.05*mm),			//at(0, 150.*mm, -5.05*mm)
	  	    		    HermeticWindowLV,							//its logical volume
	        		    "WermeticWindow",							//its name
	  	    		    VacuumChanberLV,							//its mather volume
	  	    		    false,										//no boolean operation
	  	    		    0,											//copy number
	                    checkOverlaps);								//overlaps checking

	//
	//Al chanber
	//
	
	G4Tubs* AlChanberS =
		new G4Tubs("AlChanber",
				0.*mm, 175.*mm, 5.0*mm, 0.*deg, 360*deg);
	
	G4LogicalVolume* AlChanberLV = 
		new G4LogicalVolume(AlChanberS,			//its solid
							Al, 				//its material
							"AlChanber");		//its name
	
	    new G4PVPlacement(0,							//no rotation
						G4ThreeVector(),				//at(0, 0, 0)
						AlChanberLV,			 		//its logical volume
						"AlChanber",					//its name
						VacuumChanberLV, 				//its mather volume
						false, 							//no boolean operation
						0,								//copy number
						checkOverlaps);					//overlaps checking

	//
	//Cooling Water
	//
	
	G4ThreeVector CoWa_pos = G4ThreeVector(0.*mm, 150.*mm, 2.5*mm);
	
	G4EllipticalTube* CoolingWaterS =
		new G4EllipticalTube("CoolingWater",
							6.0*mm, 12.0*mm, 1.5*mm);
	
	G4LogicalVolume* CoolingWaterLV =
	  new G4LogicalVolume(CoolingWaterS,		//its solid
	  	 			      Water,				//its material
	                      "CoolingWater");		//its name
	
	  new G4PVPlacement(0,											//no rotation
	  	     			G4ThreeVector(0.*mm, 150.*mm, 2.5*mm),		//at(0.*mm, 150.*mm, 2.5*mm)
	  	     			CoolingWaterLV,								//its logical volume
	  	     			"CoolingWater",								//its name
	  	     			AlChanberLV,								//its mather volume
	  	     			false,										//no boolean operation
	  	     			0,											//copy number
	  	     			checkOverlaps);								//overlaps checking


	//
    //Heavy Water
    //

    G4EllipticalTube* HeavyWaterS =
		new G4EllipticalTube("HeavyWater", 1.5*mm, 3.0*mm, 0.4*mm);
		//new G4EllipticalTube("HeavyWater", 20*cm, 30*cm, 20*cm);

    G4LogicalVolume* HeavyWaterLV =
		new G4LogicalVolume(HeavyWaterS,		//its solid
							D2O,				//its material
							"HeavyWater");		//its name

		new G4PVPlacement(0,									//no rotation
						G4ThreeVector(0.*mm, 150.*mm, -4.6*mm),	//at (0, 150.*mm, -4.6*mm)
						HeavyWaterLV,							//its logical
						"HeavyWater",							//its name
						AlChanberLV,							//its mother  volume
						false,									//no boolean operation
						0,										//copy numble
						checkOverlaps);							//overlaps checking

	G4cout << G4endl
		   << "-----------------------------------" << G4endl
		   << "The information of the volume : " << G4endl
		   << "(1) Name : " << HeavyWaterLV->GetName() << G4endl
		   << "(2) Mass : " << G4BestUnit(HeavyWaterLV->GetMass(), "Mass") << G4endl
		   << "-----------------------------------" << G4endl << G4endl;

	//
	//Get Color
	//
	G4VisAttributes* heavywaterVisAtt   = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
	G4VisAttributes* coolingWaterVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));

	HeavyWaterLV   ->SetVisAttributes(heavywaterVisAtt);
	CoolingWaterLV ->SetVisAttributes(coolingWaterVisAtt);

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

	auto trackerSD
        = new TrackerSD("NeutronGenerator/TrackerSD", "TrackerHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(trackerSD);
    SetSensitiveDetector("Tracker", trackerSD);

//	// -- Fetch volume for biasing:
//	G4LogicalVolume* logicTest = G4LogicalVolumeStore::GetInstance()->GetVolume("HeavyWater");
//	
//	// ----------------------------------------------
//	// -- operator creation and attachment to volume:
//	// ----------------------------------------------
//	GB01BOptrMultiParticleChangeCrossSection* testMany =
//	  new GB01BOptrMultiParticleChangeCrossSection();
//	//testMany->AddParticle("gamma");
//	//testMany->AddParticle("neutron");
//	testMany->AddParticle("deuteron");
//	testMany->AttachTo(logicTest);
//	G4cout << "-------------------------------------" << G4endl
//		   << " Attaching biasing operator " << testMany->GetName()
//	       << " to logical volume " << logicTest->GetName()
//	       << "-------------------------------------" << G4endl
//		   << G4endl;
}

void DetectorConstruction::SetMaterial(G4String materialChoice)
{
	// search the material by its name
	G4Material* pttoMaterial =
	   G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
	
	if (pttoMaterial) {
	  if(fMaterial != pttoMaterial) {
	    fMaterial = pttoMaterial;
	    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	  }
	} else {
	  G4cout << "\n--> warning from DetectorConstruction::SetMaterial : "
	         << materialChoice << " not found" << G4endl;
	}
}
