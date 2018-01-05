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
#include "GmStepLimiter.hh"
#include "GmUserLimitsMgr.hh"

#include "G4Step.hh"
#include "G4UserLimits.hh"
#include "G4VParticleChange.hh"


////////////////////////////////////
GmStepLimiter::GmStepLimiter(const G4String& aName)
  : G4VProcess(aName)
{
   if (verboseLevel>1) {
     G4cout << GetProcessName() << " is created "<< G4endl;
   }
}


////////////////////////
GmStepLimiter::~GmStepLimiter()
{
}


////////////////////////
GmStepLimiter::GmStepLimiter(GmStepLimiter& right)
  : G4VProcess(right)
{
}

 
////////////////
G4double 
  GmStepLimiter::PostStepGetPhysicalInteractionLength( 
				       const G4Track& aTrack,
				       G4double, // previousStepSize
				       G4ForceCondition* condition  )
{
  // condition is set to "Not Forced"
  *condition = NotForced;

   G4double proposedStep = DBL_MAX;
   G4UserLimits* pUserLimits = GmUserLimitsMgr::GetInstance()->GetUserLimits( aTrack.GetVolume()->GetLogicalVolume(), aTrack.GetDefinition() );

   //                 aTrack.GetVolume()->GetLogicalVolume()->GetUserLimits();
   if (pUserLimits) {
     // max allowed step length
     //
     proposedStep = pUserLimits->GetMaxAllowedStep(aTrack);
     if (proposedStep < 0.) proposedStep =0.; 
   }
   return proposedStep;
}

///////////////
G4VParticleChange*
  GmStepLimiter::PostStepDoIt( const G4Track& aTrack,
                                 const G4Step&  )
// Do Nothing
//
{
   aParticleChange.Initialize(aTrack);
   return &aParticleChange;
}
