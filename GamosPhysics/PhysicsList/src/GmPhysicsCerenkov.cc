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
#include "GmPhysicsCerenkov.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4Cerenkov.hh"
#include "G4OpticalPhoton.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"


GmPhysicsCerenkov::GmPhysicsCerenkov()
{ }

GmPhysicsCerenkov::~GmPhysicsCerenkov()
{ }

void GmPhysicsCerenkov::ConstructParticle()
{
  G4OpticalPhoton::OpticalPhotonDefinition();
}

void GmPhysicsCerenkov::ConstructProcess()
{
  G4Cerenkov*   theCerenkovProcess = new G4Cerenkov();
  G4int MaxNumPhotons = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmPhysicsCerenkov:MaxNumPhotonsPerStep",100));
  theCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumPhotons);
  
  G4bool bTrackSeco1st = G4bool( GmParameterMgr::GetInstance()->GetNumericValue("GmPhysicsCerenkov:TrackSecondariesFirst",1));
  theCerenkovProcess->SetTrackSecondariesFirst(bTrackSeco1st);

  G4double MaxBetaChange = GmParameterMgr::GetInstance()->GetNumericValue("GmPhysicsCerenkov:MaxBetaChange",0.);
  theCerenkovProcess->SetMaxBetaChangePerStep(MaxBetaChange);

  G4ParticleTable::G4PTblDicIterator* theParticleIterator = G4ParticleTable::GetParticleTable()->GetIterator();  
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theCerenkovProcess->IsApplicable(*particle)) {
      if( GmG4Utils::CheckProcessExists( pmanager, "Cerenkov" )) break;
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrderingToLast(theCerenkovProcess,idxPostStep);
    }
  }
}

