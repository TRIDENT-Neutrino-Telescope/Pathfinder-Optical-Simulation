#pragma once

#include "G4SystemOfUnits.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4PrimaryVertex;
class G4Timer;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  PrimaryGeneratorAction();
  virtual ~PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event *);

private:
  G4Timer *fTimer;
  G4int fNbOfVertex;
  const G4int fNbOfPrimary = 100;
  const G4int fNbOfVertexMax = 1000;
  G4PrimaryVertex **fVecPrimaryVertex;
};