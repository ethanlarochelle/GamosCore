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
#include "GmDataInitialParallelPVCopyNumber.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4Navigator.hh"
#include "G4VPhysicalVolume.hh"

//----------------------------------------------------------------
GmDataInitialParallelPVCopyNumber::GmDataInitialParallelPVCopyNumber()
{
  bInitial = true;
  theExcludedTypes.clear(); // delete DTRun
  std::vector<G4VProcess*> parallelWorldProcesses = GmGetParticleMgr::GetInstance()->GetG4ProcessList("parallelWorldProcess_*");
  if( parallelWorldProcesses.size() == 0 ) {
    G4Exception("GmGeomConditionStartParallel::GmGeomConditionStartParallel",
		"No G4ParallelWorldScoringProcess named 'parallelWorldProcess_*' found",
		FatalErrorInArgument,
		"Check that you have not defined the filter before '/run/initialize'");
  }

  theParallelWorldProcess = (G4ParallelWorldScoringProcess*)(parallelWorldProcesses[0]);
  
}

//----------------------------------------------------------------
GmDataInitialParallelPVCopyNumber::~GmDataInitialParallelPVCopyNumber()
{
}


//----------------------------------------------------------------
G4double GmDataInitialParallelPVCopyNumber::GetValueFromStep( const G4Step* , G4int )
{
  const G4StepPoint* preSP = theParallelWorldProcess->fGhostPreStepPoint;
  return preSP->GetPhysicalVolume()->GetCopyNo();

}

#include "G4TransportationManager.hh"
//----------------------------------------------------------------
G4double GmDataInitialParallelPVCopyNumber::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4TouchableHistory* touch = new G4TouchableHistory;

  theParallelWorldProcess->fGhostNavigator->LocateGlobalPointAndUpdateTouchable( aTrack->GetVertexPosition(), touch, false ); 
  
  G4int copyNo = touch->GetVolume()->GetCopyNo();
  
  delete touch;

  return copyNo;
  
}

//----------------------------------------------------------------
G4double GmDataInitialParallelPVCopyNumber::GetValueFromSecoTrack(const G4Track* , const G4Track* , G4int )
{
  const G4StepPoint* postSP = theParallelWorldProcess->fGhostPreStepPoint;
  return postSP->GetPhysicalVolume()->GetCopyNo();
  return 0;
}



//----------------------------------------------------------------
G4double GmDataInitialParallelPVCopyNumber::GetValueFromEvent( const G4Event* anEvent, G4int )
{ 
  G4TouchableHistory* touch = new G4TouchableHistory;
  theParallelWorldProcess->fGhostNavigator->LocateGlobalPointAndUpdateTouchable( anEvent->GetPrimaryVertex(0)->GetPosition(), touch, false ); 

  G4double val = touch->GetVolume()->GetCopyNo();
 
  delete touch;

  return val;
}
