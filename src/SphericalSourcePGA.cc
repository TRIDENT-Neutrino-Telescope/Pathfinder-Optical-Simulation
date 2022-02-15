#include "SphericalSourcePGA.hh"
#include "Control.hh"

#include "G4GeneralParticleSource.hh"
#include "G4PrimaryVertex.hh"
#include "G4OpticalPhoton.hh"
#include "G4Electron.hh"
#include "G4Event.hh"
#include "Randomize.hh"
#include "G4Timer.hh"

SphericalSourcePGA::SphericalSourcePGA()
    : G4VUserPrimaryGeneratorAction(), 
      fNbOfVertex(10000),
      fNbOfPrimary(1000)
{
  G4cout << "size of PrimaryVertex: " << sizeof(G4PrimaryVertex) << G4endl;
  G4cout << "size of PrimaryParticle: " << sizeof(G4PrimaryParticle) << G4endl;
}

SphericalSourcePGA::~SphericalSourcePGA()
{
  delete fTimer;
  delete[] fVecPrimaryVertex;
}

void SphericalSourcePGA::GeneratePrimaries(G4Event *event)
{
  fTimer = new G4Timer;
  fTimer->Start();

  fVecPrimaryVertex = new G4PrimaryVertex *[fNbOfVertex];
  G4double pos_x, pos_y, pos_z, t0, energy;   // emission position, time and energy of photon
  G4double costh, sinth, phi, sinphi, cosphi; // angles
  G4double px_photon, py_photon, pz_photon;   // the momentum of photon
  G4double cos_photon_source;                 // the cosin angle between the photon emission direction and sphere normal vector
  G4double probability;                       // the propability that this generated photon will be accepted in rejection sampling
  const G4ParticleDefinition *photonDef = G4OpticalPhoton::Definition();

  for (int i = 0; i < fNbOfVertex; i++)
  {
    fVecPrimaryVertex[i] = new G4PrimaryVertex;
    // sampling the position
    costh = G4RandFlat::shoot(-1., 1.);
    sinth = sqrt(1. - costh * costh);
    phi = G4RandFlat::shoot(0., 2. * M_PI);
    sincos(phi, &sinphi, &cosphi);
    auto &radius = Control::Instance()->radiusSource;
    pos_x = radius * sinth * cosphi;
    pos_y = radius * sinth * sinphi;
    pos_z = radius * costh;
    fVecPrimaryVertex[i]->SetPosition(pos_x, pos_y, pos_z);
    fVecPrimaryVertex[i]->SetT0(0. * ns);

    // generate primaires
    for (int j = 0; j < fNbOfPrimary; j++)
    {
      /* do rejection sampling
      Since the photon is more likely to be emitted perpendicular to the sphere surface
      */
      energy = G4RandGauss::shoot(2.75 * eV, 0.1 * eV);
      do
      {
        phi = G4RandFlat::shoot(0., 2. * M_PI);
        sincos(phi, &sinphi, &cosphi);
        costh = G4RandFlat::shoot(-1., 1.);
        sinth = sqrt(1. - costh * costh);
        px_photon = energy * sinth * cosphi;
        py_photon = energy * sinth * sinphi;
        pz_photon = energy * costh;
        cos_photon_source = (pos_x * px_photon + pos_y * py_photon + pos_z * pz_photon) /
                            radius / energy;
        probability = (cos_photon_source > 0.0) ? cos_photon_source : 0.0;
      } while (G4RandFlat::shoot(0.0, 1.) > probability);

      G4PrimaryParticle *particle = new G4PrimaryParticle(photonDef, px_photon, py_photon, pz_photon);
      { // polorization of photon
        phi = G4RandFlat::shoot(0., 2. * M_PI);
        sincos(phi, &sinphi, &cosphi);
        G4double polx = py_photon * cosphi + px_photon * pz_photon * sinphi;
        G4double poly = -px_photon * cosphi + py_photon * pz_photon * sinphi;
        G4double polz = -(px_photon * px_photon + py_photon * py_photon) * sinphi;
        G4ThreeVector pol = G4ThreeVector(polx, poly, polz);
        particle->SetPolarization(pol / pol.mag());
      }
      fVecPrimaryVertex[i]->SetPrimary(particle);
    }
  }
  for (int i = 0; i < fNbOfVertex; i++)
    event->AddPrimaryVertex(fVecPrimaryVertex[i]);
  fTimer->Stop();
  G4int memoryUsed = fNbOfVertex * fNbOfPrimary * sizeof(G4PrimaryParticle) / 1024 / 1024;
  if (event->GetEventID() == 0)
  {
    G4cout << "*********** Generate Primaries ***********"
           << "\n";
    G4cout << "event id: " << event->GetEventID() << "\n";
    G4cout << "time used to generate primaries: " << *fTimer << "\n";
    G4cout << "number of vertex: " << fNbOfVertex << "\n";
    G4cout << "number of primary per vertex: " << fNbOfPrimary << "\n";
    G4cout << "memory used for primary particles: " << memoryUsed << " MB"
           << "\n";
    G4cout << "*********** Generate Primaries ***********"
           << "\n";
    G4cout << G4endl;
  }
}