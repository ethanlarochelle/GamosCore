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
#include "GmPrintXSUA.hh"
#include "G4VEmProcess.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessVector.hh"
#include "G4ProcessManager.hh"

//------------------------------------------------------------------
GmPrintXSUA::GmPrintXSUA()
{
}

//------------------------------------------------------------------
GmPrintXSUA::~GmPrintXSUA()
{
}

//------------------------------------------------------------------
void GmPrintXSUA::BeginOfRunAction( const G4Run* )
{
  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4ProcessVector* procVector = pmanager->GetProcessList();
    for( G4int ii = 0; ii < procVector->size(); ii++ ) {
      G4VProcess* proc = (*procVector)[ii];
      G4VEmProcess* procEM = dynamic_cast<G4VEmProcess*>(proc);
      if( procEM != 0 ) {
	G4cout << " GmPrintXSUA Process EM found " << procEM->GetProcessName() << " for " << particle->GetParticleName() << G4endl; // GDEB
	for( G4int im = 1; ; im++) {
	  G4VEmModel* modelEM = procEM->EmModel(im);
	  if( modelEM == 0 ) {
	    G4cout << im << " GmPrintXSUA Model EM NOT found " << G4endl; // GDEB
	    break;
	  } else {
	    G4cout << im << " GmPrintXSUA Model EM found " << modelEM->GetName() << " for " << procEM->GetProcessName() << " for " << particle->GetParticleName() << G4endl; // GDEB
	     std::vector<G4EmElementSelector*>* elemSelec = modelEM->GetElementSelectors();
	     if( elemSelec ) {
	       for( size_t ies = 0; ies < elemSelec->size(); ies++ ) {
		 (*elemSelec)[ies]->Dump();
	       }
	     }

	  }
	}
      }
    }

  }

}
