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
#include "GmMinRangeLimitsStudyCreateInfoUA.hh"
#include "GmMinRangeLimitsStudyUA.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"

//----------------------------------------------------------------
GmMinRangeLimitsStudyCreateInfoUA::GmMinRangeLimitsStudyCreateInfoUA(GmMinRangeLimitsStudyUA* mr) : theMinRangeUA( mr )
{
  theName = "GmMinRangeLimitsStudyCreateInfoUA";
  bInfoCreated = false;
  
}

//----------------------------------------------------------------
void GmMinRangeLimitsStudyCreateInfoUA::UserSteppingAction(const G4Step* aStep )
{
  //check at least one seco is created
  //--- create for all, because when an step is accepted, it has to accept all previous steps  if( fpSteppingManager->GetfN2ndariesAtRestDoIt() + fpSteppingManager->GetfN2ndariesAlongStepDoIt() + fpSteppingManager->GetfN2ndariesPostStepDoIt() != 0 ){ 
    //    if( !GmCutsStudyMgr::GetInstance()->GetTrackStepInfo( aStep->GetTrack()->GetParentID(), aStep->GetTrack()->GetPosition() )) { 
    if( !bInfoCreated ) {
      //      G4cout << " GmMinRangeLimitsStudyCreateInfoUA " << G4endl;
      theMinRangeUA->CreateTrackStepInfo( aStep->GetTrack() );
    }
    //  }
  bInfoCreated = false;

}
