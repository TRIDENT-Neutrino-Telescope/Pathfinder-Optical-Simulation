#include "TrackInformation.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal G4Allocator<TrackInformation> *
    aTrackInformationAllocator = 0;

TrackInformation::TrackInformation()
    : G4VUserTrackInformation(),
    fLen(0)
{
}

TrackInformation::~TrackInformation()
{
}

void TrackInformation::Print() const
{
  G4cout << "**** Parient particle information ****" << G4endl;
  G4cout << "**** Position: " << fPosition.x() / m << ", "
         << fPosition.y() / m << ", " << fPosition.z() / m << G4endl;
  G4cout << G4endl;
}