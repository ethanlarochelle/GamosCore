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
//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GmDNAPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmDNAPhysics.hh"
#include "G4ParticleTable.hh"
#include "G4VEmProcess.hh"
#include "G4VEmModel.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmDNAPhysics::GmDNAPhysics() 
: G4VModularPhysicsList()
{
  currentDefaultCut   = 1.0*micrometer;
  cutForGamma         = currentDefaultCut;
  cutForElectron      = currentDefaultCut;
  cutForPositron      = currentDefaultCut;

  SetVerboseLevel(1);

  // EM physics
  theEmDNAPhysics = new G4EmDNAPhysics();
  /*  theHighEnergyModel = "livermore";
  if( theHighEnergyModel == "penelope" ) {
    theEmHighEnergyPhysics = new G4EmPenelopeLEPTSPhysics();
  } else if( theHighEnergyModel == "livermore" ) {
    theEmHighEnergyPhysics = new G4EmLivermoreLEPTSPhysics();
  } else if( theHighEnergyModel == "standard" ) {
    theEmHighEnergyPhysics = new G4EmStandardLEPTSPhysics();
    }*/

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmDNAPhysics::~GmDNAPhysics()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmDNAPhysics::ConstructParticle()
{
  theEmDNAPhysics->ConstructParticle();
  //  theEmHighEnergyPhysics->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmDNAPhysics::ConstructProcess()
{
  // Transportation
  //
  AddTransportation();

  // Electromagnetic physics list
  //
  theEmDNAPhysics->ConstructProcess();
  //  theEmHighEnergyPhysics->ConstructProcess();

  /*  G4double highELimit = 10.*keV;

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){

    G4ParticleDefinition * particle = theParticleIterator->value();
    if( particle != G4Electron::Electron() && G4Positron::Positron() ) continue;x
    G4String               pName = particle->GetParticleName();
    G4ProcessManager     * pManager = particle->GetProcessManager();
    G4ProcessVector* procList = pManager->GetProcessList();
    for( G4int ii = 0; ii < procList->size(); ii++) {
      G4VEmProcess* emProc = static_cast<G4VEmProcess*>((*procList)[ii]);
      if( !emProc ) {
	G4Exception("GmDNAPhysics::ConstructProcess",
		    "",
		    FatalException,
		    (G4String("Process is not Electromagnetic ") + (*procList)[ii]->GetProcessName()).c_str());
      }
      for( G4int ii = 0; ii < 10; ii++ ) {
	G4VEmModel* emModel = emProc->GetModelByIndex(ii,false);
	G4cout << ii << " PROC " << emProc->GetProcessName() << " P_MODEL: " << emModel << G4endl;
	if( emModel ) {
	  G4cout << " PROC " << emProc->GetProcessName() << " MODEL: " << emModel->GetName() << G4endl;
	  emModel->SetHighEnergyLimit(highELimit);
	}
      }
    }
  }
  */

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

void GmDNAPhysics::SetCuts()
{
 // fixe lower limit for cut
 G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100*eV, 1*GeV);
  
 // set cut values for gamma at first and for e- second and next for e+,
 // because some processes for e+/e- need cut values for gamma
 SetCutValue(cutForGamma, "gamma");
 SetCutValue(cutForElectron, "e-");
 SetCutValue(cutForPositron, "e+");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmDNAPhysics::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmDNAPhysics::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmDNAPhysics::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
