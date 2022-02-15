#pragma once

#include "G4VUserDetectorConstruction.hh"

class G4Sphere;
class G4Material;

class ShellDC : public G4VUserDetectorConstruction {
private:
  G4Sphere *solidWater;
  G4Material *matWater;
  G4LogicalVolume *logicWater;

  G4Sphere *solidShell;
  G4Material *matShell;
  G4LogicalVolume *logicShell;

  G4VPhysicalVolume *world;
  std::string fFileOp;

public:
  ShellDC();
  virtual ~ShellDC();

  virtual G4VPhysicalVolume *Construct();
  void BuildWaterSolid();
  void BuildWaterMaterial();
  void BuildWaterLogic();

  void BuildShellSolid();
  void BuildShellMaterial();
  void BuildShellLogic();
  void BuildShellSensitiveField();

  void BuildSouce();
};