#ifndef HeavyWaterSD_h
#define HeavyWaterSD_h 1

#include "G4VSensitiveDetector.hh"
#include "HeavyWaterHit.hh"


class G4Step;
class G4HCofThisEvent;

class HeavyWaterSD : public G4VSensitiveDetector
{
  public:
    HeavyWaterSD(const G4String& name, 
                 const G4String& hitsCollectionName);
    virtual ~HeavyWaterSD();
  
    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
    HeavyWaterHitsCollection* fHitsCollection;
};

#endif
