#include "GmDataFinalVelocity.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalVelocity::GmDataFinalVelocity()
{
  bInitial = false;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalVelocity::~GmDataFinalVelocity()
{
}


//----------------------------------------------------------------
G4double GmDataFinalVelocity::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetVelocity()/299.792;
  // c_light = 299.792 mm/ns
}

//----------------------------------------------------------------
G4double GmDataFinalVelocity::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVelocity()/299.792;
  // c_light = 299.792 mm/ns
}
