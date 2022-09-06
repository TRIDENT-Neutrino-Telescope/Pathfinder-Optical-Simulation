#pragma once

#include "globals.hh"
#include "G4UserRunAction.hh"

#include "G4Version.hh"
#if G4VERSION_NUMBER >= 1000
#include "G4AnalysisManager.hh"
#else
#include "g4root.hh"
#endif

class G4Timer;

class RunAction : public G4UserRunAction {
public:
  RunAction();
  virtual ~RunAction();

  virtual void BeginOfRunAction(const G4Run *);
  virtual void EndOfRunAction(const G4Run *);

  // Create or get analysis manager
  void InitializeAnalysis();

private:
  G4AnalysisManager *fAnalysis;
};
