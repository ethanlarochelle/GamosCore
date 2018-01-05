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
#include "GmMovementEventAction.hh"
#include "GmVMovement.hh"
#include "GmMovementMgr.hh"
#include "GmMovementMessenger.hh"
#include "GamosCore/GamosUtils/include/GmMovementUtils.hh"
#include "GamosCore/GamosGenerator/include/GmGenerator.hh"

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

//---------------------------------------------------------------------
GmMovementEventAction::GmMovementEventAction()
{
  new GmMovementMessenger();
}

//---------------------------------------------------------------------
void GmMovementEventAction::BeginOfEventAction(const G4Event* anEvent)
{
  //--- take as event time that of the first primary vertex
  std::vector<GmVMovement*> movements = GmMovementMgr::GetInstance()->GetNewMovements(anEvent->GetEventID(),anEvent->GetPrimaryVertex(0)->GetT0() );

  std::vector<GmVMovement*>::const_iterator ite;
  for( ite = movements.begin(); ite != movements.end(); ite++) {
    (*ite)->Move();
  }

  // Recalculate generator distribution (in case a GmGenerator GenerDist use volume position)
  if( GmMovementUtils::bUsingGmGenerator ) {
    theGmGenerator->ReInitialiseDistributions();
  }


}

//---------------------------------------------------------------------
void GmMovementEventAction::BeginOfRunAction(const G4Run* )
{
  GmMovementMgr::GetInstance()->InitialiseLastValue();

  if( GmMovementUtils::bUsingGmGenerator ) {
    theGmGenerator = (GmGenerator*)(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  }
}
