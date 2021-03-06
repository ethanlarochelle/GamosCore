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
#include "GmBiasingMgr.hh"
#include "GmBOptrForceCollision.hh"
#include "GmBOptrBremsSplitting.hh"
#include "GmBOptrDirBremsSplitting.hh"
#include "GmBOptrEWBS.hh"
#include "GmBOptrChangeCrossSection.hh"

#include "GmGenericBiasingPhysics.hh"
#include "G4BiasingHelper.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTable.hh"
#include "G4VModularPhysicsList.hh"
#include "G4RunManager.hh"

#include "GmBiasingVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "Reflex/PluginService.h"

//----------------------------------------------------------------------
GmBiasingMgr* GmBiasingMgr::theInstance = 0;

//-----------------------------------------------------------------------
GmBiasingMgr::GmBiasingMgr()
{	
  theBiasingPhysics = 0;
}

//----------------------------------------------------------------------
GmBiasingMgr* GmBiasingMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmBiasingMgr;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
GmBiasingMgr::~GmBiasingMgr()
{
}

//-----------------------------------------------------------------------
void GmBiasingMgr::CreateOperator(std::vector<G4String> params )
{
  if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmBiasingMgr::AddOperator",
		"BM001",
		FatalErrorInArgument,
		G4String("At least three arguments must be supplied in command '/gamos/physics/biasing/createOperator' : NAME TYPE.  They are: "+parastr).c_str());
  }
  
  G4String operName = params[0];
  std::map<G4String,GmVBiasingOperator*>::const_iterator ite = theOperators.find(operName);
  if( ite != theOperators.end() ) {
    G4Exception("GmBiasingMgr::AddOperator",
		"BM002",
		JustWarning,
		G4String("Operator already exists " + operName).c_str());
  }
  
  G4String operType = params[1];

  GmVBiasingOperator* theBOper = Reflex::PluginService::Create<GmVBiasingOperator*>("GmBOptr"+operType,operName);
    if( theBOper == 0 ) {
      G4Exception("GmBiasingMgr::CreateOperator",
	 	  "BOper1",
		  FatalErrorInArgument,
		  ("Biasing operator  " + operType + " does not exist").c_str());
    }

   theOperators[operName] = theBOper;
   /*  if( operType == "ForceCollision" ) {
    theOperators[operName] = new GmBOptrForceCollision(operName);
  } else if( operType == "BremsSplitting" ) {
    theOperators[operName] = new GmBOptrBremsSplitting(operName);
  } else if( operType == "DirBremsSplitting" ) {
    theOperators[operName] = new GmBOptrDirBremsSplitting(operName);
  } else if( operType == "ChangeCrossSection" ) {
    theOperators[operName] = new GmBOptrChangeCrossSection(operName);
  } else {
    G4Exception("GmBiasingMgr::AddOperator",
		"BM003",
		FatalErrorInArgument,
		G4String("Operator type must be: ForceCollision, BremsSplitting or ChangeCrossSection, while it is "+operType).c_str());
		}*/

  //prepare physics
  if( !theBiasingPhysics ) {
    theBiasingPhysics = new GmGenericBiasingPhysics();
    G4VUserPhysicsList* physicsList = const_cast<G4VUserPhysicsList*>(G4RunManager::GetRunManager()->GetUserPhysicsList());
    if( !physicsList ) {
      G4Exception("GmBiasingMgr::CreateOperator",
		  "BM004",
		  FatalErrorInArgument,
		  "No physics list created yet: '/gamos/physics/biasing/createOperator'");
    }
    G4VModularPhysicsList* modularPhysicsList = dynamic_cast<G4VModularPhysicsList*>(physicsList);
    if( !modularPhysicsList ) {
      G4Exception("GmBiasingMgr::CreateOperator",
		  "BM004",
		  FatalErrorInArgument,
		  "Physics list is not modular, try another physics list");
    }
    modularPhysicsList->RegisterPhysics(theBiasingPhysics);
  }
  
}

//-----------------------------------------------------------------------
void GmBiasingMgr::AssocOper2LogVol(std::vector<G4String> params )
{
  if( params.size() < 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmBiasingMgr::AssocOper2LogVol",
		"BM004",
		FatalErrorInArgument,
		G4String("At least two arguments must be supplied in command '/gamos/physics/biasing/associateProcess' : LOGICAL_VOLUE_1 LOGICAL_VOLUME_2 ... LOGICAL_VOLUME_N OPERATOR_NAME.  They are: "+parastr).c_str());
  }
  
  G4String operName = params[params.size()-1];
  std::map<G4String,GmVBiasingOperator*>::const_iterator ite = theOperators.find(operName);
  if( ite == theOperators.end() ) {
    G4Exception("GmBiasingMgr::AddOperator",
		"BM002",
		FatalException,
		G4String("Operator does not exist " + operName).c_str());
  }
  GmVBiasingOperator* biasOper = (*ite).second;
  
  std::vector<G4LogicalVolume*> logvols;
  for( unsigned int ii = 0; ii < params.size()-1; ii++ ){
    std::vector<G4LogicalVolume*> logvols1 = GmGeometryUtils::GetInstance()->GetLogicalVolumes( params[ii], FALSE );
    
    if( logvols1.size() == 0 ) {
      GmGeometryUtils::GetInstance()->DumpG4LVTree();
      G4Exception("GmBiasingMgr::AssocOper2LogVol",
		  " Cannot associate sensitive detector to a logical volume that does not exist ",
		  FatalErrorInArgument,
		  ("Command issued:  /gamos/biasing/assocOper2LogVol " +  params[ii]).c_str() );
    }
    for( size_t il = 0; il < logvols1.size(); il++ ) {
      logvols.push_back( logvols1[il] );
    }
  }
  
  for( unsigned int jj = 0; jj < logvols.size(); jj++ ){
    G4LogicalVolume* logvol = logvols[jj];
    biasOper->AttachTo(logvol);
  }
    
}

//-----------------------------------------------------------------------
void GmBiasingMgr::AddProcesses2Oper(std::vector<G4String> params)
{
  if( params.size() < 1 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmBiasingMgr::AddProcesses",
		"BM004",
		FatalErrorInArgument,
		G4String("At least two arguments must be supplied in command '/gamos/physics/biasing/addProcesses' : PROCESS_1 PROCESS_2 ... PROCESS_N.  They are: "+parastr).c_str());
  }


  // -- bias setup per individual particle name:
  G4ParticleTable* myParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* myParticleIterator = myParticleTable->GetIterator();
  myParticleIterator->reset();

  while( (*myParticleIterator)() ) {
    G4ParticleDefinition*     particle = myParticleIterator->value();
    G4String              particleName = particle->GetParticleName();
    G4ProcessManager*         pmanager = particle->GetProcessManager();
    
    G4ProcessVector* vprocess = pmanager->GetProcessList();
    std::vector< G4String > processToBiasNames;
    for (G4int ip = 0 ; ip < vprocess->size() ; ip++) {
      for( size_t ii = 0; ii < params.size()-1; ii++ ){
	if( GmGenUtils::AreWordsEquivalent( params[ii], (*vprocess)[ip]->GetProcessName() ) ) {
	  processToBiasNames.push_back( (*vprocess)[ip]->GetProcessName() );
	  break;
	}
      }
    }
    if( processToBiasNames.size() != 0 ) {
      theBiasingPhysics->PhysicsBias( particleName, processToBiasNames );
    }
    
  }
  
}

//-----------------------------------------------------------------------
void GmBiasingMgr::AddParticles2Oper(std::vector<G4String> params )
{
  if( params.size() < 1 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmBiasingMgr::AddParticle",
		"BM004",
		FatalErrorInArgument,
		G4String("At least two arguments must be supplied in command '/gamos/physics/biasing/addParticle' : PARTICLE_1 PARTICLE_2 ... PROCESS_N.  They are: "+parastr).c_str());
  }

  // check if NonPhys operator (for the moment all Phys)
  G4bool bOperPhys = true;

  // -- bias setup per individual particle name:
  G4ParticleTable* myParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* myParticleIterator = myParticleTable->GetIterator();
  myParticleIterator->reset();

  while( (*myParticleIterator)() ) {
    G4ParticleDefinition*     particle = myParticleIterator->value();
    G4String              particleName = particle->GetParticleName();
    for( size_t ii = 0; ii < params.size()-1; ii++ ){
      if( GmGenUtils::AreWordsEquivalent( params[ii], particleName ) ) {
	if( bOperPhys ) {
	  theBiasingPhysics->PhysicsBias( particleName );
	} else {
	  //	  theBiasingPhysics->NonPhysicsBias( particleName );
	  theBiasingPhysics->Bias( particleName );
	}
	break;
      }
    }
  }
}

//-----------------------------------------------------------------------
void GmBiasingMgr::AddParticleProcesses2Oper(std::vector<G4String> params )
{
  if( params.size() < 3 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmBiasingMgr::AddParticleProcesses",
		"BM004",
		FatalErrorInArgument,
		G4String("At least two arguments must be supplied in command '/gamos/physics/biasing/addParticleProcesses' : PARTICLE PROCESS_1 PROCESS_2 ... PROCESS_N OPERATOR_NAME.  They are: "+parastr).c_str());
  }

  G4String operName = params[params.size()-1];
  std::map<G4String,GmVBiasingOperator*>::const_iterator ite = theOperators.find(operName);
  //  G4bool bOperPhys = false;
  GmVBiasingOperator* bOperator = 0;
  if( ite == theOperators.end() ) {
    G4Exception("GmBiasingMgr::AssocPartProc2Oper",
		"BM005",
		JustWarning,
		G4String("Operator does not exist " + operName).c_str());
  } else {
    // check if NonPhys operator (for the moment all Phys)
    bOperator = (*ite).second;
    //    bOperPhys = bOperator->IsPhysical();
  }
  
  std::vector< G4String > processToBiasNames;
  for( size_t ii = 1; ii < params.size()-1; ii++ ) {
    processToBiasNames.push_back(params[ii]);
    bOperator->AddParticleProcess( params[0], params[ii] );
  }
  theBiasingPhysics->PhysicsBias( params[0], processToBiasNames );
  
  /*
  
  // -- bias setup per individual particle name:
  G4ParticleTable* myParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* myParticleIterator = myParticleTable->GetIterator();
  myParticleIterator->reset();

  while( (*myParticleIterator)() ) {
    G4ParticleDefinition*     particle = myParticleIterator->value();
    G4String              particleName = particle->GetParticleName();
    G4ProcessManager*         pmanager = particle->GetProcessManager();
    G4bool bPartOK = false;
    if( GmGenUtils::AreWordsEquivalent( params[0], particleName ) ) {
      bPartOK = true;
      break;
    }

    if( bPartOK ) {
      G4ProcessVector* vprocess = pmanager->GetProcessList();
      std::vector< G4String > processToBiasNames;
      for (G4int ip = 0 ; ip < vprocess->size() ; ip++) {
	if( GmGenUtils::AreWordsEquivalent( params[1], (*vprocess)[ip]->GetProcessName() ) ) {
	  processToBiasNames.push_back( (*vprocess)[ip]->GetProcessName() );
	}
      }   
      if( processToBiasNames.size() != 0 ) {
	theBiasingPhysics->PhysicsBias( particleName, processToBiasNames );
      }

    }
    
  }
  */
}
