#pragma once

#include "G4Element.hh"
#include "G4Material.hh"

class BuilderElement {
private:
  G4Element *elH, *elC, *elO, *elSi;

public:
  BuilderElement();
  void Build();
};
