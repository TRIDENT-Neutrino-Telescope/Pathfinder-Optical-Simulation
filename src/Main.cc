/* optical property simulation
A program used to simulate the absorption and scattering effect of medium.

1. A number of photons are emitted from the center spherical source.
2. After scattering and absorption, some photons can arrive to the outter shell
and recored as photon hits.
*/

#include "G4RunManager.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4Types.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

#include "Control.hh"
#include "PhysicsList.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

void readArgs(int argc, char **argv) {
  auto control = Control::Instance();
  if (argc == 1 or (argc == 2 && strcmp(argv[1], "--help") == 0)) {
    G4cout << "Usage: \n"
          << "   mode 1: OpticalSimulation config.yaml\n"
          << "   mode 2: OpticalSimulation PhotonEnergy[nm] RefracIndex AbsorptionLength[m]\n"
          << "                             MieScatterLength[m] RayleighScatterLength[m]\n"
          << "                             MieForwardMean RadiusDetector[m]\n"
          << "                             OutputDirectory OutputFileName\n";
    exit(0);
  } else if (argc == 2) {
    G4String fileNameConfig; // config files
    fileNameConfig = argv[1];
    G4cout << "Setting " << fileNameConfig << " as config file." << G4endl;
    auto yaml_valid = control->readYAML(fileNameConfig);
    if (!yaml_valid) {
      std::runtime_error("Error when reading YAML file!");
      exit(-1);
    }
  } else if (argc == 10) {
    OpticalProperty &optic = control->geoOptical;
    optic.num = 1;
    optic.energy = new double((1240.3 / atof(argv[1])) * eV);
    optic.refracIdx = new double(atof(argv[2]));
    optic.absLen = new double(atof(argv[3]) * m);
    optic.scaLenRay = new double(atof(argv[4]) * m);
    optic.scaLenMie = new double(atof(argv[5]) * m);
    optic.mieForward = atof(argv[6]);
    control->radiusDetector = atof(argv[7]) * m;
    control->pathDir = argv[8];
    control->fileName = argv[9];
    control->setRandomByTime();
    control->setNumPhoton(1e7);
  } else {
    std::runtime_error("Wrong input parameters!");
    exit(-1);
  }
}

int main(int argc, char **argv) {
  readArgs(argc, argv);

  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  CLHEP::HepRandom::setTheSeed(Control::Instance()->randomSeed);
  G4cout << "Random seed: " << Control::Instance()->randomSeed << G4endl;

  G4RunManager *runManager = new G4RunManager;

  runManager->SetUserInitialization(new DetectorConstruction());
  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserAction(new PrimaryGeneratorAction());
  runManager->SetUserAction(new EventAction());
  runManager->SetUserAction(new RunAction());
  runManager->SetUserAction(new SteppingAction());

  G4UImanager::GetUIpointer()->ApplyCommand("/tracking/verbose 0");
  G4UImanager::GetUIpointer()->ApplyCommand("/run/verbose      1");
  G4UImanager::GetUIpointer()->ApplyCommand("/control/verbose  1");
  G4UImanager::GetUIpointer()->ApplyCommand("/analysis/verbose 1");
  G4UImanager::GetUIpointer()->ApplyCommand("/event/verbose    0");
  G4UImanager::GetUIpointer()->ApplyCommand("/process/verbose  0");
  runManager->Initialize();
  runManager->BeamOn(Control::Instance()->nEvent);

  // job termination
  delete runManager;
  return 0;
}
