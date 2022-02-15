#include "SphericalSourceRA.hh"

#include "G4Run.hh"
#include "G4Timer.hh"
#include "G4UnitsTable.hh"

SphericalSourceRA::SphericalSourceRA()
    : G4UserRunAction(),
      fTimer(nullptr)
{
  fTimer = new G4Timer;
  fAnalysis = G4AnalysisManager::Instance();
  InitializeAnalysis();
}

SphericalSourceRA::~SphericalSourceRA()
{
  delete fTimer;
  delete fAnalysis;
}

void SphericalSourceRA::BeginOfRunAction(const G4Run *aRun)
{
  G4cout << "++++ Begin of Run " << aRun->GetRunID() << " ++++\n" << G4endl;
  fTimer->Start();
}

void SphericalSourceRA::EndOfRunAction(const G4Run *aRun)
{
  G4cout << "++++ End of Run " << aRun->GetRunID() << " ++++\n" << G4endl;
  fTimer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent()
         << " " << *fTimer << G4endl;
}

void SphericalSourceRA::InitializeAnalysis()
{
  G4cout << "++++ Initialize Analysis " << G4endl;
  
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  fAnalysis->SetFileName("PointSourceShell");
  fAnalysis->SetVerboseLevel(1);
  fAnalysis->SetActivation(true); // enable inactivation of histograms

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