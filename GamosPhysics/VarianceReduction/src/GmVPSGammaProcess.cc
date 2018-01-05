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
#include "GmVPSGammaProcess.hh"
#ifndef GAMOS_NO_VERBOSE
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#endif
#include "G4LossTableManager.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VEmModel.hh"
#include "G4DataVector.hh"
#include "G4PhysicsTable.hh"
#include "G4PhysicsVector.hh"
#include "G4PhysicsLogVector.hh"
#include "G4VParticleChange.hh"
#include "G4ProductionCutsTable.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4PhysicsTableHelper.hh"
#include "G4EmConfigurator.hh"
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmVPSGammaProcess::GmVPSGammaProcess(const G4String& name, G4ProcessType type):
  G4VEmProcess(name, type)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmVPSGammaProcess::~GmVPSGammaProcess()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4VParticleChange* GmVPSGammaProcess::PostStepDoIt(const G4Track& track,
                                              const G4Step&)
{
  fParticleChange.InitializeForPostStep(track);

  // Do not make anything if particle is stopped, the annihilation then
  // should be performed by the AtRestDoIt!
  if (track.GetTrackStatus() == fStopButAlive) return &fParticleChange;

  G4double finalT = track.GetKineticEnergy();

  // Integral approach
  if (integral) {
    G4double lx = GetLambda(finalT, currentCouple);
    if(preStepLambda<lx && 1 < verboseLevel) {
      G4cout << "WARNING: for " << particle->GetParticleName() 
             << " and " << GetProcessName()
             << " E(MeV)= " << finalT/CLHEP::MeV
             << " preLambda= " << preStepLambda << " < " << lx << " (postLambda) "
	     << G4endl;  
    }

    if(preStepLambda*G4UniformRand() > lx) {
      ClearNumberOfInteractionLengthLeft();
      return &fParticleChange;
    }
  }

  SelectModel(finalT, currentCoupleIndex);
  if(!currentModel->IsActive(finalT)) return &fParticleChange;
  /*g494  if(useDeexcitation) {
    currentModel->SetDeexcitationFlag(idxDERegions[currentCoupleIndex]);
    }*/
  /*  
  if(0 < verboseLevel) {
    G4cout << "GmVPSGammaProcess::PostStepDoIt: Sample secondary; E= "
           << finalT/CLHEP::MeV
           << " MeV; model= (" << currentModel->LowEnergyLimit()
           << ", " <<  currentModel->HighEnergyLimit() << ")"
           << G4endl;
  }
  */

  
  // sample secondaries
  secParticles.clear();

  std::vector<G4double> secondaryWeights;
  //  G4cout << this << " SAMPLE " << thePSModel << " " << GetProcessName() << G4endl;
  thePSModel->SampleSecondariesPS(&secParticles, 
				  secondaryWeights, 
				  currentCouple, 
				  track.GetDynamicParticle(), 
				  (*theCuts)[currentCoupleIndex], 
				  100.*CLHEP::TeV, 
				  track );

  // save secondaries
  G4int num = secParticles.size();
  G4int numGamma = 0;

#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "GmVPSGammaProcess: N SECO " << num << G4endl;
#endif
#endif
  
  fParticleChange.SetNumberOfSecondaries(num);
  G4double edep = fParticleChange.GetLocalEnergyDeposit();
  
  for (G4int ii=0; ii<num; ii++) {
    G4DynamicParticle* dp = secParticles[ii];
    const G4ParticleDefinition* part = dp->GetDefinition();
    G4double kinE = dp->GetKineticEnergy();
    G4bool good = true;
    if(applyCuts) {
      if (part == theGamma) {
	if (kinE< (*theCutsGamma)[currentCoupleIndex]) good = false;
	
      } else if (part == theElectron) {
	if (kinE< (*theCutsElectron)[currentCoupleIndex]) good = false;
	
      } else if (part == thePositron) {
	if (electron_mass_c2 < (*theCutsGamma)[currentCoupleIndex] &&
	    kinE< (*theCutsPositron)[currentCoupleIndex]) {
	  good = false;
	  kinE+= 2.0*electron_mass_c2;
	}
      }
      if(!good) {
	delete dp;
	edep += kinE;
      }
      G4cout << " APPLY CUTS " << applyCuts << G4endl;
    }
    if (good) {
      if (part == theGamma) numGamma++;
      if( numGamma == 1 && part == theGamma && track.GetDefinition() == theGamma ) {
	fParticleChange.ProposeMomentumDirection(dp->GetMomentumDirection());
	fParticleChange.SetProposedKineticEnergy(dp->GetKineticEnergy());
	fParticleChange.ProposeParentWeight(secondaryWeights[ii]);
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << " GmVPSGammaProcess: SECONDARY : setting variables of original gamma " 
	       << " MOMDIR " << fParticleChange.GetProposedMomentumDirection() 
	       << " POS " << track.GetPosition()
	       << " ENE " << fParticleChange.GetProposedKineticEnergy() 
	       << " WEIGHT " << fParticleChange.GetParentWeight() 
	       << G4endl;
#endif
#endif
      }else {
	G4Track* aTrack = new G4Track(secParticles[ii], track.GetGlobalTime(), track.GetPosition());
	// set IsGoodGorTrackingFlag
	aTrack->SetGoodForTrackingFlag();
	// touchable handle is copied to keep the pointer
	aTrack->SetTouchableHandle(track.GetTouchableHandle());
	// set weight
	// add the secondary
	fParticleChange.AddSecondaryTrack(aTrack); // fParticleChange is G4ParticleChangeForLoss
	aTrack->SetWeight(secondaryWeights[ii]); // this has to be done later, beacuse AddSecondary takes the one from the parent
	
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "GmVPSGammaProcess: SECONDARY " << ii 
	       << " " << aTrack->GetDefinition()->GetParticleName()
	       << " MOMDIR " << aTrack->GetMomentumDirection()
	       << " POS " << aTrack->GetPosition()
	       << " ENE " << aTrack->GetKineticEnergy()
	       << " WEIGHT " << aTrack->GetWeight() << G4endl;
#endif
#endif
      }
    }
  } 
  fParticleChange.ProposeLocalEnergyDeposit(edep);
  
  if( track.GetDefinition() == theGamma && numGamma == 0 ) {
    //no gamma accepted, killl current gamma
    fParticleChange.ProposeTrackStatus(fStopAndKill);
    fParticleChange.ProposeLocalEnergyDeposit(track.GetKineticEnergy());
  }

  ClearNumberOfInteractionLengthLeft();
  return &fParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool GmVPSGammaProcess::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Gamma::Gamma());
}

void GmVPSGammaProcess::PrintInfo()
{}         
