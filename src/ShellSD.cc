#include "ShellSD.hh"
#include "GeometryParameters.hh"

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
  G4String namePhysVol = step->GetPostStepPoint()->GetPhysicalVolume()->GetName();
  const G4ParticleDefinition *particle = step->GetTrack()->GetDefinition();
  // G4cout << "name of physical volume: " << namePhysVol << ", particle: " << particle->GetParticleName() << G4endl;

  if (namePhysVol != "SHELL_PV" || particle != G4OpticalPhoton::Definition())
  {
    // G4cout << "Photon reflected." << G4endl;
    // G4cout << "Particle time: " << step->GetTrack()->GetGlobalTime() << G4endl;
    // G4cout << "name of physical volume: " << namePhysVol << ", particle: " << particle->GetParticleName() << G4endl;
    return false;
  }

#ifdef QE
  if (G4UniformRand() > GetEfficiency(step->GetTrack()->GetKineticEnergy()))
  {
    G4cout << "G4UniformRand() > QE, reject hit." <<G4endl;
    step->GetTrack()->SetTrackStatus(fStopAndKill);
    return false;
  }
#endif

  step->GetTrack()->SetTrackStatus(fStopAndKill);
  G4ThreeVector photonDir = step->GetPostStepPoint()->GetMomentumDirection();
  G4ThreeVector photonPos = step->GetPostStepPoint()->GetPosition() / m;
  G4double photonTime = step->GetPostStepPoint()->GetGlobalTime();
  float costh = photonDir.dot(photonPos) / (PointSourceShell::RadiusShellOuter / m);

  // If the photon has been deflected too much,
  // it will not show in the camera view
  if (costh < 0.95)
    return false;

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(0, 0, photonDir.x());
  analysisManager->FillNtupleDColumn(0, 1, photonDir.y());
  analysisManager->FillNtupleDColumn(0, 2, photonDir.z());
  analysisManager->FillNtupleDColumn(0, 3, photonPos.x());
  analysisManager->FillNtupleDColumn(0, 4, photonPos.y());
  analysisManager->FillNtupleDColumn(0, 5, photonPos.z());
  analysisManager->FillNtupleDColumn(0, 6, photonTime);
  if (step->GetTrack()->GetCurrentStepNumber() > 1)
    analysisManager->FillNtupleIColumn(0, 7, 1);
  else
    analysisManager->FillNtupleIColumn(0, 7, 0);
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

G4double ShellSD::GetEfficiency([[maybe_unused]] G4double energy)
{
  return 0.3;
}

G4double ShellSD::GetTTS()
{
  return (G4RandGauss::shoot(0., 5. * CLHEP::ns));
}
