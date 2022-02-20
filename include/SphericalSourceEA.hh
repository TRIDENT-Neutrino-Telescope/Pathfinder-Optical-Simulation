#pragma once

#include "G4UserEventAction.hh"
#include "globals.hh"

class SphericalSourceEA : public G4UserEventAction {
public:
  SphericalSourceEA();
  ~SphericalSourceEA();
  virtual void BeginOfEventAction(const G4Event *);
  virtual void EndOfEventAction(const G4Event *);

private:
  int fNbOfEvents;
  int fWidthEvents;
  void ProgressBar(std::ostream &os, int eventId);
};