// katya
#ifndef GmDataInitialVelocity_hh
#define GmDataInitialVelocity_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataInitialVelocity : public GmVData
{
public:
  GmDataInitialVelocity();
  ~GmDataInitialVelocity();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

};
#endif
