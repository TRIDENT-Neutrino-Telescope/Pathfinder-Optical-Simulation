#pragma once

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

class G4Material;
class G4HCofThisEvent;
class G4Step;

class ShellSD : public G4VSensitiveDetector
{
public:
  ShellSD(G4String);
  ~ShellSD();
  virtual G4bool ProcessHits(G4Step *astep, G4TouchableHistory *ROHist);

private:
  void DumpInfo(G4Step *aStep);
  G4double GetEfficiency(G4double energy);
  G4double GetTTS();
};