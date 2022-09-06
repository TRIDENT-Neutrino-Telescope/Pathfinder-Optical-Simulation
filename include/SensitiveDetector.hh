#pragma once

#include "globals.hh"
#include "G4VSensitiveDetector.hh"
#include "G4Version.hh"
#if G4VERSION_NUMBER >= 1000
#include "G4AnalysisManager.hh"
#else
#include "g4root.hh"
#endif

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