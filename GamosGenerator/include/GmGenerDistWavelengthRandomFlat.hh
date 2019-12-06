////////////////////////////////////////////////////////////////////////
// Optical Photon User-Defined Wavelength Distribution
////////////////////////////////////////////////////////////////////////
//
// File G4GenerDistWavelengthRandomFlat.hh
// Description: Generates optical photons uniformly in a waveband.
// Created: 2013-02-22
// Author: Adam Glaser
// Updated: 2019-12-06 by Ethan LaRochelle, port to 6.1
//
// This subroutine will generate a uniform whitelight spectrum.
//
// mail:  adam.k.glaser@dartmouth.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef GmGenerDistWavelengthRandomFlat_HH
#define GmGenerDistWavelengthRandomFlat_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistEnergy.hh"
class GmParticleSource;

class GmGenerDistWavelengthRandomFlat : public GmVGenerDistEnergy
{
public:
  GmGenerDistWavelengthRandomFlat(){};
  virtual ~GmGenerDistWavelengthRandomFlat(){};

  virtual G4double GenerateEnergy( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4double theEnergyMin, theEnergyMax;
};

#endif
