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
#include "GmDataInitialPosMaxXY.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialPosMaxXY::GmDataInitialPosMaxXY()
{
  bInitial = true;
  theHMin = -1000.;
  theHMax = 1000.;
}

//----------------------------------------------------------------
GmDataInitialPosMaxXY::~GmDataInitialPosMaxXY()
{
}


//----------------------------------------------------------------
G4double GmDataInitialPosMaxXY::GetValueFromStep( const G4Step* aStep, G4int )
{

  return std::max(fabs(aStep->GetPreStepPoint()->GetPosition().x()),fabs(aStep->GetPreStepPoint()->GetPosition().y()));
}

//----------------------------------------------------------------
G4double GmDataInitialPosMaxXY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return std::max(fabs(aTrack->GetVertexPosition().x()),fabs(aTrack->GetVertexPosition().y()));
}

//----------------------------------------------------------------
G4double GmDataInitialPosMaxXY::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return std::max(fabs(aTrack2->GetPosition().x()),fabs(aTrack2->GetPosition().y()));
}

//----------------------------------------------------------------
G4double GmDataInitialPosMaxXY::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  return std::max(fabs(anEvent->GetPrimaryVertex(0)->GetX0()),fabs(anEvent->GetPrimaryVertex(0)->GetY0()));
}
