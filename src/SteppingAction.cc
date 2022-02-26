#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "Run.hh"

#include "G4ParticleTypes.hh"
#include "G4HadronicProcess.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

#include "G4AnalysisManager.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
: fEventAction(eventAction)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
	Run* run 
		= static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
	       
	// count processes
	// 
	const G4StepPoint* endPoint = step->GetPostStepPoint();
	G4VProcess* process   = 
	                 const_cast<G4VProcess*>(endPoint->GetProcessDefinedStep());
	run->CountProcesses(process);
	
	// check that an real interaction occured (eg. not a transportation)
	G4StepStatus stepStatus = endPoint->GetStepStatus();
	G4bool transmit = (stepStatus==fGeomBoundary || stepStatus==fWorldBoundary);
	if (transmit) return;
	                    
	//real processes : sum track length
	//
	G4double stepLength = step->GetStepLength();
	run->SumTrack(stepLength);
	
	//energy-momentum balance initialisation
	//
	const G4StepPoint* prePoint = step->GetPreStepPoint();
	G4double Q             = - prePoint->GetKineticEnergy();
	G4ThreeVector Pbalance = - prePoint->GetMomentum();
	
	//initialisation of the nuclear channel identification
	//
	G4ParticleDefinition* particle = step->GetTrack()->GetDefinition();
	G4String partName = particle->GetParticleName();
	G4String nuclearChannel = partName;
	G4HadronicProcess* hproc = dynamic_cast<G4HadronicProcess*>(process);
	const G4Isotope* target = NULL;
	if (hproc) target = hproc->GetTargetIsotope();
	G4String targetName = "XXXX";  
	if (target) targetName = target->GetName();
	nuclearChannel += " + " + targetName + " --> ";
	if (targetName == "XXXX") run->SetTargetXXX(true);
	  
	//scattered primary particle (if any)
	//
	//G4AnalysisManager* analysis = G4AnalysisManager::Instance();
	//G4int ih = 1;
	if (step->GetTrack()->GetTrackStatus() == fAlive) {
	  G4double energy = endPoint->GetKineticEnergy();      
	  //analysis->FillH1(ih,energy);
	  //
	  G4ThreeVector momentum = endPoint->GetMomentum();
	  Q        += energy;
	  Pbalance += momentum;
	  //
	  nuclearChannel += partName + " + ";
	}  
	
	//secondaries
	//
	const std::vector<const G4Track*>* secondary 
	                                  = step->GetSecondaryInCurrentStep();  
	for (size_t lp=0; lp<(*secondary).size(); lp++) {
	  particle = (*secondary)[lp]->GetDefinition(); 
	  G4String name   = particle->GetParticleName();
	  G4String type   = particle->GetParticleType();      
	  G4double energy = (*secondary)[lp]->GetKineticEnergy();
	  run->ParticleCount(name,energy);
	  //energy spectrum
	  //ih = 0; 
	  //     if (particle == G4Gamma::Gamma())       ih = 2;
	  //else if (particle == G4Electron::Electron()) ih = 3;
	  //else if (particle == G4Neutron::Neutron())   ih = 4;        
	  //else if (particle == G4Proton::Proton())     ih = 5;
	  //else if (particle == G4Deuteron::Deuteron()) ih = 6;
	  //else if (particle == G4Alpha::Alpha())       ih = 7;       
	  //else if (type == "nucleus")                  ih = 8;
	  //else if (type == "meson")                    ih = 9;
	  //else if (type == "baryon")                   ih = 10;        
	  //if (ih > 0) analysis->FillH1(ih,energy);
	  ////atomic mass
	  //if (type == "nucleus") {
	  //  G4int A = particle->GetAtomicMass();
	  //  analysis->FillH1(13, A);
	  //}
	  //energy-momentum balance
	  G4ThreeVector momentum = (*secondary)[lp]->GetMomentum();
	  Q        += energy;
	  Pbalance += momentum;
	  //count e- from internal conversion together with gamma
	  if (particle == G4Electron::Electron()) particle = G4Gamma::Gamma();
	  //particle flag
	  fParticleFlag[particle]++;
	}
	
	//energy-momentum balance
	G4double Pbal = Pbalance.mag();
	run->Balance(Pbal);
	//ih = 11;
	//analysis->FillH1(ih,Q);
	//ih = 12;
	//analysis->FillH1(ih,Pbal);  
	
	// nuclear channel
	const G4int kMax = 16;  
	const G4String conver[] = {"0","","2 ","3 ","4 ","5 ","6 ","7 ","8 ","9 ",
	                           "10 ","11 ","12 ","13 ","14 ","15 ","16 "};
	std::map<G4ParticleDefinition*,G4int>::iterator ip;               
	for (ip = fParticleFlag.begin(); ip != fParticleFlag.end(); ip++) {
	  particle = ip->first;
	  G4String name = particle->GetParticleName();      
	  G4int nb = ip->second;
	  if (nb > kMax) nb = kMax;   
	  G4String Nb = conver[nb];    
	  if (particle == G4Gamma::Gamma()) {
	   run->CountGamma(nb);
	   Nb = "N ";
	   name = "gamma or e-";
	  } 
	  if (ip != fParticleFlag.begin()) nuclearChannel += " + ";
	  nuclearChannel += Nb + name;
	}
	
	///G4cout << "\n nuclear channel: " << nuclearChannel << G4endl;
	run->CountNuclearChannel(nuclearChannel, Q);
	  
	fParticleFlag.clear();
	            
	// kill event after first interaction
	//
	//G4RunManager::GetRunManager()->AbortEvent();
}
