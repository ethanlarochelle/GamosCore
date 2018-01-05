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
#include "GmPSeIonisation.hh"
#include "G4Electron.hh"
#include "G4MollerBhabhaModel.hh"
#include "G4UniversalFluctuation.hh"
#include "G4BohrFluctuations.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

GmPSeIonisation::GmPSeIonisation(const G4String& name)
  : GmVPSEnergyLossProcess(name)
{
  //  SetStepFunction(0.2, 1*mm);
  // SetIntegral(true);
  // SetVerboseLevel(1);
  SetProcessSubType(fIonisation);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmPSeIonisation::~GmPSeIonisation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double GmPSeIonisation::MinPrimaryEnergy(const G4ParticleDefinition*,
					 const G4Material*,
					 G4double cut)
{
  G4double x = cut;
  if(isElectron) x += cut;
  return x;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool GmPSeIonisation::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Electron::Electron() || &p == G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmPSeIonisation::InitialiseEnergyLossProcess(
		    const G4ParticleDefinition* part,
		    const G4ParticleDefinition*)
{
  if(!isInitialised) {
    if(part == G4Positron::Positron()) isElectron = false;
    SetSecondaryParticle(theElectron);
    if (!EmModel()) SetEmModel(new G4MollerBhabhaModel());
    EmModel()->SetLowEnergyLimit (MinKinEnergy());
    EmModel()->SetHighEnergyLimit(MaxKinEnergy());
    if (!FluctModel()) SetFluctModel(new G4UniversalFluctuation());
                
    AddEmModel(1, EmModel(), FluctModel());
    isInitialised = true;
  }

#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(infoVerb) ) G4cout << "GmPSeIonisation:GmPSeIonisation  registering model " << MinKinEnergy() << " " << MaxKinEnergy() << " " << EmModel(1) << " " << EmModel(2) << "NM  " << NumberOfModels() << G4endl;
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmPSeIonisation::PrintInfo()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
