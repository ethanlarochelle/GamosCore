////////////////////////////////////////////////////////////////////////
// Optical Photon User-Defined Wavelength Distribution
////////////////////////////////////////////////////////////////////////
//
// File G4GenerDistWavelengthFromFile.hh
// Description: Generates optical photons from a user-defined spectrum.
// Created: 2013-02-22
// Author: Adam Glaser
// Updated: 2019-12-06 by Ethan LaRochelle, port to 6.1
//
// This subroutine will generate a user-defined whitelight spectrum.
//
// mail:  adam.k.glaser@dartmouth.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef GmGenerDistWavelengthFromFile_h
#define GmGenerDistWavelengthFromFile_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4ParticleGun;
class G4Event;
class GmAnalysisMgr;
#include <map>

#include "GamosCore/GamosGenerator/include/GmVGenerDistEnergy.hh"
class GmParticleSource;
enum EFFCalcType2 { EFFCT_Fixed2, EFFCT_Histogram2, EFFCT_Interpolate2, EFFCT_InterpolateLog2 };

class GmGenerDistWavelengthFromFile : public GmVGenerDistEnergy
{
public:
  GmGenerDistWavelengthFromFile();
  virtual ~GmGenerDistWavelengthFromFile(){};

  virtual G4double GenerateEnergy( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4String theFileName;
  
  void ReadWavelengthDist();

  G4double theEnergyUnit;

  std::map<G4double,G4double> theProbEner; // map <added up probability, energy>

  G4double theHBin;

  EFFCalcType2 theCalculationType2;

  G4double theUnit;

};

#endif
