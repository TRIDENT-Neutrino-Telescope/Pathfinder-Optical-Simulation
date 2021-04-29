#pragma once

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"

class G4PrimaryVertex;
class G4Timer;

class SphericalSourcePGA : public G4VUserPrimaryGeneratorAction
{
public:
  SphericalSourcePGA();
  virtual ~SphericalSourcePGA();

  virtual void GeneratePrimaries(G4Event *);

private:
  G4double fTimeSpread;
  G4int fNbOfVertex;
  G4int fNbOfPrimary;
  G4PrimaryVertex **fVecPrimaryVertex;
  G4PrimaryVertex *fPrimaryVertex;
  G4Timer *fTimer;
};