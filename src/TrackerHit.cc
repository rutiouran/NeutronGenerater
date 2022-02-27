#include "TrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<TrackerHit>* TrackerHitAllocator=0;

TrackerHit::TrackerHit()
 : G4VHit(),
   fPos(G4ThreeVector()),
   fKinE(0.)
{}

TrackerHit::~TrackerHit() {}

TrackerHit::TrackerHit(const TrackerHit& right)
  : G4VHit()
{
  fPos       = right.fPos;
  fKinE	     = right.fKinE;
}

const TrackerHit& TrackerHit::operator=(const TrackerHit& right)
{
  fPos       = right.fPos;
  fKinE      = right.fKinE;

  return *this;
}

G4bool TrackerHit::operator==(const TrackerHit& right) const
{
  return ( this == &right ) ? true : false;
}
