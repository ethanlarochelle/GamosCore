#ifndef GmDataInitialNoVertexDirTheta_hh
#define GmDataInitialNoVertexDirTheta_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataInitialNoVertexDirTheta : public GmVData
{
public:
  GmDataInitialNoVertexDirTheta();
  ~GmDataInitialNoVertexDirTheta();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );
  virtual G4double GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index = 0 );
  virtual G4double GetValueFromEvent( const G4Event* aEvent, G4int index = 0);

};
#endif
