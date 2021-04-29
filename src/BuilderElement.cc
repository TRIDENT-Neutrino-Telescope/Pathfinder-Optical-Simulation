#include "BuilderElement.hh"
#include "G4Element.hh"
#include "G4SystemOfUnits.hh"

BuilderElement::BuilderElement()
{
}

void BuilderElement::Build()
{
  // define element
  G4double a; // Zeff
  a = 1.01 * g / mole;
  elH = new G4Element("Hydrogen", "H", 1., a);
  a = 12.01 * g / mole;
  elC = new G4Element("Carbon", "C", 6., a);
  a = 16.00 * g / mole;
  elO = new G4Element("Oxygen", "O", 8., a);
  a = 28.00 * g / mole;
  elSi = new G4Element("Silicon", "Si", 14., a);
}