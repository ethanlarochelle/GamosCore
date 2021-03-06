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
//#include "GamosCore/GamosGeometry/include/GmGeometryFromText.hh"

#include "GmRunManager.hh"
#include "GmRunMessenger.hh"
#include "GmFactoriesMessenger.hh"
#include "GmDeprecatedCommandsMessenger.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseMessenger.hh"
#include "GamosCore/GamosSD/include/GmSDMessenger.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMessenger.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryMessenger.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringMessenger.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmExtraPhysicsMessenger.hh"
#include "GamosCore/GamosPhysics/Cuts/include/GmUserLimitsMessenger.hh"
#include "GamosCore/GamosPhysics/VarianceReduction/include/GmVarianceReductionMessenger.hh"
#include "GamosCore/GamosPhysics/Biasing/include/GmBiasingMessenger.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringMgr.hh"

#include "MagFieldManager/include/MagneticFieldModelManager.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4VUserPhysicsList.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Run.hh"

#include "Reflex/PluginService.h"

#include "CLHEP/Random/RandomEngine.h"

#include <sstream>

GmRunManager* GmRunManager::theRunManager = 0;
//----------------------------------------------------------------------
GmRunManager* GmRunManager::GetRunManager()
{
  return theRunManager; 
}

//----------------------------------------------------------------------
GmRunManager::GmRunManager()
{
  if(theRunManager) {
    G4Exception("GmRunManager::GmRunManager",
		"Error",
		FatalException,
		"Constructed twice."); 
  }

  theRunManager = this;

  new GmFactoriesMessenger( this );

  new GmDeprecatedCommandsMessenger;

  theGeometryName = "";
  thePhysicsListName = "";
  theGeneratorName = "";
  
  InitialiseFactories();

  GmParameterMgr::GetInstance();
  theGmSDMessenger = new GmSDMessenger();
  theGmAnalysisMessenger =  new GmAnalysisMessenger();
  theGmRunMessenger = new GmRunMessenger(this);

  new GmBaseMessenger();

  new GmGeometryMessenger();

  new GmExtraPhysicsMessenger();
  new GmUserLimitsMessenger();
  new GmVarianceReductionMessenger();
  new GmBiasingMessenger();

  new GmScoringMessenger();

  if( G4bool(GmParameterMgr::GetInstance()->GetNumericValue("  MagneticFieldModelManager:On",0)) ) {
    MagneticFieldModelManager::GetInstance();
  }

  bRestoreSeedsUsed = false;

}

//----------------------------------------------------------------------
void GmRunManager::CreateG4RunManager()
{
  theG4RunManager = new G4RunManager;
  theG4RunManager->SetVerboseLevel(1);
}

//----------------------------------------------------------------------
GmRunManager::~GmRunManager()
{
  //  delete theG4RunManager;
  delete theGmRunMessenger;
  GmAnalysisMgr::DeleteInstances();
  delete theGmSDMessenger;
  delete theGmAnalysisMessenger;

  GmParameterMgr::GetInstance()->CheckParametersUsage();

  delete GmUserActionMgr::GetInstance();

  delete GmScoringMgr::GetInstance();

  //--- Check if too many random numbers are used 
  G4int nCalls =  CLHEP::HepRandomEngine::theNCalls;
  if( G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmRunManager:PrintNRandomNumbersUsed",0) ) ){
    G4int nev = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEvent();
    G4cout << "@@@ Number of random numbers used: " << nCalls << " / " << nev << " = " << nCalls/nev << " numbers/event" << G4endl;
  }
  if(bRestoreSeedsUsed && nCalls > theNSeedsInFile ) {
    G4Exception("GmRunManager::~GmRunManager",
		"Warning",
		JustWarning,
		G4String("Too many random numbers used: " + GmGenUtils::itoa(nCalls) + ", more than those in file: " + GmGenUtils::itoa(theNSeedsInFile) + " There is a very small chance of random number repetition ").c_str());
    G4int nev = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEvent();
    G4cout << "Number of random numbers used per event= " << nCalls/nev << G4endl;
  }
  

}

//----------------------------------------------------------------------
void GmRunManager::InitialiseFactories()
{
}


//----------------------------------------------------------------------
void GmRunManager::Initialise()
{
  theG4RunManager->Initialize();
}


//----------------------------------------------------------------------
void GmRunManager::SelectGeometry( const G4String& name )
{  
  if( theGeometryName != "" ) {
    G4cerr << "!!WARNING: selecting detector " << name << " while detector " << theGeometryName << " was previously selected " << G4endl;
  }

  //  Reflex::PluginService::SetDebug(21);  
  G4VUserDetectorConstruction* theGeometry = Reflex::PluginService::Create<G4VUserDetectorConstruction*>(name);
  if( theGeometry == 0 ) {
    G4cerr << "!!ERROR: detector " << name << " does not exist, program will crash if another detector is not selected " << G4endl;
  } else {
    theGeometryName = name;
    theG4RunManager->SetUserInitialization( theGeometry );
  }

}

//----------------------------------------------------------------------
void GmRunManager::SelectPhysicsList( const G4String& name )
{
  if( thePhysicsListName != "" ) {
    G4cerr << "!!WARNING: selecting physics list " << name << " while physics list " << thePhysicsListName << " was previously selected " << G4endl;
  }

  Reflex::PluginService::SetDebug(21);  
   G4VUserPhysicsList* thePhysicsList = Reflex::PluginService::Create<G4VUserPhysicsList*>(name);
  if( thePhysicsList == 0 ) {
    G4cerr << "!!ERROR: physics list " << name << " does not exist, program will crash if another physics list is not selected " << G4endl;
  } else {
    theG4RunManager->SetUserInitialization( thePhysicsList );
    thePhysicsListName = name;
  }

  //---- Register GmScoringUA to print scorers at end of run
  SelectUserAction("GmScoringUA");

}

//----------------------------------------------------------------------
void GmRunManager::SelectGenerator( const G4String& name )
{
  if( theGeneratorName != "" ) {
    G4cerr << "!!WARNING: selecting generator " << name << " while generator " << theGeneratorName << " was previously selected " << G4endl;
  }

  Reflex::PluginService::SetDebug(21);  
  G4VUserPrimaryGeneratorAction* theGenerator = Reflex::PluginService::Create<G4VUserPrimaryGeneratorAction*>(name);
  if( theGenerator == 0 ) {
    G4cerr << "!!ERROR: generator " << name << " does not exist, program will crash if another generator is not selected " << G4endl;
  } else {
    theGeneratorName = name;
    theG4RunManager->SetUserAction( theGenerator );
  }
}

//----------------------------------------------------------------------
void GmRunManager::SelectUserAction( const G4String& values )
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( values );
  G4String name = wl[0];
  //-  G4cout << " GmRunManager::selectUserAction "<< name << G4endl;
  Reflex::PluginService::SetDebug(21);  
  GmUserAction* userAction = Reflex::PluginService::Create<GmUserAction*>(name);
  if( userAction == 0 ) {
    G4Exception("GmRunManager::SelectUserAction",
		"user action does not exist",
		FatalErrorInArgument,
		G4String(" User action " + name).c_str());
  } else {
    userAction->SetName( name );
//    if( UAVerb(infoVerb) ) G4cout << " GmRunManager::SelectUserAction user action added: " << name << G4endl;
    GmUserActionMgr::GetInstance()->CheckIfRepeated( name );
    for( unsigned int ii = 1; ii < wl.size(); ii++ ){
      userAction->SetFilterOrClassifier(wl[ii]);
    }
    userAction->InitialiseHistoNames();
  }
}


//----------------------------------------------------------------------
void GmRunManager::SelectVerbosity( const G4String& name )
{
#ifndef GAMOS_NO_VERBOSE
  //  G4cout << " GmRunManager::selectVerbosity "<< name << G4endl;
#endif
  G4String verbName;
  G4String verbLevel;
  std::istringstream is((char*)name.c_str());
  is >> verbName >> verbLevel;

 G4int val = 4;
 if( GmGenUtils::IsNumber( verbLevel ) ) {
   val = atoi( verbLevel );
 } else {
   if( verbLevel == "silent" ) {
     val = silentVerb;
   } else if( verbLevel == "error" ) {
     val = errorVerb;
   } else if( verbLevel == "warning" ) {
     val = warningVerb;
   } else if( verbLevel == "info" ) {
     val = infoVerb;
      } else if( verbLevel == "debug" ) {
     val = debugVerb;
   } else if( verbLevel == "test" ) {
     val = testVerb;
   } else {
     G4Exception("GmRunManager::SelectVerbosity",
		 "Wrong argument",
		 FatalErrorInArgument,
		 G4String("invalid value = " + verbLevel).c_str());
   }
 }

 GmVVerbosity::SetVerbosityLevel( verbName, val );
}

//---------------------------------------------------------------------
void GmRunManager::SetRandomSeeds( G4int index, G4int nTries )
{
  // init
  if (index==0&&nTries==0) {
    index = 1; // CMSTime().seconds();
    nTries = 1; //getpid() % 50;
  }
  
  index = abs(index);
  //  index = index % 215;
  
  G4cout << "Gamos Random: initializing seeds from table at position " 
		 << index <<  " and throwing " << nTries << "+1 numbers" << G4endl;

  long seeds[3];
  CLHEP::HepRandom::getTheTableSeeds(seeds,index);
  seeds[0] = long(fabs(double(seeds[0]))); 
  seeds[1] = long(fabs(double(seeds[1]))); 
  seeds[2] = long(fabs(double(seeds[2])));

  G4cout << " GamosRandom: sees before nTries " << seeds[0] << " " << seeds[1] << " " << seeds[2] << G4endl;
  seeds[2]=0;

  CLHEP::HepRandom hran; 
  CLHEP::HepRandom::setTheSeeds(seeds);
  //  G4cout << "Gamos Random: after seeds " <<  hran.flat() << G4endl;

  //  int nTriesOrig = nTries;
  float l;
  while(nTries-- >= 0) { 
    l = hran.flat();
  }
  G4cout << "Gamos Random: First random number is " << l << "\n" << G4endl;

}

#include <sstream>
//---------------------------------------------------------------------
void GmRunManager::RestoreRandomSeeds( G4int index, G4int nTries )
{
  bRestoreSeedsUsed = true;
  theNSeedsInFile = GmParameterMgr::GetInstance()->GetNumericValue("GmRunManager:RandomSeedsInFile",1.e11);
  G4String seedFileDir = GmParameterMgr::GetInstance()->GetStringValue("GmRunManager:SeedFileDir","initialSeeds");

  //--- 
  std::ostringstream nseedS;
  nseedS << std::setprecision(2) << theNSeedsInFile;

#ifdef WIN32
  // convert 1e+010 in 1e+10
  G4String nseedSs = nseedS.str().substr(0,3) + nseedS.str().substr(4,2);
  G4String filePrefix = seedFileDir + "\\initialSeeds." + nseedSs + ".";
#else
  G4String filePrefix = seedFileDir + "/initialSeeds." + nseedS.str() + ".";
#endif
  G4String numStr = GmGenUtils::itoa(index);
  if( index < 10 ) {
    numStr = "000"+numStr;
  } else if( index < 100 ) {
    numStr = "00"+numStr;
  } else if( index < 1000 ) {
    numStr = "0"+numStr;
  }
  G4String fileName = filePrefix + numStr;
  
  char* pathc = getenv( "GAMOS_SEARCH_PATH" );
  if( !pathc ) G4Exception("GmRunManager::RestoreRandomSeeds",
                           "Wrong argument",
                           FatalErrorInArgument,
                           "No GAMOS_SEARCH_PATH variable defined, please defin\
e it as in config/gamosconf.sh");
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  fileName = GmGenUtils::FileInPath( path, fileName );
  CLHEP::HepRandom::restoreEngineStatus(fileName);
  //  CLHEP::HepRandom::showEngineStatus();

  G4cout << "Gamos Random: restoring seeds from file " 
		 << fileName <<  " and throwing " << nTries << "+1 numbers" << G4endl;

  CLHEP::HepRandom hran; 
  float l;
  while(nTries-- >= 0) { 
    l = hran.flat();
  }

  G4cout << "Gamos Random: First random number is " << l << "\n" << G4endl;

}
