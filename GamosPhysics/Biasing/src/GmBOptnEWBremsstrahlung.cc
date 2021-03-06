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
#include "GmBOptnEWBremsstrahlung.hh"
#include "GmBiasingVerbosity.hh"

#include "G4BiasingProcessInterface.hh"

#include "G4ParticleChangeForLoss.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWBremsstrahlung::GmBOptnEWBremsstrahlung(G4String name)
: GmVEWBiasingOperation(name)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWBremsstrahlung::~GmBOptnEWBremsstrahlung()
{
}

G4VParticleChange*  
GmBOptnEWBremsstrahlung::
ApplyFinalStateBiasing( const G4BiasingProcessInterface* callingProcess,
                        const G4Track*                            track,
                        const G4Step*                              step,
                        G4bool&                                         )
{
  G4String history = "";
  G4String newHistory = "";
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWBremsstrahlung " << callingProcess->GetProcessName() << " " << fNSplit << G4endl; 
#endif

  G4int nSplit = fNSplit;
  if( track->GetWeight() < 1 ) nSplit = 1;

  // -- a special case here: the process corrects for cross-section change
  // -- over the step due to energy loss by sometimes "abandoning" the interaction,
  // -- returning an unchanged incoming electron/positron.
  // -- We respect this correction, and if no secondary is produced, its means this
  // -- case is happening:
  //  if ( processFinalState->GetNumberOfSecondaries() == 0 )  return processFinalState;

  // -- Now deal with the gamma's:
  // -- their common weight:
  G4double gammaWeight = track->GetWeight() / nSplit;
  G4double gammaRRSurvivesWeight = gammaWeight * fNSplit ;
  
  // -- inform we will have fNSplit gamma's:
  G4int nSecos = 0;


  // -- now start the fNSplit calls to the process to store each
  // -- related gamma:
  G4int nCalls = 1;
  while ( nCalls <= nSplit ) {
    
    // ( note: we don't need to cast to actual type here, as methods for accessing
    //   secondary particles are from base class G4VParticleChange )
    G4VParticleChange* processFinalState = callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);
    if ( nSplit == 1 ) {
#ifndef GAMOS_NO_VERBOSE
      if( BiasingVerb(testVerb) ) {
	if( processFinalState->GetNumberOfSecondaries() != 0 )  {
	  G4cout << " GmBOptnEWBremsstrahlung NOT FAT = NO SPLITTING " <<  processFinalState->GetNumberOfSecondaries() << G4endl; //GDEB
	  G4Track* gammaTrack = processFinalState->GetSecondary(0);
	  history = "_NONFAT";
	  G4cout << "CHK " << " Bremsstrahlung " << gammaTrack->GetDefinition()->GetParticleName() << " " << gammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	}
      }
#endif
      return processFinalState;
    }

    //    G4cout << " processFinalState NSECOS " <<  processFinalState->GetNumberOfSecondaries() << G4endl; //GDEB
    if( nCalls == 1 ) {
      //      if ( processFinalState->GetNumberOfSecondaries() == 0 )  return processFinalState;
      // --   - the electron state will be taken as the first one produced by the 
      // --     process, hence the one stored in above processFinalState particle change.
      // --     This state will be stored in our fParticleChange object.
      // -- methods of the concrete G4ParticleChangeForLoss type:
      G4ParticleChangeForLoss* actualParticleChange =
	( G4ParticleChangeForLoss* ) processFinalState ;
      
      fParticleChange.Initialize(*track);
      
      // -- Store electron final state:
      fParticleChange.
	ProposeTrackStatus      ( actualParticleChange->GetTrackStatus() );
      fParticleChange.
	ProposeEnergy           ( actualParticleChange->GetProposedKineticEnergy() );
      fParticleChange.
	ProposeMomentumDirection( actualParticleChange->GetProposedMomentumDirection() );
      
      // -- inform we will have nSplit gamma's:
      fParticleChange.SetNumberOfSecondaries( nSplit );
      // -- inform we take care of secondaries weight (otherwise these
      // -- secondaries are by default given the primary weight).
      fParticleChange.SetSecondaryWeightByProcess(true);
      
      //-      actualParticleChange->Clear();
      
    }

#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWBremsstrahlung NSECOS0 " <<  processFinalState->GetNumberOfSecondaries() << G4endl; //GDEB
#endif

    for ( G4int ii = 0 ; ii < processFinalState->GetNumberOfSecondaries() ; ii++) {
      //	if( ii > 10 ) break;
      G4Track* gammaTrack = processFinalState->GetSecondary(ii);
      if( IsInPlane( gammaTrack->GetMomentumDirection(), gammaTrack->GetPosition() ) ) {
	fParticleChange.AddSecondary( gammaTrack );
	nSecos++;
	gammaTrack->SetWeight( gammaWeight );
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) {
	  G4cout << " GmBOptnEWBremsstrahlung ACCEPTED In Plane " << G4endl; //GDEB
	  newHistory = "_Aip";
	  G4cout << "CHK " << " Bremsstrahlung "  <<gammaTrack->GetDefinition()->GetParticleName() << " " << gammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	}
#endif
      } else {
	G4double randn = CLHEP::RandFlat::shoot();
	if(randn < fNSplitInv ) {
	  fParticleChange.AddSecondary( gammaTrack );
	  nSecos++;
	  gammaTrack->SetWeight( gammaRRSurvivesWeight );
#ifndef GAMOS_NO_VERBOSE
	  if( BiasingVerb(testVerb) ) {
	    G4cout << " GmBOptnEWBremsstrahlung ACCEPTED by RR " << randn << " < " << fNSplitInv << G4endl; //GDEB
	    newHistory = "_Arr";
	    G4cout << "CHK " << " Bremsstrahlung "  <<gammaTrack->GetDefinition()->GetParticleName() << " " << gammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	  }
#endif
	} else {
	  delete gammaTrack;
#ifndef GAMOS_NO_VERBOSE
	  if( BiasingVerb(testVerb) ) {
	    G4cout << " GmBOptnEWBremsstrahlung REJECTED by RR " << randn << " >= " << fNSplitInv << G4endl; //GDEB
	  }
#endif
	}
      }
      
    }
    processFinalState->Clear(); 

    nCalls++;

  }
  
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWBremsstrahlung NSECOS_FINAL " << fParticleChange.GetNumberOfSecondaries() << " = " << nSecos << G4endl; //GDEB
#endif
  
  // -- we are done:
  //-? fParticleChange.SetNumberOfSecondaries( nSecos );
  return &fParticleChange;
}



