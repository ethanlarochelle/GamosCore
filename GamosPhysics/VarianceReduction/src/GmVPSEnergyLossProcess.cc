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
#include "GmVPSEnergyLossProcess.hh"
#include "GmVPSModelHelper.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#ifndef GAMOS_NO_VERBOSE
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#endif
#include "G4Gamma.hh"
#include "G4BremsstrahlungCrossSectionHandler.hh"

//-------------------------------------------------------------------------
GmVPSEnergyLossProcess::GmVPSEnergyLossProcess(const G4String& aName, G4ProcessType aType)
  : G4VEnergyLossProcess(aName,aType)
{
}

//-------------------------------------------------------------------------
G4VParticleChange* GmVPSEnergyLossProcess::PostStepDoIt(const G4Track& track,
							const G4Step& )
{
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(testVerb) ) G4cout << " G4VParticleChange* GmVPSEnergyLossProcess::PostStepDoIt(const G4Track& track " << G4endl;
#endif

  G4double finalT = track.GetKineticEnergy();
  if(finalT <= lowestKinEnergy) return &fParticleChange;

  G4double postStepScaledEnergy = finalT*massRatio;

  if(!currentModel->IsActive(postStepScaledEnergy)) return &fParticleChange;
  /*
  if(1 < verboseLevel) {
    G4cout << GetProcessName()
           << "::PostStepDoIt: E(MeV)= " << finalT/CLHEP::MeV
	   << G4endl;
  }
  */
  // Integral approach
  if (integral) {
    G4double lx = GetLambdaForScaledEnergy(postStepScaledEnergy);
    /*
    if(preStepLambda<lx && 1 < verboseLevel && nWarnings<200) {
      G4cout << "WARNING: for " << particle->GetParticleName()
             << " and " << GetProcessName()
             << " E(CLHEP::MeV)= " << finalT/CLHEP::MeV
             << " preLambda= " << preStepLambda 
	     << " < " << lx << " (postLambda) "
	     << G4endl;
      nWarnings++;
    }
    */
    if(preStepLambda*G4UniformRand() > lx) {
      ClearNumberOfInteractionLengthLeft();
      return &fParticleChange;
    }
  }

  SelectModel(postStepScaledEnergy);
  /*g494  if(useDeexcitation) {
    currentModel->SetDeexcitationFlag(idxDERegions[currentMaterialIndex]);
    }*/

  const G4DynamicParticle* dynParticle = track.GetDynamicParticle();
  //g495  G4double tcut = (*theCuts)[currentMaterialIndex];
  G4double tcut = (*theCuts)[currentCoupleIndex];

  // sample secondaries
  secParticles.clear();
  std::vector<G4double> secondaryWeights;
  
  thePSModel->SampleSecondariesPS(&secParticles, secondaryWeights, currentCouple, dynParticle, tcut, 100.*CLHEP::TeV, track );
  //  G4cout << " GmVPSEnergyLoss NSECO " << secParticles.size() << " Z " << track.GetPosition().z() << G4endl;

  //  secondaryWeights *= track.Getweight()

  // save secondaries
  G4int num = secParticles.size();
  fParticleChange.SetNumberOfSecondaries(num);
   for (G4int ii=0; ii<num; ++ii) {
    //  for (ites=secondaries->begin(), itew=secondaryWeights.begin(); ites!=secondaries->end(); ites++,itew++) {
    // create track
    G4Track* aTrack = new G4Track(secParticles[ii], track.GetGlobalTime(), track.GetPosition());
    // set IsGoodGorTrackingFlag
    aTrack->SetGoodForTrackingFlag();
    // touchable handle is copied to keep the pointer
    aTrack->SetTouchableHandle(track.GetTouchableHandle());
    // set weight
    fParticleChange.AddSecondary(aTrack); // fParticleChange is G4ParticleChangeForLoss
    aTrack->SetWeight(secondaryWeights[ii]); // this has to be done later, beacuse AddSecondary takes the one from the parent
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " GmVPSEnergyLossProcess: final wgt " << aTrack->GetWeight() << " = " << secondaryWeights[ii] << G4endl;
#endif
      // add the secondary

#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "GmVPSEnergyLossProcess: SECONDARY " << ii  
	   << " " << aTrack->GetDefinition()->GetParticleName()
	   << " MOM " << aTrack->GetMomentumDirection()
	   << " POS " << aTrack->GetPosition()
	   << " ENE " << aTrack->GetKineticEnergy()
	   << " WEIGHT " << aTrack->GetWeight() << G4endl;
#endif
  }
#ifndef GAMOS_NO_VERBOSE
  //  if( PhysicsVerb(debugVerb) ) G4cout << "GmVPSEnergyLossProcess: N SECO " << fParticleChange.GetNumberOfSecondaries() << " = "
  //	 << secParticles.size() << G4endl;
#endif

  /*
  if(1 < verboseLevel) {
    G4cout << "::PostStepDoIt: Sample secondary; Efin= " 
    << fParticleChange.GetProposedKineticEnergy()/CLHEP::MeV
           << " CLHEP::MeV; model= (" << currentModel->LowEnergyLimit()
           << ", " <<  currentModel->HighEnergyLimit() << ")"
           << "  preStepLambda= " << preStepLambda
           << "  dir= " << track.GetMomentumDirection()
           << "  status= " << track.GetTrackStatus()
           << G4endl;
  }
  */
  ClearNumberOfInteractionLengthLeft();
  return &fParticleChange;
}
