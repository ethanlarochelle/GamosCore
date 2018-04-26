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
#include "GmEWBSSplittingProcess.hh"
#include "GmBiasingVerbosity.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//------------------------------------------------------------------
GmEWBSSplittingProcess::GmEWBSSplittingProcess(const G4String& processName,
                                           G4ProcessType type)
  : G4VDiscreteProcess(processName, type)
{

#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(infoVerb) ) G4cout << GetProcessName() << " is created " << G4endl;
#endif 

  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  thePlaneMinZ = parMgr->GetNumericValue("EWBS:SplittingPlaneMinZ",9.e9);
  if ( thePlaneMinZ == 9.e9 ) {
    G4Exception("GmEWBSSplittingProcess",
		"EWBS001",
		FatalException,
		"No plane minimum Z defined, please use: /gamos/setParam EWBS:SplittingPlaneMinZ VALUE");
  }

  thePlaneMaxZ = parMgr->GetNumericValue("EWBS:SplittingPlaneMaxZ",9.e9);
  if ( thePlaneMaxZ == 9.e9 ) {
    G4Exception("GmEWBSSplittingProcess",
		"EWBS001",
		FatalException,
		"No plane maximum Z defined, please use: /gamos/setParam EWBS:SplittingPlaneMinZ VALUE");
  }

  theNSplit = G4int(parMgr->GetNumericValue("EWBS:SplittingPlaneNSplit",1));
  theMinimumWeight = parMgr->GetNumericValue("EWBS:SplittingPlaneMinimumWeight",1.);
    
}

//------------------------------------------------------------------
GmEWBSSplittingProcess::~GmEWBSSplittingProcess()
{
} 

//------------------------------------------------------------------
inline G4bool
GmEWBSSplittingProcess::IsApplicable(const G4ParticleDefinition& aParticleType)
{
  return ( &aParticleType == G4Electron::ElectronDefinition()
	   || &aParticleType == G4Positron::PositronDefinition() );
}
/*
//------------------------------------------------------------------
G4double GmEWBSSplittingProcess::PostStepGetPhysicalInteractionLength(
                             const G4Track& ,
			     G4double ,
			     G4ForceCondition* condition)
{
  *condition = StronglyForced;

  return DBL_MAX; 
}
*/
//------------------------------------------------------------------
G4VParticleChange*
GmEWBSSplittingProcess::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
 
  pParticleChange->Initialize(aTrack);

  if( aTrack.GetWeight() < theMinimumWeight ) return pParticleChange;
  double posZ = aStep.GetPreStepPoint()->GetPosition().z();
#ifndef BEAMZPOS
  if( posZ > thePlaneMinZ || posZ < thePlaneMaxZ ) return pParticleChange;
#else
  if( posZ < thePlaneMinZ || posZ > thePlaneMaxZ ) return pParticleChange;
#endif
    
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(debugVerb) ) G4cout << &aTrack << " GmEWBSSplittingProcess::PostStepDoIt  weight " << aTrack.GetWeight() << G4endl;
#endif

  //----- If reached OutOfWorld do not split
  if( !aStep.GetPostStepPoint()->GetPhysicalVolume() ) return pParticleChange;
  
  //----- Get splitting number from distribution
  if( theNSplit <= 0 || fabs(theNSplit-1) < 1.E-9 ) return pParticleChange;

  if( theNSplit > 1. ) {
    G4double newWeight = aTrack.GetWeight() / (theNSplit);
    pParticleChange->ProposeParentWeight( newWeight );
    
    G4ParticleDefinition* particle = aTrack.GetDefinition();
    G4ThreeVector dir = aTrack.GetMomentumDirection();
    G4double ener = aTrack.GetKineticEnergy();
    G4double time = aTrack.GetGlobalTime();
    G4ThreeVector pos = aTrack.GetPosition();
    
    pParticleChange->SetNumberOfSecondaries(pParticleChange->GetNumberOfSecondaries()+theNSplit-1) ;
    
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(debugVerb) ) G4cout << " GmImportanceSamplingProcess::PostStepDoIt splitting particle " << theNSplit << " new weight " << newWeight << " old weight " << aTrack.GetWeight() << G4endl;
#endif

    for( G4int ii = 0; ii < theNSplit-1; ii++ ) {
      //------ Create a repeated track
      G4Track* newTrack = new G4Track( new G4DynamicParticle( particle, dir, ener ), time, pos );
      //      newTrack->SetGoodForTrackingFlag();
      //   newTrack->SetTouchableHandle(aStep.GetPostStepPoint()->GetTouchableHandle());
      pParticleChange->AddSecondary(newTrack);
      newTrack->SetWeight( newWeight );
    }
    
  }
    
  ClearNumberOfInteractionLengthLeft();
  
  return pParticleChange;
  //t  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

G4double GmEWBSSplittingProcess::GetMeanFreePath(const G4Track&, 
                                               G4double ,
                                               G4ForceCondition* condition)
{
  *condition = StronglyForced;

  return DBL_MAX;
}
