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
#include "GmPSeBremsstrahlung.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#include "G4Gamma.hh"
//#include "GmEWPSBremsstrahlungModel.hh"
#include "GmBSLivermoreBremsstrahlungModel.hh"
#include "G4LossTableManager.hh"
using namespace CLHEP;

//-------------------------------------------------------------------------
GmPSeBremsstrahlung::GmPSeBremsstrahlung(const G4String& aName)
  : GmVPSEnergyLossProcess(aName),
  isInitialised(false)
{
  SetProcessSubType(fBremsstrahlung);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool GmPSeBremsstrahlung::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Electron::Electron() || &p == G4Positron::Positron());
}

//-------------------------------------------------------------------------
void GmPSeBremsstrahlung::InitialiseEnergyLossProcess(
                                                const G4ParticleDefinition* ,
                                                const G4ParticleDefinition* )
{
  if(!isInitialised) {
    /*t    particle = p;
    SetSecondaryParticle(G4Gamma::Gamma());
    SetIonisation(false); */
    G4EmParameters* param = G4EmParameters::Instance();

    G4double emin = param->MinKinEnergy();
    G4double emax = param->MaxKinEnergy();
    G4double energyLimit = std::min(emax, GeV);
    G4VEmFluctuationModel* fm = 0;

    if (!EmModel(1)) SetEmModel(new GmBSLivermoreBremsstrahlungModel(), 1);
    EmModel(1)->SetLowEnergyLimit(emin);
    EmModel(1)->SetHighEnergyLimit(energyLimit);
    EmModel(1)->SetSecondaryThreshold(param->BremsstrahlungTh());
    EmModel(1)->SetLPMFlag(false);
    AddEmModel(1, EmModel(1), fm);

    /*GAMOS    if(emax > energyLimit) {
      if (!EmModel(2)) { SetEmModel(new G4eBremsstrahlungRelModel(), 2); }
      EmModel(2)->SetLowEnergyLimit(energyLimit);
      EmModel(2)->SetHighEnergyLimit(emax); 
      EmModel(2)->SetSecondaryThreshold(param->BremsstrahlungTh());
      EmModel(2)->SetLPMFlag(param->LPM());
      AddEmModel(2, EmModel(2), fm);
      } */
    isInitialised = true;
  }

#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(infoVerb) ) G4cout << "GmPSeBremsstrahlung::GmPSeBremsstrahlung  registering model " << MinKinEnergy() << " " << MaxKinEnergy() << " " << EmModel(1) << " " << EmModel(2) << "NM  " << NumberOfModels() << G4endl;
#endif
  //GAMOS  EmModel(2)->SetLPMFlag(man->LPMFlag());

}


void GmPSeBremsstrahlung::PrintInfo()
{
  if(EmModel(1)) {
    G4EmParameters* man = G4EmParameters::Instance(); 
    G4double eth = man->BremsstrahlungTh(); 
    G4cout << "      LPM flag: " << man->LPM() << " for E > " 
	   << EmModel(1)->HighEnergyLimit()/GeV << " GeV";
    if(eth < DBL_MAX) G4cout << ",  HighEnergyThreshold(GeV)= " << eth/GeV; 
    G4cout << G4endl;
  }
}
