// katya
#include "GmDataInitialVelocity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"

//----------------------------------------------------------------
GmDataInitialVelocity::GmDataInitialVelocity()
{
  bInitial = true;
  theHMax = 1.;
}

//----------------------------------------------------------------
GmDataInitialVelocity::~GmDataInitialVelocity()
{
}


//----------------------------------------------------------------
G4double GmDataInitialVelocity::GetValueFromStep( const G4Step* aStep, G4int )
{
  //  G4cout << " GmDataInitialVelocity::GetValueFromStep " << aStep->GetPreStepPoint()->GetVelocity() << G4endl;
  return aStep->GetPreStepPoint()->GetVelocity()/299.792;
  // c_lihght = 299.792 mm/ns
}


