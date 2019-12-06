////////////////////////////////////////////////////////////////////////
// Optical Photon Wavelength Data Type
////////////////////////////////////////////////////////////////////////
//
// File GmDataWavelengthEnergy.cc
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

#include "GmDataWavelengthEnergy.hh"
#include "G4tgrUtils.hh"
#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataWavelengthEnergy::GmDataWavelengthEnergy()
{
  bInitial = false;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataWavelengthEnergy::~GmDataWavelengthEnergy()
{
}


//----------------------------------------------------------------
G4double GmDataWavelengthEnergy::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4double energy=aStep->GetPostStepPoint()->GetKineticEnergy()+aStep->GetTrack()->GetDefinition()->GetPDGMass(); 

 return 1239.84187/energy;
}

//----------------------------------------------------------------
G4double GmDataWavelengthEnergy::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4double energy=aTrack->GetKineticEnergy()+aTrack->GetDefinition()->GetPDGMass();

 return 1239.84187/energy;
}
