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
#include "GmGeomConditionExitParallel.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

//-------------------------------------------------------------------------
GmGeomConditionExitParallel::GmGeomConditionExitParallel()
{
  theTouchable = new G4TouchableHistory();
  
  std::vector<G4VProcess*> parallelWorldProcesses = GmGetParticleMgr::GetInstance()->GetG4ProcessList("parallelWorldProcess_*");
  if( parallelWorldProcesses.size() == 0 ) {
    G4Exception("GmGeomConditionExitParallel::GmGeomConditionExitParallel",
		"No G4ParallelWorldScoringProcess named 'parallelWorldProcess_*' found",
		FatalErrorInArgument,
		"Check that you have not defined the filter before '/run/initialize'");
  }

  theParallelWorldProcess = (G4ParallelWorldScoringProcess*)(parallelWorldProcesses[0]);

}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionExitParallel::GetTouchableFromTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExitParallel::GetTouchableFromTrack " << G4endl;
#endif

  // it cannot be exiting any volume if it is PreUserTrackingAction or PostUserTrackingAction, unless it is exiting the world
  if( aTrack->GetTrackStatus() == fStopAndKill ) {
    if( aTrack->GetStep() != 0 ) {
      const G4StepPoint* preSP = theParallelWorldProcess->fGhostPreStepPoint;
      const G4StepPoint* postSP = theParallelWorldProcess->fGhostPostStepPoint;
      if( postSP->GetStepStatus() == fWorldBoundary ){ 
#ifndef GAMOS_NO_VERBOSE
	if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExitParallel::GetTouchableFromTrack return touchable " << preSP->GetPhysicalVolume()->GetName() << G4endl;
#endif
	return GetTouchableFromStepPoint( preSP );
      }
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExitParallel::GetTouchableFromTrack return 0 " << G4endl;
#endif

  return 0;

}

#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionExitParallel::GetTouchableFromStep(const G4Step* )
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExitParallel::GetTouchableFromStep return touchable " << G4endl;
#endif

  const G4StepPoint* preSP = theParallelWorldProcess->fGhostPreStepPoint;
  const G4StepPoint* postSP = theParallelWorldProcess->fGhostPostStepPoint;

  if( postSP->GetStepStatus() != fGeomBoundary ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExitParallel::GetTouchableFromStep return 0, status " << preSP->GetStepStatus() << G4endl;
#endif
    return 0;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExitParallel::GetTouchableFromStep return touchable " << G4endl;
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

  //  G4cout << "GmGeomConditionExitParallel::GetTouchableFromStep parallel volume= " << preSP->GetTouchable()->GetVolume()->GetName() << " == " << preSP->GetPhysicalVolume()->GetName() << " mass volume= " << aStep->GetTrack()->GetVolume()->GetName() << G4endl;
}
