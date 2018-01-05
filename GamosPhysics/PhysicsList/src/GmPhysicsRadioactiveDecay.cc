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
#include "GmPhysicsRadioactiveDecay.hh"
#include "G4ProcessManager.hh"

#include "G4RadioactiveDecay.hh"
#include "G4GenericIon.hh"
#include "globals.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

//----------------------------------------------------------------------
GmPhysicsRadioactiveDecay::GmPhysicsRadioactiveDecay()
: theRadioactiveDecay(0)
{}

//----------------------------------------------------------------------
GmPhysicsRadioactiveDecay::~GmPhysicsRadioactiveDecay()
{
  delete theRadioactiveDecay;
}
 
//----------------------------------------------------------------------
void GmPhysicsRadioactiveDecay::ConstructProcess()
{
  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator->reset();
  if( !theRadioactiveDecay ) theRadioactiveDecay = new G4RadioactiveDecay();

  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    if(particle->GetParticleName() != "GenericIon" && 
       ( particle->GetParticleType() != "nucleus" || particle->GetPDGLifeTime() < 0. )) continue;
    // THIS HAS A BUG IN GEANT4   if( ! theRadioactiveDecay->IsApplicable( *particle ) ) continue;

    G4ProcessManager* pmanager = particle->GetProcessManager();

    if( GmG4Utils::CheckProcessExists( pmanager, "RadioactiveDecay" )) break;
    //  if not Found
    //      G4cout << " GmPhysicsRadioactiveDecay::ConstructProcess  "<< particle->GetParticleName()  << G4endl;
    //  G4GenericIon* ion = G4GenericIon::GenericIon();
    //  G4ProcessManager* pmanager = ion->GetProcessManager();
    pmanager->AddProcess(theRadioactiveDecay);
    pmanager->SetProcessOrdering(theRadioactiveDecay, idxPostStep);
    pmanager->SetProcessOrdering(theRadioactiveDecay, idxAtRest);
  }
}


