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
// ---------------------------------------------------------------------------
#include "globals.hh"
#include "GmPhysicsParallel.hh"

#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParallelWorldProcess.hh"
#include "G4VUserParallelWorld.hh"
#include "G4tgbParallelGeomMgr.hh"
#include "G4UIcommand.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

// ---------------------------------------------------------------------------
GmPhysicsParallel::GmPhysicsParallel()
{
}

// ---------------------------------------------------------------------------
GmPhysicsParallel::~GmPhysicsParallel()
{
}

// ---------------------------------------------------------------------------
void GmPhysicsParallel::ConstructProcess()
{
  G4ParallelWorldProcess* theParallelWorldProcess
    = new G4ParallelWorldProcess("ParaWorldProc");
  theParallelWorldProcess->SetParallelWorld("ParallelWorld");
  if( G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPhysicsParallel:LayeredMaterial",0)) ) {
    theParallelWorldProcess->SetLayeredMaterialFlag();
  }

  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    pmanager->AddProcess(theParallelWorldProcess);
    //    pmanager->SetProcessOrderingToLast(theParallelWorldProcess, idxAtRest);
    //    pmanager->SetProcessOrdering(theParallelWorldProcess, idxAlongStep, 1);
    //   pmanager->SetProcessOrderingToLast(theParallelWorldProcess, idxPostStep);
    pmanager->SetProcessOrdering(theParallelWorldProcess, idxAtRest, 9999);
    pmanager->SetProcessOrdering(theParallelWorldProcess, idxAlongStep, 1);
    pmanager->SetProcessOrdering(theParallelWorldProcess, idxPostStep, 9999);
  }

  std::vector<G4VUserParallelWorld*> parallelWorlds = G4tgbParallelGeomMgr::GetInstance()->GetParallelWorlds();
  for( unsigned int ii = 0; ii < parallelWorlds.size(); ii++ ) {
    theParallelWorldProcess->SetParallelWorld( parallelWorlds[ii]->GetName() );
    /*    G4String pwName = parallelWorlds[ii]->GetName();
    G4int index = G4UIcommand::ConvertToInt( pwName.substr(
		   pwName.rfind("_", pwName.length())+1, pwName.length() ).c_str() );
    G4cout << " RegisterParallelWorld " << pwName << " index " << index << " " << G4endl;
    G4tgbParallelGeomMgr::GetInstance()->BuildPhysicsProcess(parallelWorlds[ii]->GetName(), index );
    */
  }

}
