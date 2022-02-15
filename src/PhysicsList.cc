#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4IonPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4OpticalPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4StoppingPhysics.hh"

#include "G4EmParameters.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList() {
  SetVerboseLevel(1);

  RegisterPhysics(new G4EmStandardPhysics_option1);

  // optical physics
  G4OpticalPhysics *opticalPhysics = new G4OpticalPhysics();
  RegisterPhysics(opticalPhysics);
}

PhysicsList::~PhysicsList() {}
