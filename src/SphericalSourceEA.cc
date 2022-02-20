#include "SphericalSourceEA.hh"
#include "Control.hh"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"

SphericalSourceEA::SphericalSourceEA() {
  fNbOfEvents = Control::Instance()->nEvent;
  fWidthEvents = static_cast<int>(std::log10(fNbOfEvents)) + 1;
}

SphericalSourceEA::~SphericalSourceEA() {}

void SphericalSourceEA::BeginOfEventAction([
    [maybe_unused]] const G4Event *event) {
  ProgressBar(G4cout, event->GetEventID());
}

void SphericalSourceEA::EndOfEventAction([
    [maybe_unused]] const G4Event *event) {}

void SphericalSourceEA::ProgressBar(std::ostream &os, int eventId) {
  constexpr int nStarsTot = 50;
  int nStars = nStarsTot * eventId / fNbOfEvents;
  auto stars = std::string(nStars, '*');

  if (eventId != 0) {
    os << "\r";
  }
  os << "Event [" << std::setw(fWidthEvents) << eventId + 1 << "/"
     << fNbOfEvents << "] " << stars << std::flush;
  if (eventId == fNbOfEvents - 1) {
    os << std::endl;
  }
}