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
#include "GmPhysicsXrayRefraction.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "GmXrayRefraction.hh"
#include "G4Gamma.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

// ---------------------------------------------------------------------------
GmPhysicsXrayRefraction::GmPhysicsXrayRefraction()
{ }

// ---------------------------------------------------------------------------
GmPhysicsXrayRefraction::~GmPhysicsXrayRefraction()
{ }

// ---------------------------------------------------------------------------
void GmPhysicsXrayRefraction::ConstructProcess()
{
  // Add GmXrayRefraction for gammas
  
  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();

  theParticleIterator->reset();
  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator -> value();

      if (particle == G4Gamma::Gamma()) 
	{
	  G4ProcessManager* pmanager = particle -> GetProcessManager();
	  if( GmG4Utils::CheckProcessExists( pmanager, "XrayRefraction" )) break;
	  pmanager -> AddDiscreteProcess(new GmXrayRefraction);
	}   
    }
}
