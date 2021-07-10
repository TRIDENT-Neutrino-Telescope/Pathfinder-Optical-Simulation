#include "ShellSD.hh"
#include "GeometryParameters.hh"
#include "TrackInformation.hh"

#include "G4Material.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"
#include "Randomize.hh"

ShellSD::ShellSD(G4String name) : G4VSensitiveDetector(name)
{
}

ShellSD::~ShellSD()
{
}

G4bool ShellSD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  G4String namePhysVol = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();

  G4ThreeVector photonDir = step->GetPreStepPoint()->GetMomentumDirection();
  G4ThreeVector photonPos = step->GetPreStepPoint()->GetPosition() / m;
  G4double photonTime = step->GetPreStepPoint()->GetGlobalTime();
  auto trackInfo = static_cast<TrackInformation *>(step->GetTrack()->GetUserInformation());

  // If the photon has been deflected too much,
  // it will not show in the camera view
  // float costh = photonDir.dot(photonPos) / (PointSourceShell::RadiusShellOuter / m);
  // if (costh < 0.95)
  //   return false;

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(0, 0, trackInfo->GetPosition().x());
  analysisManager->FillNtupleDColumn(0, 1, trackInfo->GetPosition().y());
  analysisManager->FillNtupleDColumn(0, 2, trackInfo->GetPosition().z());
  analysisManager->FillNtupleDColumn(0, 3, photonDir.x());
  analysisManager->FillNtupleDColumn(0, 4, photonDir.y());
  analysisManager->FillNtupleDColumn(0, 5, photonDir.z());
  analysisManager->FillNtupleDColumn(0, 6, photonPos.x() / m);
  analysisManager->FillNtupleDColumn(0, 7, photonPos.y() / m);
  analysisManager->FillNtupleDColumn(0, 8, photonPos.z() / m);
  analysisManager->FillNtupleDColumn(0, 9, photonTime);
  analysisManager->FillNtupleIColumn(0, 10, step->GetTrack()->GetCurrentStepNumber());
  analysisManager->AddNtupleRow(0);

  return true;
}

void ShellSD::DumpInfo([[maybe_unused]] G4Step *step)
{
  // code for understanding the return value of Geant4 function
  G4cout << "*******************************" << G4endl;
  G4cout << "           SHELL HIT           " << G4endl;
  G4cout << "   WRITE YOUR DEBUG INFO HERE  " << G4endl;
  G4cout << "*******************************" << G4endl << G4endl;
}
