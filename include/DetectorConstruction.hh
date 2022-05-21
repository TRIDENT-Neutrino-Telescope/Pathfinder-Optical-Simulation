#pragma once

#include "G4VUserDetectorConstruction.hh"

class G4Sphere;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction {
private:
  G4Material *matWater;
  G4Sphere *solidWater;
  G4LogicalVolume *logicWater;
  G4VPhysicalVolume *world;

  G4Material *matGlass;
  G4Sphere *receiverShell;
  G4LogicalVolume *logicReceiverShell;

  std::string fFileOp;

public:
  DetectorConstruction();
  virtual ~DetectorConstruction();

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