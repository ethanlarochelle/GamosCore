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
#include "GmGeomConditionInMass.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "G4VTouchable.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

//-------------------------------------------------------------------------
GmGeomConditionInMass::GmGeomConditionInMass()
{
  theTouchable = new G4TouchableHistory();
  theNavigator = 0;
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionInMass::GetTouchableFromTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionInMass::GetTouchableFromTrack return touchable " << G4endl;
#endif
//  return aTrack->GetTouchable();

  if( !theNavigator ) {
    theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  }
  theNavigator->LocateGlobalPointAndUpdateTouchable( aTrack->GetPosition(), theTouchable, false ); 

  return theTouchable;
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionInMass::GetTouchableFromStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionInMass::GetTouchableFromTrack return touchable " << G4endl;
#endif

  if( !theNavigator ) {
    theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  }
  theNavigator->LocateGlobalPointAndUpdateTouchable( aStep->GetPreStepPoint()->GetPosition(), theTouchable, false ); 
  /*  const G4StepPoint* preSP = aStep->GetPreStepPoint();
  const G4VTouchable* touch = preSP->GetTouchable();
  G4cout << " GmGeomConditionI touch " << touch << " new " << aStep->GetTrack()->GetTouchable() << G4endl; // different if parallel geom  */
  return theTouchable;

}
