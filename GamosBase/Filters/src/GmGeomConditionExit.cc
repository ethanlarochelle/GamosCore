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
#include "GmGeomConditionExit.hh"
#include "G4VTouchable.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionExit::GetTouchableFromTrack(const G4Track* trk)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExit::GetTouchableFromTrack" << G4endl;
#endif

  // it cannot be exiting any volume if it is PreUserTrackingAction or PostUserTrackingAction, unless it is exiting the world
  if( trk->GetTrackStatus() == fStopAndKill ) {
    if( trk->GetStep() != 0 ) {
      const G4StepPoint* preSP = trk->GetStep()->GetPreStepPoint();
      const G4StepPoint* postSP = trk->GetStep()->GetPostStepPoint();
      if( postSP->GetStepStatus() == fWorldBoundary ){ 
#ifndef GAMOS_NO_VERBOSE
	if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExit::GetTouchableFromTrack return touchable " << preSP->GetTouchable()->GetVolume()->GetName() << G4endl;
#endif
	return GetTouchableFromStepPoint( preSP );
      }
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExit::GetTouchableFromTrack return 0 " << G4endl;
#endif

  return 0;
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionExit::GetTouchableFromStep(const G4Step* step)
{

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExit::GetTouchableFromStep " << G4endl;
#endif

  const G4StepPoint* preSP = step->GetPreStepPoint();
  const G4StepPoint* postSP = step->GetPostStepPoint();
  if( postSP->GetStepStatus() != fGeomBoundary ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExit::GetTouchableFromStep return 0, status " << preSP->GetStepStatus() << G4endl;
#endif
    return 0;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExit::GetTouchableFromStep GeomBoundary " << G4endl;
#endif
    if( preSP->GetTouchable() && preSP->GetTouchable()->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << "Volume = " << preSP->GetTouchable()->GetVolume()->GetName() << G4endl; 
#endif
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << " volume 0 " << G4endl;
#endif
    }
    return GetTouchableFromStepPoint( preSP );
  }

  return 0;

}
