#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "DetectorConstruction.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"

#include "QBBC.hh"
#include "FTFP_BERT.hh"
#include "PhysicsList.hh"
#include "G4ParticleHPManager.hh"
//#include "G4GenericBiasingPhysics.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

int main(int argc,char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = nullptr;
  if ( argc == 1 ) { ui = new G4UIExecutive(argc, argv); }

  // Optionally: choose a different Random engine...
  // G4Random::setTheEngine(new CLHEP::MTwistEngine);

  //use G4SteppingVerboseWithUnits
  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);

  // Construct the default run manager
  //
  auto* runManager =
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // Set mandatory initialization classes
  //
  // Detector construction
  //set mandatory initialization classes
  DetectorConstruction* det = new DetectorConstruction;
  runManager->SetUserInitialization(det);

  // Physics list
  G4VModularPhysicsList* physicsList = new PhysicsList;
  physicsList->SetVerboseLevel(1);

  // Replaced HP environmental variables with C++ calls
  G4ParticleHPManager::GetInstance()->SetSkipMissingIsotopes	( false );
  G4ParticleHPManager::GetInstance()->SetDoNotAdjustFinalState	( true );
  G4ParticleHPManager::GetInstance()->SetUseOnlyPhotoEvaporation( true );
  G4ParticleHPManager::GetInstance()->SetNeglectDoppler			( false );
  G4ParticleHPManager::GetInstance()->SetProduceFissionFragments( false );
  G4ParticleHPManager::GetInstance()->SetUseWendtFissionModel	( false );
  G4ParticleHPManager::GetInstance()->SetUseNRESP71Model		( false );

//  // -- and augment it with biasing facilities:
//  G4GenericBiasingPhysics* biasingPhysics = new G4GenericBiasingPhysics();
//  if ( true )
//    {
//      //biasingPhysics->Bias("gamma");
//      //biasingPhysics->Bias("neutron");
//      biasingPhysics->Bias("deuteron");
//      //biasingPhysics->Bias("kaon0L");
//      //biasingPhysics->Bias("kaon0S");
//      physicsList->RegisterPhysics(biasingPhysics);
//      G4cout << "      ********************************************************* " << G4endl;
//      G4cout << "      ********** processes are wrapped for biasing ************ " << G4endl;
//      G4cout << "      ********************************************************* " << G4endl;
//    }
//  else
//    {
//      G4cout << "      ************************************************* " << G4endl;
//      G4cout << "      ********** processes are not wrapped ************ " << G4endl;
//      G4cout << "      ************************************************* " << G4endl;
//    }

  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization( det ));

  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}
