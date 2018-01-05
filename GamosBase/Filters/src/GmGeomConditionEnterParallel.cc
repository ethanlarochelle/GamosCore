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
#include "GmGeomConditionEnterParallel.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

//-------------------------------------------------------------------------
GmGeomConditionEnterParallel::GmGeomConditionEnterParallel()
{
  theTouchable = new G4TouchableHistory();
  
  std::vector<G4VProcess*> parallelWorldProcesses = GmGetParticleMgr::GetInstance()->GetG4ProcessList("parallelWorldProcess_*");
  if( parallelWorldProcesses.size() == 0 ) {
    G4Exception("GmGeomConditionEnterParallel::GmGeomConditionEnterParallel",
		"No G4ParallelWorldScoringProcess named 'parallelWorldProcess_*' found",
		FatalErrorInArgument,
		"Check that you have not defined the filter before '/run/initialize'");
  }

  theParallelWorldProcess = (G4ParallelWorldScoringProcess*)(parallelWorldProcesses[0]);

}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionEnterParallel::GetTouchableFromTrack(const G4Track* )
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionEnterParallel::GetTouchableFromTrack return 0 " << G4endl;
#endif
  return 0;
}

#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionEnterParallel::GetTouchableFromStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionEnterParallel::GetTouchableFromStep " << G4endl;
#endif

  const G4StepPoint* postSP = theParallelWorldProcess->fGhostPostStepPoint;
  if( postSP->GetStepStatus() != fGeomBoundary ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionEnterParallel::GetTouchableFromStep return 0, stepStatus " << postSP->GetStepStatus() << G4endl;
#endif
    return 0;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "GmGeomConditionEnterParallel::GetTouchableFromStep return touchable ";
#endif
    if( postSP->GetTouchable() && postSP->GetTouchable()->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
      if( BaseVerb(debugVerb) ) G4cout 
	     << postSP->GetTouchable()->GetVolume()->GetName() << " " << aStep->GetTrack()->GetVolume()->GetName() << G4endl;
#endif
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( BaseVerb(debugVerb) ) G4cout << " 0 " << G4endl;
#endif
    }
    return GetTouchableFromStepPoint( postSP );
  }

  //  G4cout << "GmGeomConditionEnterParallel::GetTouchableFromStep parallel volume= " << postSP->GetTouchable()->GetVolume()->GetName() << " == " << postSP->GetPhysicalVolume()->GetName() << " mass volume= " << aStep->GetTrack()->GetVolume()->GetName() << G4endl;
}
