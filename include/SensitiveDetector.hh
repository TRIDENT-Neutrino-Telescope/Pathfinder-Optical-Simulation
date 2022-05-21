#pragma once

#include "G4VSensitiveDetector.hh"
#include "g4root.hh"
#include "globals.hh"

class G4Material;
class G4HCofThisEvent;
class G4Step;

class SensitiveDetector : public G4VSensitiveDetector {
public:
  SensitiveDetector(G4String);
  ~SensitiveDetector();
  virtual G4bool ProcessHits(G4Step *astep, G4TouchableHistory *ROHist);

private:
  G4AnalysisManager *fAnalysis;
  void DumpInfo(G4Step *aStep);
};