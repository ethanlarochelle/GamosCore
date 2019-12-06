///////////////////////////////////////////////////////////////////////
// Optical Photon Wavelength Data Type
////////////////////////////////////////////////////////////////////////
//
// File GmDataWavelengthEnergy.hh
// Description: Wavelength data type.
// Created: 2013-02-22
// Author: Adam Glaser
// Updated: 2019-12-06 by Ethan LaRochelle, port to 6.1
//
// Creates wavelength data type for optical photons.
//
// mail:  adam.k.glaser@dartmouth.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef GmDataWavelengthEnergy_hh
#define GmDataWavelengthEnergy_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataWavelengthEnergy : public GmVData
{
public:
  GmDataWavelengthEnergy();
  ~GmDataWavelengthEnergy();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
