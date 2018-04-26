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
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
// ----------------------------------------------------------------------------
//                 GEANT 4 - Hadrontherapy example
// ----------------------------------------------------------------------------
// Code developed by:
//
// G.A.P. Cirrone(a)*, G. Candiano, F. Di Rosa(a), S. Guatelli(b), G. Russo(a)
// 
// (a) Laboratori Nazionali del Sud 
//     of the National Institute for Nuclear Physics, Catania, Italy
// (b) National Institute for Nuclear Physics Section of Genova, genova, Italy
// 
// * cirrone@lns.infn.it
// --------------------------------------------------------------

#include "GmPhysicsElectronStandard.hh"
#include "GmPhysicsMultipleScattering.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4SeltzerBergerModel.hh"

GmPhysicsElectronStandard::GmPhysicsElectronStandard(const G4String& name, G4int type): GmVPhysicsElectron(name,type)
{ }

GmPhysicsElectronStandard::~GmPhysicsElectronStandard()
{ }

void GmPhysicsElectronStandard::ConstructProcess()
{
  // Add standard processes for electrons
  
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator->reset();

  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* manager = particle->GetProcessManager();
      G4String particleName = particle->GetParticleName();
     
      if (particleName == "e-") 
	{
	  manager->AddProcess(new GmPhysicsMultipleScattering("msc","Electron"), -1, 1, 1);
	  manager->AddProcess(new G4eIonisation,  -1, 2, 2);
	  G4SeltzerBergerModel* model = new G4SeltzerBergerModel();
	  G4eBremsstrahlungRelModel* model2 = new G4eBremsstrahlungRelModel();
	  G4eBremsstrahlung* brems = new G4eBremsstrahlung();
	  model->SetLowEnergyLimit(brems->MinKinEnergy());
	  //	  model->SetHighEnergyLimit(model2->LowEnergyLimit());
	  model->SetHighEnergyLimit(1.*CLHEP::GeV);
	  model2->SetHighEnergyLimit(brems->MaxKinEnergy());                
	  /*	  G4VEmFluctuationModel* fm = 0;
	  brems->AddEmModel(0, model, fm);
	  brems->AddEmModel(1, model2, fm); */
	  brems->AddEmModel(0, model);
	  brems->AddEmModel(1, model2);
	  SelectBremssAngularDist( model, "Electron" );
	  manager->AddProcess(brems,  -1, 3, 3); 
	}   
    }
}

