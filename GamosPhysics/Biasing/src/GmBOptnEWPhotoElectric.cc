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
#include "GmBOptnEWPhotoElectric.hh"
#include "GmBiasingVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4BiasingProcessInterface.hh"

#include "G4ParticleChangeForLoss.hh"
#include "G4Gamma.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWPhotoElectric::GmBOptnEWPhotoElectric(G4String name)
: GmVEWBiasingOperation(name)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWPhotoElectric::~GmBOptnEWPhotoElectric()
{
}

G4VParticleChange*  
GmBOptnEWPhotoElectric::
ApplyFinalStateBiasing( const G4BiasingProcessInterface* callingProcess,
                        const G4Track*                            track,
                        const G4Step*                              step,
                        G4bool&                                         )
{ 
  G4String history = "";
  G4String newHistory = "";
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWPhotoElectric " << callingProcess->GetProcessName() << " " << fNSplit << " WEI " <<  track->GetWeight() << " density= " << track->GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity()/(CLHEP::g/CLHEP::cm3)  << G4endl; 
#endif

  // -- Now deal with the gamma's:
  // -- their common weight:
  G4double gammaWeight = track->GetWeight();
  
  //  G4int nSplit = 1;  // no splitting

  G4bool bSurvives = true;
  if( track->GetWeight() < 1. ) {
    if( track->GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity() >= theGasDensity ) {
      if( CLHEP::RandFlat::shoot() < fNSplitInv ) { // if it survives RR, increase weight
	gammaWeight *= fNSplit;
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) {
	  G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric gamma ACCEPTED by RR increase weight " << gammaWeight << G4endl; 
	  history += "_NOFAT_NOGAS_Arr";
	}
#endif
      } else {
	bSurvives = false;
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric gamma REJECTED by RR "  << G4endl; 
#endif
      }
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( BiasingVerb(testVerb) ) {
	G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric gamma in GAS " << gammaWeight << G4endl; 
	history += "_NOFAT_GAS";
      }
#endif
    }
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) {
      G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric gamma in GAS " << gammaWeight << G4endl; 
      history += "_FAT";
    }
#endif
  }


  fParticleChange.Initialize(*track);

  if( bSurvives ) {

    G4VParticleChange* processFinalState = callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);
    //    G4cout << " processFinalState NSECOS " <<  processFinalState->GetNumberOfSecondaries() << G4endl; //GDEB
    if ( processFinalState->GetNumberOfSecondaries() == 0 )  return processFinalState;
    // --   - the electron state will be taken as the first one produced by the 
    // --     process, hence the one stored in above processFinalState particle change.
    // --     This state will be stored in our fParticleChange object.
    // -- methods of the concrete G4ParticleChangeForLoss type:
    G4ParticleChangeForLoss* actualParticleChange =
      ( G4ParticleChangeForLoss* ) processFinalState ;
  
    // -- Store electron final state:
    fParticleChange.
      ProposeTrackStatus      ( actualParticleChange->GetTrackStatus() );
    fParticleChange.
      ProposeEnergy           ( actualParticleChange->GetProposedKineticEnergy() );
    fParticleChange.
      ProposeMomentumDirection( actualParticleChange->GetProposedMomentumDirection() );
    
    // -- inform we will have fNSplit e-:
    G4int nSecos = 0;
    //t    fParticleChange.SetNumberOfSecondaries( nSplit + fNSplit );
    // -- inform we take care of secondaries weight (otherwise these
    // -- secondaries are by default given the primary weight).
    fParticleChange.SetSecondaryWeightByProcess(true);
    fParticleChange.ProposeWeight( gammaWeight );
    
    //  actualParticleChange->Clear();
    
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWPhotoElectric fluo gamma  N= " << processFinalState->GetNumberOfSecondaries() << gammaWeight << G4endl; 
#endif
    for ( G4int ii = 0 ; ii < processFinalState->GetNumberOfSecondaries() ; ii++) {
      G4Track* gammaTrack = processFinalState->GetSecondary(ii);
      //--- PROCESS FLUORESCENT GAMMAS
      if( gammaTrack->GetDefinition() == G4Gamma::Gamma() ) { // fluorescent gammas
	//
	//	if( ii > 10 ) break;
	//--- FIRST GAMMA: IN PLANE OR RR
	newHistory = "";
	if( IsInPlane( gammaTrack->GetMomentumDirection(), gammaTrack->GetPosition() ) ) {
	  fParticleChange.AddSecondary( gammaTrack );
	  nSecos++;
	  gammaTrack->SetWeight( gammaWeight/fNSplit );
#ifndef GAMOS_NO_VERBOSE
	  if( BiasingVerb(testVerb) ) {
	    G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric 1st fluo gamma  ACCEPTED in plane " << gammaWeight << G4endl; 
	    newHistory += "FLUO_Aip";
	    G4cout << "CHK " << " PhotoElectric "  << gammaTrack->GetDefinition()->GetParticleName() << " " <<  gammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	  } 
#endif
	} else {
	  if( CLHEP::RandFlat::shoot() < fNSplitInv ) {
	    fParticleChange.AddSecondary( gammaTrack );
	    nSecos++;
	    gammaTrack->SetWeight( gammaWeight );
#ifndef GAMOS_NO_VERBOSE
	    if( BiasingVerb(testVerb) ) {
	      G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric 1st fluo gamma ACCEPTED by RR increase weight " << gammaWeight << G4endl; 
	      newHistory += "FLUO_Arr";
	      G4cout << "CHK " << " PhotoElectric "  << gammaTrack->GetDefinition()->GetParticleName() << " " <<  gammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	    } 
#endif
	  } else {
#ifndef GAMOS_NO_VERBOSE
	    if( BiasingVerb(testVerb) ) G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric 1st fluo gamma REJECTED by RR " << gammaWeight << G4endl; 
#endif
	    delete gammaTrack;
	  }
	}
	if( fabs(gammaWeight-1.) < 1.e-6 )  {
	  // fat fluo gamma: split uniformly in other fNSplit-1
	  G4double gammaKinE = gammaTrack->GetKineticEnergy();
	  G4ThreeVector gammaPos = gammaTrack->GetPosition();
	  G4double gammaTime = gammaTrack->GetGlobalTime();
	  //---- PRODUCE fNSplit-1 FLUO GAMMAS ISOTROPICALLY
	  for( G4int ifluo = 0; ifluo < fNSplit-1; ifluo++ ) {
	    newHistory = "";
	    G4double cosTeta = 2.*G4UniformRand()-1.; 
	    G4double sinTeta = sqrt((1.-cosTeta)*(1.0 + cosTeta));
	    G4double phi     = CLHEP::twopi * G4UniformRand();
	    G4ThreeVector direction(sinTeta*cos(phi), sinTeta*sin(phi), cosTeta);
	    	    
	    //--- NON FIRST GAMMAS: IN PLANE OR RR
	    if( IsInPlane( direction, track->GetPosition() ) ) {
	      G4DynamicParticle* dp =
		new G4DynamicParticle (G4Gamma::Gamma(),direction,gammaKinE);
	      gammaTrack = new G4Track(dp, gammaTime, gammaPos);
	      fParticleChange.AddSecondary( gammaTrack );
	      nSecos++;
	      gammaTrack->SetWeight( gammaWeight/fNSplit );
#ifndef GAMOS_NO_VERBOSE
	      if( BiasingVerb(testVerb) ) {
		G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric " << ifluo << " fluo gamma  ACCEPTED in plane " << gammaWeight/fNSplit << G4endl; 
		newHistory += "FLUO_Aip";
		G4cout << "CHK " << " PhotoElectric "  << gammaTrack->GetDefinition()->GetParticleName() << " " <<  gammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	      } 
#endif
	    } else {	      
	      if( CLHEP::RandFlat::shoot() < fNSplitInv ) {
		G4DynamicParticle* dp =
		  new G4DynamicParticle (G4Gamma::Gamma(),direction,gammaKinE);
		gammaTrack = new G4Track(dp, gammaTime, gammaPos);
		fParticleChange.AddSecondary( gammaTrack );
		nSecos++;
		gammaTrack->SetWeight( gammaWeight );
#ifndef GAMOS_NO_VERBOSE
		if( BiasingVerb(testVerb) ) {
		  G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric " << ifluo << " fluo gamma ACCEPTED by RR increase weight " << gammaWeight << G4endl; 
		  newHistory += "FLUO_Arr";
		  G4cout << "CHK " << " PhotoElectric "  << gammaTrack->GetDefinition()->GetParticleName() << " " <<  gammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
		}
#endif
	      } else {
#ifndef GAMOS_NO_VERBOSE
		if( BiasingVerb(testVerb) ) G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric  " << ifluo << " fluo gamma REJECTED by RR " << G4endl; 
#endif
	      }
	    }
	  }
	} 
      } else {  // ELECTRONS
	fParticleChange.AddSecondary( gammaTrack );
	nSecos++;
	gammaTrack->SetWeight( gammaWeight );
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) G4cout << "CHK " << " PhotoElectric "  << gammaTrack->GetDefinition()->GetParticleName() << " " <<  gammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
#endif
      }
      
    }
    processFinalState->Clear(); 

  } else {
    fParticleChange.ProposeTrackStatus( fStopAndKill );
  }
  
  
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWPhotoElectric NSECOS_FINAL " << fParticleChange.GetNumberOfSecondaries() << G4endl; 
#endif  
 
  // -- we are done:
  // t=? fParticleChange.SetNumberOfSecondaries( nSecos );
  return &fParticleChange;
}



