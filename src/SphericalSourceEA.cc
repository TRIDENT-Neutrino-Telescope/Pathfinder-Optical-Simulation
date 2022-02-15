#include "SphericalSourceEA.hh"
#include "Control.hh"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"

SphericalSourceEA::SphericalSourceEA()
{
  // G4cout << "++++ SphericalSourceEA Constructor ++++" << G4endl;
}

SphericalSourceEA::~SphericalSourceEA()
{
  // G4cout << "++++ SphericalSourceEA Destructor ++++" << G4endl;
}

void SphericalSourceEA::BeginOfEventAction(const G4Event *evt)
{
  fEventID = evt->GetEventID();
  G4cout << "++++ Begin of Event "<< fEventID <<" ++++" << G4endl;

  // generate histograms
  G4AnalysisManager *analysisMan = G4AnalysisManager::Instance();
  if (analysisMan->IsActive())
  {
    G4String directory = Control::Instance()->pathDir;
    G4String filename = Control::Instance()->fileName;
    G4cout << "ROOT FILE: " << directory + "/" + filename << G4endl;
    analysisMan->OpenFile(directory + "/" + filename);
  }
}

void SphericalSourceEA::EndOfEventAction(const G4Event *evt)
{
  G4cout << "++++ End of Event "<< evt->GetEventID() <<" ++++" << G4endl;

  // save histograms
  G4AnalysisManager *analysisMan = G4AnalysisManager::Instance();
  if (analysisMan->IsActive())
  {
    G4cout << "write and close analysis manager..." << G4endl;
    analysisMan->Write();
    analysisMan->CloseFile();
  }
}
