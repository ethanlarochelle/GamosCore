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
#include "GmPSeplusAnnihilation.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4Gamma.hh"
#include "G4PhysicsVector.hh"
#include "G4PhysicsLogVector.hh"
//#include "GmEWPSeeToTwoGammaModel.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
using namespace std;
using namespace CLHEP;

GmPSeplusAnnihilation::GmPSeplusAnnihilation(const G4String& name)
  : GmVPSGammaProcess(name), isInitialised(false)
{
  SetProcessSubType(fAnnihilation);
  enableAtRestDoIt = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmPSeplusAnnihilation::~GmPSeplusAnnihilation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool GmPSeplusAnnihilation::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double GmPSeplusAnnihilation::AtRestGetPhysicalInteractionLength(
                              const G4Track&, G4ForceCondition* condition)
{
  *condition = NotForced;
  return 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmPSeplusAnnihilation::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) {
    isInitialised = true;
    SetBuildTableFlag(true);
    SetStartFromNullFlag(false);
    SetSecondaryParticle(G4Gamma::Gamma());
    //    if(!Model()) SetModel(new GmEWPSeeToTwoGammaModel());
    EmModel()->SetLowEnergyLimit(MinKinEnergy());
    EmModel()->SetHighEnergyLimit(MaxKinEnergy());
    AddEmModel(1, EmModel());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmPSeplusAnnihilation::PrintInfo()
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VParticleChange* GmPSeplusAnnihilation::AtRestDoIt(const G4Track& aTrack,
                                                     const G4Step& )
//
// Performs the e+ e- annihilation when both particles are assumed at rest.
// It generates two back to back photons with energy = electron_mass.
// The angular distribution is isotropic.
// GEANT4 internal units
//
// Note : Effects due to binding of atomic electrons are negliged.
{
  fParticleChange.InitializeForPostStep(aTrack);

  fParticleChange.SetNumberOfSecondaries(2);

  G4double cosTeta = 2.*G4UniformRand()-1. , sinTeta = sqrt(1.-cosTeta*cosTeta);
  G4double phi     = twopi * G4UniformRand();
  G4ThreeVector direction (sinTeta*cos(phi), sinTeta*sin(phi), cosTeta);
  fParticleChange.AddSecondary( new G4DynamicParticle (G4Gamma::Gamma(),
                                            direction, electron_mass_c2) );
  fParticleChange.AddSecondary( new G4DynamicParticle (G4Gamma::Gamma(),
                                           -direction, electron_mass_c2) );
  // Kill the incident positron
  //
  fParticleChange.ProposeTrackStatus(fStopAndKill);
  return &fParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
