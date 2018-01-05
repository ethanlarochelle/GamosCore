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
#include "GmVBiasingOperator.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTable.hh"
#include "G4BiasingProcessInterface.hh"

//-----------------------------------------------------------------
GmVBiasingOperator::GmVBiasingOperator(G4String name)
  : G4VBiasingOperator(name)
{
}

//-----------------------------------------------------------------
GmVBiasingOperator::~GmVBiasingOperator()
{
}

//-----------------------------------------------------------------
void GmVBiasingOperator::AddParticleProcess( G4String partName, G4String procName )
{
  theParticleProcessNames.insert(mmss::value_type(partName,procName));
  
}

//-----------------------------------------------------------------
void GmVBiasingOperator::ConvertParticleProcessToPointers()
{

  G4ParticleTable* myParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* myParticleIterator = myParticleTable->GetIterator();
  myParticleIterator->reset();

  while( (*myParticleIterator)() ) {
    G4ParticleDefinition*     particle = myParticleIterator->value();
    G4String              particleName = particle->GetParticleName();
    G4ProcessManager*         pmanager = particle->GetProcessManager();

    std::vector<mmss::const_iterator> partItes;
    for( mmss::const_iterator ite = theParticleProcessNames.begin(); ite != theParticleProcessNames.end(); ite++ ) {
      if( GmGenUtils::AreWordsEquivalent( (*ite).first, particleName ) ) {
	partItes.push_back( ite );
      }
    }
    if( partItes.size() != 0 ) {
      G4ProcessVector* vprocess = pmanager->GetProcessList();
      for (G4int ip = 0 ; ip < vprocess->size() ; ip++) {
	G4String processName = (*vprocess)[ip]->GetProcessName();
	for( size_t ii = 0; ii < partItes.size(); ii++ ) {
	  //	  for( mmss::const_iterator ite = pite.first; ite != pite.second; ite++ ) {	  
	  G4String addedName = "biasWrapper("+(*(partItes[ii])).second+")";	   
	  if( GmGenUtils::AreWordsEquivalent( addedName, processName ) ) {
	    theParticleProcesses.insert(mmpp::value_type( particle, (*vprocess)[ip] ));
	    break;
	  }
	}
      }

    }
    
  }

  for( mmpp::const_iterator itep = theParticleProcesses.begin(); itep != theParticleProcesses.end(); itep++ ) {
  }

  if( theParticleProcesses.size() == 0 ) {
    G4Exception("GmVBiasingOperator::ConvertParticleProcessToPointers()",
		"VBP001",
		FatalException,
		("No particle process defined for operator: " + GetName()).c_str());
  }
}

//-----------------------------------------------------------------
G4bool GmVBiasingOperator::CheckParticleProcess( G4ParticleDefinition* particle, const G4BiasingProcessInterface* process)
{
  std::pair<mmpp::const_iterator, mmpp::const_iterator> pite = theParticleProcesses.equal_range(particle);
  //  G4cout << GetName() << "  GmVBiasingOperator::CheckParticleProcess( particle " << particle->GetParticleName() << " N " << std::distance(pite.first, pite.second ) << G4endl; //GDEB
  if( pite.first != pite.second ) {
    for(mmpp::const_iterator ite = pite.first; ite != pite.second; ite++ ) {
      //      G4cout << "  GmVBiasingOperator::CheckParticleProcess( process " << process->GetProcessName() << " <> " <<  (*ite).second->GetProcessName() << G4endl; //GDEB
  if( process == (*ite).second ) {
	return true;
      }
    }
    return false;
  } else {
    return false;
  }
   
}
