#pragma once

#include "G4UserSteppingAction.hh"
#include "globals.hh"

/* Do nothing in stepping action
*/
class SphericalSourceSA : public G4UserSteppingAction
{
public:
  SphericalSourceSA();
  virtual ~SphericalSourceSA();

  virtual void UserSteppingAction(const G4Step *);

private:
  G4int fVerbose;
};
