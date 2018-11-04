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
#include "GmPhysicsScintillation.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4OpticalPhoton.hh"
#include "G4OpScintillation.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

GmPhysicsScintillation::GmPhysicsScintillation()
{ }

GmPhysicsScintillation::~GmPhysicsScintillation()
{ }

void GmPhysicsScintillation::ConstructParticle()
{
  G4OpticalPhoton::OpticalPhoton();
}

void GmPhysicsScintillation::ConstructProcess()
{
  // Add scintillation process
  
  G4OpScintillation* theScintillationProcess = new G4OpScintillation();
  G4double yieldFactor = GmParameterMgr::GetInstance()->GetNumericValue("GmPhysicsScintillation:YieldFactor",1.);
  theScintillationProcess->SetScintillationYieldFactor(yieldFactor);
  
  G4bool bTrackSeco1st = G4bool( GmParameterMgr::GetInstance()->GetNumericValue("GmPhysicsScintillation:TrackSecondariesFirst",1));
  theScintillationProcess->SetTrackSecondariesFirst(bTrackSeco1st);
  
  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator -> reset();

  G4bool bFound = false;

  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator -> value();
      G4ProcessManager* pmanager = particle -> GetProcessManager();
      G4String particleName = particle -> GetParticleName();
      
      if (theScintillationProcess->IsApplicable(*particle)) 
	{
	  if( GmG4Utils::CheckProcessExists( pmanager, "Scintillation" )) break;
	  pmanager->AddProcess(theScintillationProcess);
	  pmanager->SetProcessOrderingToLast(theScintillationProcess, idxAtRest);
	  pmanager->SetProcessOrderingToLast(theScintillationProcess, idxPostStep);
	}     
    }
}
