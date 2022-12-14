#include "RunAction.hh"
#include "Control.hh"

#include "G4Run.hh"
#include "G4Timer.hh"
#include "G4UnitsTable.hh"

RunAction::RunAction() : G4UserRunAction() {
  fAnalysis = G4AnalysisManager::Instance();
  InitializeAnalysis();
}

RunAction::~RunAction() { delete fAnalysis; }

void RunAction::BeginOfRunAction([[maybe_unused]] const G4Run *run) {
  // generate histograms
  if (fAnalysis->IsActive()) {
    G4String directory = Control::Instance()->pathDir;
    G4String filename = Control::Instance()->fileName;
    G4cout << "ROOT FILE: " << directory + "/" + filename << G4endl;
    fAnalysis->OpenFile(directory + "/" + filename);
  }
}

void RunAction::EndOfRunAction([[maybe_unused]] const G4Run *run) {
  // save histograms
  if (fAnalysis->IsActive()) {
    G4cout << "write and close analysis manager..." << G4endl;
    fAnalysis->Write();
    fAnalysis->CloseFile();
  }
}

void RunAction::InitializeAnalysis() {
  G4cout << "++++ Initialize Analysis " << G4endl;

  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  fAnalysis->SetFileName("PointSourceShell");
  fAnalysis->SetVerboseLevel(1);
  fAnalysis->SetActivation(true); // enable inactivation of histograms

  //! If remove the dummy histogram, Geant4 can not find the following ntuple
  fAnalysis->CreateH1("dummy", "dummy", 100, 0., 100.);
  fAnalysis->SetH1Activation(0, true);

  // set up Ntuples
  fAnalysis->SetNtupleMerging(false);
  fAnalysis->SetFirstNtupleId(0);
  fAnalysis->SetFirstNtupleColumnId(0);

  // Creating ntuple for hit on Shell
  fAnalysis->CreateNtuple("ShellHit", "ShellHit");

  // the source position
  fAnalysis->CreateNtupleDColumn(0, "sx");
  fAnalysis->CreateNtupleDColumn(0, "sy");
  fAnalysis->CreateNtupleDColumn(0, "sz");

  // the source direction
  fAnalysis->CreateNtupleDColumn(0, "s_nx");
  fAnalysis->CreateNtupleDColumn(0, "s_ny");
  fAnalysis->CreateNtupleDColumn(0, "s_nz");

  // photon direction vector
  fAnalysis->CreateNtupleDColumn(0, "nx");
  fAnalysis->CreateNtupleDColumn(0, "ny");
  fAnalysis->CreateNtupleDColumn(0, "nz");

  // photon hit position vector
  fAnalysis->CreateNtupleDColumn(0, "x0");
  fAnalysis->CreateNtupleDColumn(0, "y0");
  fAnalysis->CreateNtupleDColumn(0, "z0");

  // photon hit time
  fAnalysis->CreateNtupleDColumn(0, "t0");

  // number of scattering times
  fAnalysis->CreateNtupleIColumn(0, "scatter");
  fAnalysis->FinishNtuple();
  fAnalysis->SetNtupleActivation(0, true);
}