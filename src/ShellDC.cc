#include "ShellDC.hh"
#include "ShellSD.hh"
#include "BuilderElement.hh"
#include "GeometryParameters.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "cnpy/cnpy.hpp"

using namespace PointSourceShell;

ShellDC::ShellDC(std::string fileOp)
  : fFileOp(fileOp)
{
}

ShellDC::~ShellDC()
{
}

G4VPhysicalVolume *ShellDC::Construct()
{
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
  BuildShellSurface();
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
  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, 0),
                    logicShell,
                    "SHELL_PV",
                    logicWater,
                    false,
                    0,
                    true);
  return world;
}

void ShellDC::BuildWaterSolid()
{
  solidWater = new G4Sphere("Water_SV",
                            0., RadiusShellOuter + 1.0 * m,
                            0, 2 * M_PI,
                            0, M_PI);
}

void ShellDC::BuildWaterMaterial()
{
  // get element
  G4Element *elH = G4Element::GetElement("Hydrogen");
  G4Element *elO = G4Element::GetElement("Oxygen");

  // define material
  matWater = new G4Material("Water_MT", 1.03 * g / cm3, 2);
  matWater->AddElement(elH, 2);
  matWater->AddElement(elO, 1);

  // set water optical property
  std::string path_data = "config/";
  cnpy::NpyArray array = cnpy::npy_load(path_data + fFileOp);
  G4int numOp = array.shape[1];
  G4double *dataOp = array.data<double>();
  G4double *energy = dataOp;
  for (int i = 0; i < numOp; i++) { energy[i] *= eV; }
  G4double *refracIdx = dataOp + numOp;
  for (int i = 0; i < numOp; i++) { refracIdx[i] *= 1.; }
  G4double *absLen = dataOp + numOp * 2;
  for (int i = 0; i < numOp; i++) { absLen[i] *= m; }
  G4double *rayScaLen = dataOp + numOp * 3;
  for (int i = 0; i < numOp; i++) { rayScaLen[i] *= m; }
  G4double *mieScaLen = dataOp + numOp * 4;
  for (int i = 0; i < numOp; i++) { mieScaLen[i] *= m; }

  G4double mieForward = 0.6;
  G4double mieBackward = 0.6;
  G4double mieRatio = 0.8;

  G4MaterialPropertiesTable *mptWater = new G4MaterialPropertiesTable();
  mptWater->AddProperty("RINDEX", energy, refracIdx, 300)->SetSpline(true);
  mptWater->AddProperty("ABSLENGTH", energy, absLen, 300)->SetSpline(true);
  mptWater->AddProperty("RAYLEIGH", energy, rayScaLen, 300)->SetSpline(true);
  matWater->SetMaterialPropertiesTable(mptWater);
  if (mieForward)
  {
    mptWater->AddProperty("MIEHG", energy, mieScaLen, 300)->SetSpline(true);
    mptWater->AddConstProperty("MIEHG_FORWARD", mieForward);
    mptWater->AddConstProperty("MIEHG_BACKWARD", mieBackward);
    mptWater->AddConstProperty("MIEHG_FORWARD_RATIO", mieRatio);
  }
}

void ShellDC::BuildWaterLogic()
{
  logicWater = new G4LogicalVolume(solidWater,
                                   matWater,
                                   "Water_LV");
}

void ShellDC::BuildShellSolid()
{
  solidShell = new G4Sphere("Shell_SV",
                            RadiusShellOuter, RadiusShellOuter + 0.1 * m,
                            0, 2 * M_PI,
                            0, M_PI);
}

void ShellDC::BuildShellMaterial()
{
  // get element
  G4Element *elH = G4Element::GetElement("Hydrogen");
  G4Element *elC = G4Element::GetElement("Carbon");
  G4Element *elO = G4Element::GetElement("Oxygen");
  G4Element *elSi = G4Element::GetElement("Silicon");

  // glass material
  G4Material *matGlass = new G4Material("Glass_MT", 1.19 * g / cm3, 4);
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
  matShell = matGlass;
}

void ShellDC::BuildShellSurface()
{
  G4OpticalSurface *opSurfaceShell = new G4OpticalSurface("Shell_OS");
  opSurfaceShell->SetType(dielectric_metal);
  const G4int num = 5;
  G4double ephoton[num] = {2.06667 * eV, 2.5 * eV, 3.0 * eV, 3.5 * eV, 4.13333 * eV};
  G4double reflection[num] = {0.0, 0.0, 0.0, 0.0, 0.0};
  G4double efficiency[num] = {1.0, 1.0, 1.0, 1.0, 1.0};
  G4MaterialPropertiesTable *mptSurfaceShell = new G4MaterialPropertiesTable();
  mptSurfaceShell->AddProperty("REFLECTIVITY", ephoton, reflection, num);
  mptSurfaceShell->AddProperty("EFFICIENCY", ephoton, efficiency, num);
  opSurfaceShell->SetMaterialPropertiesTable(mptSurfaceShell);
  new G4LogicalSkinSurface("Shell_SS",
                           logicShell,
                           opSurfaceShell);
}

void ShellDC::BuildShellSensitiveField()
{
  ShellSD *shellSD = new ShellSD("SHELL_SD");
  G4SDManager::GetSDMpointer()->AddNewDetector(shellSD);
  logicShell->SetSensitiveDetector(shellSD);
}

void ShellDC::BuildShellLogic()
{
  logicShell = new G4LogicalVolume(solidShell,
                                   matShell,
                                   "Shell_LV");
}

void ShellDC::BuildSouce()
{
  /* Source Sphere
  The source sphere is a reflecting sphere
  It is used to reflect photons from the actual photon sources
  */
  G4Sphere *solidSource = new G4Sphere("Source_SV",                // name
                                       0, RadiusSource - 0.1 * mm, // radius
                                       0, 2 * M_PI,                // phi
                                       0, M_PI);                   // theta
  G4Material *matSource = G4Material::GetMaterial("Glass_MT", true);
  G4LogicalVolume *logicSource = new G4LogicalVolume(solidSource,  // solid
                                                     matSource,    // material
                                                     "Source_LV"); // name
  {
    G4OpticalSurface *SourceOpSurface = new G4OpticalSurface("Source_OS");
    SourceOpSurface->SetType(dielectric_dielectric);
    SourceOpSurface->SetFinish(polished);
    G4MaterialPropertiesTable *SourceMpt = new G4MaterialPropertiesTable();
    const G4int num = 5;
    G4double ephoton[num] = {2.06667 * eV, 2.5 * eV, 3.0 * eV, 3.5 * eV, 4.13333 * eV};
    G4double reflection[num] = {1., 1., 1., 1., 1.};
    SourceMpt->AddProperty("REFLECTIVITY", ephoton, reflection, num);
    SourceOpSurface->SetMaterialPropertiesTable(SourceMpt);
    new G4LogicalSkinSurface("Source_LSS",
                             logicSource,
                             SourceOpSurface);
  }
  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, 0),
                    logicSource,
                    "SHELL_PV",
                    logicWater,
                    false,
                    0,
                    true);
}