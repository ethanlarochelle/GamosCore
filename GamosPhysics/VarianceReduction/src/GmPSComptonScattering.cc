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
#include "GmPSComptonScattering.hh"
//#include "GmEWPSKleinNishinaComptonModel.hh"
#include "G4Electron.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

GmPSComptonScattering::GmPSComptonScattering(const G4String& processName,
  G4ProcessType type):GmVPSGammaProcess (processName, type),
    isInitialised(false)
{
  SetProcessSubType(fComptonScattering);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
GmPSComptonScattering::~GmPSComptonScattering()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmPSComptonScattering::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) {
    isInitialised = true;
    SetBuildTableFlag(true);
    SetSecondaryParticle(G4Electron::Electron());
    //    if(!Model()) SetModel(new GmEWPSKleinNishinaComptonModel);
    EmModel()->SetLowEnergyLimit(MinKinEnergy());
    EmModel()->SetHighEnergyLimit(MaxKinEnergy());
    AddEmModel(1, EmModel());
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
