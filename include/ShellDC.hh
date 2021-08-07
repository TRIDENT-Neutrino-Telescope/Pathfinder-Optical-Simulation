#pragma once

#include "G4VUserDetectorConstruction.hh"

class G4Sphere;
class G4Material;

#ifdef GDML
#include "G4GDMLParser.hh"
#endif
class ShellDC : public G4VUserDetectorConstruction
{
private:
  G4Sphere *solidWater;
  G4Material *matWater;
  G4LogicalVolume *logicWater;

  G4Sphere *solidShell;
  G4Material *matShell;
  G4LogicalVolume *logicShell;

  G4VPhysicalVolume *world;
  std::string fFileOp;
#ifdef GDML
  G4GDMLParser parser;
#endif

public:
  ShellDC(std::string fileOp);
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

#ifdef GDML
  void WriteGeometryGDML();
#endif
};