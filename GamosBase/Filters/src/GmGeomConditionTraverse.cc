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
#include "GmGeomConditionTraverse.hh"
#include "G4VTouchable.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionTraverse::GetTouchableFromTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionTraverse::GetTouchableFromStep " << G4endl;
#endif
  // check it is in volume
  // check not exiting world
  if( aTrack->GetVolume() == 0 ) {
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionTraverse::GetTouchableFromStep return 0 " << G4endl;
#endif
    return 0;
  }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionTraverse::GetTouchableFromStep return touchable ";
#endif
  if( aTrack->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout  << aTrack->GetVolume()->GetName() << G4endl;
#endif
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " 0 " << G4endl;
#endif
  }

  return aTrack->GetTouchable();
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionTraverse::GetTouchableFromStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionTraverse::GetTouchableFromStep " << G4endl;
#endif
  const G4StepPoint* preSP = aStep->GetPreStepPoint();
  const G4StepPoint* postSP = aStep->GetPostStepPoint();
  if( preSP->GetTouchable() != postSP->GetTouchable() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionTraverse::GetTouchableFromStep return 0  preSP " << preSP->GetTouchable() << " != " << postSP->GetTouchable() << G4endl;
#endif
    return 0;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionTraverse::GetTouchableFromStep return touchable ";
#endif
    if( preSP->GetPhysicalVolume() && preSP->GetPhysicalVolume()->GetName() ) {
#ifndef GAMOS_NO_VERBOSE
      if( BaseVerb(debugVerb) ) G4cout << preSP->GetPhysicalVolume()->GetName() << G4endl;
#endif
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( BaseVerb(debugVerb) ) G4cout << " 0 " << G4endl;
#endif
    }
    return GetTouchableFromStepPoint( preSP );
  }
}
