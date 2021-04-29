#pragma once

#include "G4Material.hh"
#include "G4Element.hh"

class BuilderElement
{
private:
  G4Element *elH, *elC, *elO, *elSi;

public:
  BuilderElement();
  void Build();
};
