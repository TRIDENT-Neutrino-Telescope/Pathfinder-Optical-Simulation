#include "SphericalSourceSA.hh"
#include "TrackInformation.hh"

#include "G4Step.hh"
#include "G4Track.hh"

SphericalSourceSA::SphericalSourceSA() {}

SphericalSourceSA::~SphericalSourceSA() {}

void SphericalSourceSA::UserSteppingAction([
    [maybe_unused]] const G4Step *step) {
  auto track = step->GetTrack();
  auto trackInfo = static_cast<TrackInformation *>(track->GetUserInformation());
  if (!trackInfo) {
    trackInfo = new TrackInformation();
    track->SetUserInformation(trackInfo);
  }

  if (step->GetPreStepPoint()->GetLocalTime() == 0) {
    trackInfo->SetPosition(step->GetPreStepPoint()->GetPosition());
    trackInfo->SetDirection(step->GetPreStepPoint()->GetMomentumDirection());
  }
}