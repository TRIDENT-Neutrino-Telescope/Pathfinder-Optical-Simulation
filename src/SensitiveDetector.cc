#include "SensitiveDetector.hh"
#include "TrackInformation.hh"

#include "G4Material.hh"
#include "G4OpticalPhoton.hh"
#include "G4ParticleDefinition.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4TouchableHistory.hh"
#include "G4VTouchable.hh"
#include "Randomize.hh"

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name) {
  fAnalysis = G4AnalysisManager::Instance();
}

SensitiveDetector::~SensitiveDetector() {}

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *) {
  G4String namePhysVol =
      step->GetPreStepPoint()->GetPhysicalVolume()->GetName();

  G4ThreeVector photonDir = step->GetPreStepPoint()->GetMomentumDirection();
  G4ThreeVector photonPos = step->GetPreStepPoint()->GetPosition();
  G4double photonTime = step->GetPreStepPoint()->GetGlobalTime();
  auto trackInfo =
      static_cast<TrackInformation *>(step->GetTrack()->GetUserInformation());

  // If the photon has been deflected too much,
  // it will not show in the camera view.
  // Uncomment following to do the selection.
  // float costh = photonDir.dot(photonPos) /
  // (PointSourceShell::RadiusShellOuter / m); if (costh < 0.95)
  //   return false;

  fAnalysis->FillNtupleDColumn(0, 0, trackInfo->GetPosition().x() / m);
  fAnalysis->FillNtupleDColumn(0, 1, trackInfo->GetPosition().y() / m);
  fAnalysis->FillNtupleDColumn(0, 2, trackInfo->GetPosition().z() / m);
  fAnalysis->FillNtupleDColumn(0, 3, trackInfo->GetDirection().x());
  fAnalysis->FillNtupleDColumn(0, 4, trackInfo->GetDirection().y());
  fAnalysis->FillNtupleDColumn(0, 5, trackInfo->GetDirection().z());
  fAnalysis->FillNtupleDColumn(0, 6, photonDir.x());
  fAnalysis->FillNtupleDColumn(0, 7, photonDir.y());
  fAnalysis->FillNtupleDColumn(0, 8, photonDir.z());
  fAnalysis->FillNtupleDColumn(0, 9, photonPos.x() / m);
  fAnalysis->FillNtupleDColumn(0, 10, photonPos.y() / m);
  fAnalysis->FillNtupleDColumn(0, 11, photonPos.z() / m);
  fAnalysis->FillNtupleDColumn(0, 12, photonTime / ns);
  fAnalysis->FillNtupleIColumn(0, 13,
                               step->GetTrack()->GetCurrentStepNumber() -
                                   trackInfo->GetNumBoundary() - 1);
  fAnalysis->AddNtupleRow(0);

  step->GetTrack()->SetTrackStatus(fStopAndKill);
  return true;
}

void SensitiveDetector::DumpInfo([[maybe_unused]] G4Step *step) {
  // code for testing
  G4cout << "*******************************" << G4endl;
  G4cout << "           SHELL HIT           " << G4endl;
  G4cout << "   WRITE YOUR DEBUG INFO HERE  " << G4endl;
  G4cout << "*******************************" << G4endl << G4endl;
}
