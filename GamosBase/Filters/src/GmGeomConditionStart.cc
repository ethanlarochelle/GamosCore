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
#include "GmGeomConditionStart.hh"
#include "G4VTouchable.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4TouchableHistory.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

GmGeomConditionStart::GmGeomConditionStart()
{
  theTouchable = new G4TouchableHistory();
  theNavigator = 0;
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionStart::GetTouchableFromTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromTrack step number " << aTrack->GetCurrentStepNumber() << G4endl;
#endif
  if( !theNavigator ) {
    theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  }
  theNavigator->LocateGlobalPointAndUpdateTouchable( aTrack->GetVertexPosition(), theTouchable, false ); 

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromTrack return touchable " << theTouchable->GetVolume()->GetName() << G4endl;
#endif
  return theTouchable;
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionStart::GetTouchableFromStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromStep step number " << aStep->GetTrack()->GetCurrentStepNumber() << G4endl;
#endif
  if( aStep->GetTrack()->GetCurrentStepNumber() != 1 ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromStep return 0 " << G4endl;
#endif
    return 0;
  } else {
    const G4StepPoint* preSP = aStep->GetPreStepPoint();
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromStep return ";
#endif
    if( preSP->GetTouchable() && preSP->GetTouchable()->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << preSP->GetTouchable()->GetVolume()->GetName() << G4endl; 
#endif
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << " 0 " << G4endl;
#endif
    }
    return GetTouchableFromStepPoint( preSP );
  }
}
