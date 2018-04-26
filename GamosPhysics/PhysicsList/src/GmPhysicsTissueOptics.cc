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
#include "GmPhysicsTissueOptics.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4OpTissueAbsorption.hh"
#include "G4OpTissueRayleigh.hh"
#include "G4OpTissueMieHG.hh"
#include "G4OpTissueFluor.hh"
#include "G4OpTissueScatter.hh"
#include "G4OpTissueBoundaryProcess.hh"
#include "G4OpticalPhoton.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

GmPhysicsTissueOptics::GmPhysicsTissueOptics()
{ }

GmPhysicsTissueOptics::~GmPhysicsTissueOptics()
{ }

void GmPhysicsTissueOptics::ConstructParticle()
{
  G4OpticalPhoton::OpticalPhoton();
}

void GmPhysicsTissueOptics::ConstructProcess()
{
  // Add processes for optical gammas
  
  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator -> reset();

  G4bool bFound = false;

  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator -> value();
      G4ProcessManager* pmanager = particle -> GetProcessManager();
      G4String particleName = particle -> GetParticleName();    
      
	if (particleName == "opticalphoton") 
	{
	  bFound = true;
	  if( GmG4Utils::CheckProcessExists( pmanager, "OpTissueAbsorption" )) break;
	  pmanager -> AddDiscreteProcess(new G4OpTissueAbsorption);
	  if( GmG4Utils::CheckProcessExists( pmanager, "OpTissueRayleigh" )) break;
	  pmanager -> AddDiscreteProcess(new G4OpTissueRayleigh);
	  if( GmG4Utils::CheckProcessExists( pmanager, "OpTissueMieHG" )) break;
	  pmanager -> AddDiscreteProcess(new G4OpTissueMieHG);
	  if( GmG4Utils::CheckProcessExists( pmanager, "OpTissueFluor" )) break;
	  pmanager -> AddDiscreteProcess(new G4OpTissueFluor);
	  if( GmG4Utils::CheckProcessExists( pmanager, "OpTissueScatter" )) break;
	  pmanager -> AddDiscreteProcess(new G4OpTissueScatter);
	  if( GmG4Utils::CheckProcessExists( pmanager, "OpTissueBoundary" )) break;
	  G4OpTissueBoundaryProcess* theBoundaryProcess  = new G4OpTissueBoundaryProcess();
	  theBoundaryProcess->SetModel(unified);
	  pmanager -> AddDiscreteProcess(theBoundaryProcess);
	}   
    }

  if( !bFound ) {
    G4Exception("GmPhysicsTissueOptics::ConstructProcess",
		"Fatal error",
		FatalErrorInArgument,
		"tissue-optics processes cannot be added if opticalphoton particle is not created, please add the command '/gamos/phsysics/createParticle opticalphoton' before '/run/initialize'");
  }

}
