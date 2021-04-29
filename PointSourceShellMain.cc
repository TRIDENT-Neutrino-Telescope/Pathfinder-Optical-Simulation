/* optical property simulation
A program used to simulate the absorption and scattering effect of medium.

1. A number of photons are emitted from the center spherical source.
2. After scattering and absorption, some photons can arrive to the outter shell and recored as photon hits.
*/

#include "G4Types.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4String.hh"
#include "G4VisExecutive.hh"
#include "G4SystemOfUnits.hh"

#include "ShellDC.hh"
#include "PhysicsList.hh"

#include "SphericalSourceSA.hh"
#include "SphericalSourceEA.hh"
#include "SphericalSourceRA.hh"
#include "SphericalSourcePGA.hh"
#include "DirectoryHelper.hh"

int main(int, const char** argv)
{
  const char* fileOpTemp = argv[1];
  std::string fileOp(fileOpTemp);

  DirectoryHelper::Initialize();
  G4RunManager *runManager = new G4RunManager;

  ShellDC *shellDetector = new ShellDC(fileOp);
  runManager->SetUserInitialization(shellDetector);

  runManager->SetUserInitialization(new PhysicsList());

  runManager->SetUserAction(new SphericalSourcePGA());
  runManager->SetUserAction(new SphericalSourceEA());
  runManager->SetUserAction(new SphericalSourceRA());
  runManager->SetUserAction(new SphericalSourceSA());

  //initialize visualization
  G4VisManager *visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager::GetUIpointer()->ApplyCommand("/tracking/verbose 0");
  G4UImanager::GetUIpointer()->ApplyCommand("/run/verbose      1");
  G4UImanager::GetUIpointer()->ApplyCommand("/control/verbose  1");
  G4UImanager::GetUIpointer()->ApplyCommand("/analysis/verbose 1");
  G4UImanager::GetUIpointer()->ApplyCommand("/event/verbose    0");
  G4UImanager::GetUIpointer()->ApplyCommand("/process/verbose  0");
  runManager->Initialize();
  runManager->BeamOn(1);

  // job termination
  delete visManager;
  delete runManager;
  return 0;
}
