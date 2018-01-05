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
#include "G4TouchableHistory.hh"
#include "GmGeomConditionInParallel.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

//-------------------------------------------------------------------------
GmGeomConditionInParallel::GmGeomConditionInParallel()
{
 theTouchable = new G4TouchableHistory();
  
  std::vector<G4VProcess*> parallelWorldProcesses = GmGetParticleMgr::GetInstance()->GetG4ProcessList("parallelWorldProcess_*");
  if( parallelWorldProcesses.size() == 0 ) {
    G4Exception("GmGeomConditionInParallel::GmGeomConditionInParallel",
		"No G4ParallelWorldScoringProcess named 'parallelWorldProcess_*' found",
		FatalErrorInArgument,
		"Check that you have not defined the filter before '/run/initialize'");
  }

  theParallelWorldProcess = (G4ParallelWorldScoringProcess*)(parallelWorldProcesses[0]);

}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionInParallel::GetTouchableFromTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionInParallel::GetTouchableFromTrack " << G4endl;
#endif
  // check not exiting world
  if( aTrack->GetVolume() == 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionInParallel::GetTouchableFromTrack return 0 " << G4endl;
#endif
    return 0;
  }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionInParallel::GetTouchableFromTrack return touchable " << aTrack->GetTouchable()->GetVolume()->GetName() << " " << aTrack->GetVolume()->GetName() << G4endl;
#endif
  return aTrack->GetTouchable();

}

#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionInParallel::GetTouchableFromStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionInParallel::GetTouchableFromStep " << G4endl;
#endif

  const G4StepPoint* preSP = theParallelWorldProcess->fGhostPreStepPoint;
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "GmGeomConditionInParallel::GetTouchableFromStep return touchable ";
#endif
  if( preSP->GetTouchable() && preSP->GetTouchable()->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout 
	   << preSP->GetTouchable()->GetVolume()->GetName() << " " << aStep->GetTrack()->GetVolume()->GetName() << G4endl;
#endif
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << G4endl;
#endif
  }

  //  G4cout << "GmGeomConditionInParallel::GetTouchableFromStep parallel volume= " << preSP->GetTouchable()->GetVolume()->GetName() << " == " << preSP->GetPhysicalVolume()->GetName() << " mass volume= " << aStep->GetTrack()->GetVolume()->GetName() << G4endl;
  return GetTouchableFromStepPoint( preSP );
}
