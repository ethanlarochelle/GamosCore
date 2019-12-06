#ifndef GmDataMomChangeZ_hh
#define GmDataMomChangeZ_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataMomChangeZ : public GmVData
{
public:
  GmDataMomChangeZ();
  ~GmDataMomChangeZ();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
