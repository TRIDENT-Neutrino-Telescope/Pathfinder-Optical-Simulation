#include "DetectorConstruction.hh"
#include "BuilderElement.hh"
#include "Control.hh"
#include "SensitiveDetector.hh"

#include "G4Box.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  // build element
  BuilderElement elements = BuilderElement();
  elements.Build();

  // build water
  G4cout << "building water..." << G4endl;
  BuildWaterSolid();
  BuildWaterMaterial();
  BuildWaterLogic();

  // build glass shell
  G4cout << "building glass shell..." << G4endl;
  BuildShellSolid();
  BuildShellMaterial();
  BuildShellLogic();
  BuildShellSensitiveField();

  // placing water as world
  G4cout << "placing water world..." << G4endl;
  world = new G4PVPlacement(0,               // rotation
                            G4ThreeVector(), // relative position
                            logicWater,      // logical volume
                            "Water_PV",      // name
                            0,               // mother  volume
                            false,           // boolean operation
                            0,               // copy number
                            true);           // overlaps checking

  // place glass shell in water
  G4cout << "placing glass shell..." << G4endl;
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicReceiverShell, "SHELL_PV",
                    logicWater, false, 0, true);
  // BuildSouce();
  return world;
}

void DetectorConstruction::BuildWaterSolid() {
  solidWater = new G4Sphere("Water_SV", 0.,
                            Control::Instance()->radiusDetector + 1.0 * m, 0,
                            2 * M_PI, 0, M_PI);
}

void DetectorConstruction::BuildWaterMaterial() {
  // get element
  G4Element *elH = G4Element::GetElement("Hydrogen");
  G4Element *elO = G4Element::GetElement("Oxygen");

  // define material
  matWater = new G4Material("Water_MT", 1.03 * g / cm3, 2);
  matWater->AddElement(elH, 2);
  matWater->AddElement(elO, 1);

  // build optical property
  G4MaterialPropertiesTable *mpt = new G4MaterialPropertiesTable();
  auto &opticalProperty = Control::Instance()->geoOptical;
  mpt->AddProperty("RINDEX", opticalProperty.energy, opticalProperty.refracIdx,
                   opticalProperty.num);
  mpt->AddProperty("ABSLENGTH", opticalProperty.energy, opticalProperty.absLen,
                   opticalProperty.num);
  mpt->AddProperty("RAYLEIGH", opticalProperty.energy,
                   opticalProperty.scaLenRay, opticalProperty.num);
  mpt->AddProperty("MIEHG", opticalProperty.energy, opticalProperty.scaLenMie,
                   opticalProperty.num);
  mpt->AddConstProperty("MIEHG_FORWARD", opticalProperty.mieForward);
  mpt->AddConstProperty("MIEHG_BACKWARD", opticalProperty.mieBackward);
  mpt->AddConstProperty("MIEHG_FORWARD_RATIO", opticalProperty.mieRatio);
  matWater->SetMaterialPropertiesTable(mpt);
}

void DetectorConstruction::BuildWaterLogic() {
  logicWater = new G4LogicalVolume(solidWater, matWater, "Water_LV");
}

void DetectorConstruction::BuildShellSolid() {
  receiverShell = new G4Sphere("Shell_SV", Control::Instance()->radiusDetector,
                               Control::Instance()->radiusDetector + 0.1 * m, 0,
                               2 * M_PI, 0, M_PI);
}

void DetectorConstruction::BuildShellMaterial() {
  // get element
  G4Element *elH = G4Element::GetElement("Hydrogen");
  G4Element *elC = G4Element::GetElement("Carbon");
  G4Element *elO = G4Element::GetElement("Oxygen");
  G4Element *elSi = G4Element::GetElement("Silicon");

  // glass material
  matGlass = new G4Material("Glass_MT", 1.19 * g / cm3, 4);
  matGlass->AddElement(elC, 5);
  matGlass->AddElement(elH, 8);
  matGlass->AddElement(elO, 2);
  matGlass->AddElement(elSi, 3);

  G4MaterialPropertiesTable *mptGlass = new G4MaterialPropertiesTable();
  const G4int num = 2;
  G4double photonEnergy[num] = {2.06667 * eV, 4.13333 * eV};
  G4double refractiveIndex[num] = {1.50, 1.50};
  G4double absorptionLength[num] = {10. * m, 10. * m};
  mptGlass->AddProperty("RINDEX", photonEnergy, refractiveIndex, num);
  mptGlass->AddProperty("ABSLENGTH", photonEnergy, absorptionLength, num);
  matGlass->SetMaterialPropertiesTable(mptGlass);
}

void DetectorConstruction::BuildShellSensitiveField() {
  SensitiveDetector *sd = new SensitiveDetector("Shell_SD");
  G4SDManager::GetSDMpointer()->AddNewDetector(sd);
  logicReceiverShell->SetSensitiveDetector(sd);
}

void DetectorConstruction::BuildShellLogic() {
  logicReceiverShell = new G4LogicalVolume(receiverShell, matGlass, "Shell_LV");
}

void DetectorConstruction::BuildSouce() {
  /* Source ball
  Add a ball with air in center and a glass shell that represent the 
  emitter ball in the experiment. It is potional and is used to simulate the 
  optical surface effect of air-glass and glass-water.
  */

  G4Material *matAir = new G4Material("Air_MT", 0.001 * g / cm3, 1);
  matAir->AddElement(G4Element::GetElement("Hydrogen"), 2);

  G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();
  const int num = 2;
  G4double photonEnergy[num] = {2.06667 * eV, 4.13333 * eV};
  double refracAir[num] = {1.0, 1.0};
  mptAir->AddProperty("RINDEX", photonEnergy, refracAir, 2);
  matAir->SetMaterialPropertiesTable(mptAir);

  G4Sphere *solidSourceGlass =
      new G4Sphere("SourceGlass_SV", // name
                   0,
                   Control::Instance()->radiusSource +
                       Control::Instance()->thicknessSourceGlass, // radius
                   0, 2 * M_PI,                                   // phi
                   0, M_PI);                                      // theta
  G4LogicalVolume *logicSourceGlass =
      new G4LogicalVolume(solidSourceGlass,  // solid
                          matGlass,          // material
                          "SourceGlass_LV"); // name
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicSourceGlass,
                    "SourceGlass_PV", logicWater, false, 0, true);

  G4Sphere *solidSourceAir =
      new G4Sphere("SourceAir_SV",                                  // name
                   0, Control::Instance()->radiusSource + 0.1 * mm, // radius
                   0, 2 * M_PI,                                     // phi
                   0, M_PI);                                        // theta
  G4LogicalVolume *logicSourceAir = new G4LogicalVolume(solidSourceAir, // solid
                                                        matAir, // material
                                                        "SourceAir_LV"); // name
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicSourceAir, "SourceAir_PV",
                    logicSourceGlass, false, 0, true);
}