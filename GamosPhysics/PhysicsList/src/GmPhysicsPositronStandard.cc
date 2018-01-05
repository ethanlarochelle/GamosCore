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
#include "GmPhysicsPositronStandard.hh"
#include "GmAcollinearEplusAnnihilation.hh" //gdl

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "GmPhysicsMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

GmPhysicsPositronStandard::GmPhysicsPositronStandard(const G4String& name,G4int type): G4VPhysicsConstructor(name,type)
{ }

GmPhysicsPositronStandard::~GmPhysicsPositronStandard()
{ }

void GmPhysicsPositronStandard::ConstructProcess()
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
	  manager -> AddProcess(new GmPhysicsMultipleScattering("msc","Positron"), -1, 1,1);
	  manager -> AddProcess(new G4eIonisation,        -1, 2,2);
	  manager -> AddProcess(new G4eBremsstrahlung,    -1, 3,3);
	  G4bool bAcollinear = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPhysicsPositronStandard:ePlusAcollinearAnnihilation",1));
	  if( !bAcollinear ) {
	    manager -> AddProcess(new G4eplusAnnihilation,   0,-1,4); 
	  } else {
	    manager -> AddProcess(new GmAcollinearEplusAnnihilation,   0,-1,4); //gdl
	  }

	}   
    }
}
