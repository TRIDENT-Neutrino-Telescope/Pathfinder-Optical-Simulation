#pragma once

#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4VUserTrackInformation.hh"
#include "globals.hh"

/*
This class is used to store the information of the parents
of Cherenkov photons. It is assigned in the SteppingAction.
*/
class TrackInformation : public G4VUserTrackInformation {
public:
  TrackInformation();
  virtual ~TrackInformation();

  inline void *operator new(size_t);
  inline void operator delete(void *aTrackInfo);

  TrackInformation &operator=(const TrackInformation &right);
  void SetSourceTrackInformation(const G4Track *aTrack);
  virtual void Print() const;

public:
  inline void SetPosition(G4ThreeVector pos) { fPosition = pos; }
  inline G4ThreeVector GetPosition() { return fPosition; }
  inline void SetDirection(G4ThreeVector dir) { fDirection = dir; }
  inline G4ThreeVector GetDirection() { return fDirection; }
  inline void AddNumBoundary() { fNumBoundary++; }
  inline G4int GetNumBoundary() { return fNumBoundary; }

private:
  G4ThreeVector fPosition;
  G4ThreeVector fDirection;
  G4int fNumBoundary = 0;
};

extern G4ThreadLocal G4Allocator<TrackInformation> *aTrackInformationAllocator;

inline void *TrackInformation::operator new(size_t) {
  if (!aTrackInformationAllocator)
    aTrackInformationAllocator = new G4Allocator<TrackInformation>;
  return (void *)aTrackInformationAllocator->MallocSingle();
}

inline void TrackInformation::operator delete(void *aTrackInfo) {
  aTrackInformationAllocator->FreeSingle((TrackInformation *)aTrackInfo);
}
