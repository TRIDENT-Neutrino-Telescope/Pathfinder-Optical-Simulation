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
#include "Control.hh"
#include "SphericalSourceSA.hh"
#include "SphericalSourceEA.hh"
#include "SphericalSourceRA.hh"
#include "SphericalSourcePGA.hh"

void PrintUsage()
{
  G4cout << "Usage: \n"
         << "  Telescope CONFIG\n"
         << "  For example: ./Telescope config.yaml" << G4endl;
}

int main(int argc, char **argv)
{
  G4String fileNameConfig; // config files
  if (argc == 1 or (argc == 2 && strcmp(argv[1], "--help") == 0))
  {
    PrintUsage();
    return 0;
  }
  else if (argc == 2)
  {
    fileNameConfig = argv[1];
    G4cout << "Setting " << fileNameConfig << " as config file.";
  }
  else
  {
    G4cout << "Wrong input parameters!" << G4endl;
    return 1;
  }

  // Initialize Control
  auto yaml_valid = Control::Instance()->readYAML(fileNameConfig);
  if (!yaml_valid)
  {
    G4cerr << "[Read YAML] ==> Reading Error from YAML file: " << G4endl;
    return -1;
  }

  G4RunManager *runManager = new G4RunManager;

  runManager->SetUserInitialization(new ShellDC());
  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserAction(new SphericalSourcePGA());
  runManager->SetUserAction(new SphericalSourceEA());
  runManager->SetUserAction(new SphericalSourceRA());
  runManager->SetUserAction(new SphericalSourceSA());

  G4UImanager::GetUIpointer()->ApplyCommand("/tracking/verbose 0");
  G4UImanager::GetUIpointer()->ApplyCommand("/run/verbose      1");
  G4UImanager::GetUIpointer()->ApplyCommand("/control/verbose  1");
  G4UImanager::GetUIpointer()->ApplyCommand("/analysis/verbose 1");
  G4UImanager::GetUIpointer()->ApplyCommand("/event/verbose    0");
  G4UImanager::GetUIpointer()->ApplyCommand("/process/verbose  0");
  runManager->Initialize();
  runManager->BeamOn(1);

  // job termination
  delete runManager;
  return 0;
}
