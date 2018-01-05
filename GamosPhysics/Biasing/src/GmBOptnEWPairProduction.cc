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
#include "GmBOptnEWPairProduction.hh"
#include "GmBiasingVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4BiasingProcessInterface.hh"

#include "G4ParticleChangeForLoss.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWPairProduction::GmBOptnEWPairProduction(G4String name)
: GmVEWBiasingOperation(name)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWPairProduction::~GmBOptnEWPairProduction()
{
}

G4VParticleChange*  
GmBOptnEWPairProduction::
ApplyFinalStateBiasing( const G4BiasingProcessInterface* callingProcess,
                        const G4Track*                            track,
                        const G4Step*                              step,
                        G4bool&                                         )
{
  G4String history = "";
  G4String newHistory = "";
  G4int nSecos = 0;
  
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWPairProduction " << callingProcess->GetProcessName() << " " << fNSplit << " WEI " <<  track->GetWeight() << " density= " << track->GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity()/(CLHEP::g/CLHEP::cm3) << G4endl; 
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
	  G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPairProduction gamma ACCEPTED by RR increase weight " << gammaWeight << G4endl; 
	  history += "_NOFAT_NOGAS_Arr";
	}
#endif
      } else {
	bSurvives = false;
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) {
	  G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPairProduction gamma REJECTED by RR "  << G4endl; 
	  //-	  history += "_NOFAT_NOGAS_Rrr";
	}
#endif
      }
    }
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) {
      G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPairProduction gamma in GAS " << gammaWeight << G4endl; 
      history += "_FAT";
    }
#endif
  }

  fParticleChange.Initialize(*track);

  if( bSurvives ) {
    G4VParticleChange* processFinalState = callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);
    //    G4cout << " processFinalState NSECOS " <<  processFinalState->GetNumberOfSecondaries() << G4endl; //GDEB
    //-  if ( processFinalState->GetNumberOfSecondaries() == 0 )  return processFinalState;
    G4ParticleChangeForLoss* actualParticleChange =
      ( G4ParticleChangeForLoss* ) processFinalState ;
        
    // -- Store gamma final state:
    fParticleChange.
      ProposeTrackStatus      ( actualParticleChange->GetTrackStatus() );
    fParticleChange.
      ProposeEnergy           ( actualParticleChange->GetProposedKineticEnergy() );
    fParticleChange.
      ProposeMomentumDirection( actualParticleChange->GetProposedMomentumDirection() );
    
    // -- inform we take care of secondaries weight (otherwise these
    // -- secondaries are by default given the primary weight).
    fParticleChange.SetSecondaryWeightByProcess(true);
    fParticleChange.ProposeWeight( gammaWeight );
    
    //  actualParticleChange->Clear();
  
    // -- inform we will have nSplit e+ e-:
    fParticleChange.SetNumberOfSecondaries( 2 );
    for ( G4int ii = 0 ; ii < processFinalState->GetNumberOfSecondaries() ; ii++) {
      G4Track* eTrack = processFinalState->GetSecondary(ii);
      fParticleChange.AddSecondary( eTrack );
      nSecos++;
      eTrack->SetWeight( gammaWeight );
#ifndef GAMOS_NO_VERBOSE
      if( BiasingVerb(testVerb) ) G4cout << "CHK " << " PairProduction "  << eTrack->GetDefinition()->GetParticleName() << " " <<  eTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
#endif
    }
    processFinalState->Clear(); 
  } else {
    fParticleChange.ProposeTrackStatus( fStopAndKill );
  }
    
  // -- we are done:
  //-?  fParticleChange.SetNumberOfSecondaries( nSecos );
 
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWPairProduction NSECOS_FINAL " << fParticleChange.GetNumberOfSecondaries() << G4endl; 
#endif  

  return &fParticleChange;
}



