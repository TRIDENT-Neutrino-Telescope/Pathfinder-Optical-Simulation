#pragma once

#include "G4UserRunAction.hh"
#include "g4root.hh"
#include "globals.hh"

class G4Timer;

class SphericalSourceRA : public G4UserRunAction {
public:
  SphericalSourceRA();
  virtual ~SphericalSourceRA();

  virtual void BeginOfRunAction(const G4Run *);
  virtual void EndOfRunAction(const G4Run *);

  // Create or get analysis manager
  void InitializeAnalysis();

private:
  G4AnalysisManager *fAnalysis;
};
