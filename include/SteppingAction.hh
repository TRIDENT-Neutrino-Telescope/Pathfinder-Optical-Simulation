#pragma once

#include "G4UserSteppingAction.hh"
#include "globals.hh"

/* Do nothing in stepping action
 */
class SteppingAction : public G4UserSteppingAction {
public:
  SteppingAction();
  virtual ~SteppingAction();

  virtual void UserSteppingAction(const G4Step *);

private:
  G4int fVerbose;
};
