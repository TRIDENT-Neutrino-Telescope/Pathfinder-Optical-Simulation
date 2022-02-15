#include "SphericalSourceEA.hh"
#include "Control.hh"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"

SphericalSourceEA::SphericalSourceEA()
{
}

SphericalSourceEA::~SphericalSourceEA()
{
}

void SphericalSourceEA::BeginOfEventAction([[maybe_unused]]const G4Event *evt)
{
  G4cout << "++++ Begin of Event "<< evt->GetEventID() <<" ++++" << G4endl;
}

void SphericalSourceEA::EndOfEventAction([[maybe_unused]]const G4Event *evt)
{
}
