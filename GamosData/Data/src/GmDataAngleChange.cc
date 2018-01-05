//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  GAMOS software  is  copyright of the Copyright  Holders  of *
// * the GAMOS Collaboration.  It is provided  under  the  terms  and *
// * conditions of the GAMOS Software License,  included in the  file *
// * LICENSE and available at  http://fismed.ciemat.es/GAMOS/license .*
// * These include a list of copyright holders.                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GAMOS collaboration.                       *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the GAMOS Software license.           *
// ********************************************************************
//
#include "GmDataAngleChange.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataAngleChange::GmDataAngleChange()
{
  theHMax = 180.;
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataAngleChange::~GmDataAngleChange()
{
}

//----------------------------------------------------------------
G4double GmDataAngleChange::GetValueFromStep( const G4Step* aStep, G4int )
{
 double cosAng  = aStep->GetPostStepPoint()->GetMomentumDirection()*aStep->GetPreStepPoint()->GetMomentumDirection();
 if( cosAng > 1. ) cosAng = 1.;
 if( cosAng < -1. ) cosAng = -1.;
  return acos(cosAng)/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataAngleChange::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return acos(aTrack->GetMomentumDirection() * aTrack->GetVertexMomentumDirection())/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataAngleChange::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track*, G4int )
{
  if( DataVerb(debugVerb) ) G4cout << " GmDataAngleChange::GetValueFromSecoTrack " 
	 << acos(aTrack1->GetStep()->GetPostStepPoint()->GetMomentumDirection()*aTrack1->GetStep()->GetPreStepPoint()->GetMomentumDirection())/CLHEP::deg
	 << " anglePost " << aTrack1->GetStep()->GetPostStepPoint()->GetMomentumDirection()
	 << " anglePre " << aTrack1->GetStep()->GetPreStepPoint()->GetMomentumDirection() << G4endl;
  return acos(aTrack1->GetStep()->GetPostStepPoint()->GetMomentumDirection()*aTrack1->GetStep()->GetPreStepPoint()->GetMomentumDirection())/CLHEP::deg;
}
