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
#include "GmPhysicsPositronZBSStandard.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "GmPSeBremsstrahlung.hh"
#include "GmPSeplusAnnihilation.hh"
//#include "GmEWPSBremsstrahlungModel.hh"
#include "GmBSLivermoreBremsstrahlungModel.hh"
#include "G4eplusAnnihilation.hh"
using namespace CLHEP;

GmPhysicsPositronZBSStandard::GmPhysicsPositronZBSStandard(const G4String& name): G4VPhysicsConstructor(name,111)
{ }

GmPhysicsPositronZBSStandard::~GmPhysicsPositronZBSStandard()
{ }

void GmPhysicsPositronZBSStandard::ConstructProcess()
{
  // Add standard processes for positrons
  
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator -> reset();

  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator -> value();
      G4ProcessManager* manager = particle -> GetProcessManager();
      G4String particleName = particle -> GetParticleName();
     
      if (particleName == "e+") 
	{
	  GmPhysicsMultipleScattering* msc = new GmPhysicsMultipleScattering("msc","Positron");
	  manager -> AddProcess(msc, -1, 1,1);
	  manager -> AddProcess(new G4eIonisation,        -1, 2,2);
	  // Bremsstrahlung
	  GmPSeBremsstrahlung* eBremProcess = new GmPSeBremsstrahlung("GmZBSBrems");
	  //	  GmEWPSBremsstrahlungModel* bremsModel = new GmEWPSBremsstrahlungModel();
	  GmBSLivermoreBremsstrahlungModel* bremsModel = new GmBSLivermoreBremsstrahlungModel();
	  eBremProcess->AddEmModel(0, bremsModel);
	  eBremProcess->AddPSEmModel(bremsModel);
	  manager->AddProcess(eBremProcess, -1,-3, 3);
	  
	  // Annihilation
	  manager -> AddProcess(new G4eplusAnnihilation,   0,-1,4); 

	}   
    }
}
