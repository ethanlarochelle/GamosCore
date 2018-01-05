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
#include "GmRunMessenger.hh"
#include "GmRunManager.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4UImanager.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/MTwistEngine.h"
#include "CLHEP/Random/RanecuEngine.h"
#include "CLHEP/Random/Ranlux64Engine.h"
#include "CLHEP/Random/DualRand.h"
#include "CLHEP/Random/RanluxEngine.h"
#include "CLHEP/Random/RanshiEngine.h"
#include "CLHEP/Random/JamesRandom.h"

#include <sstream>

//---------------------------------------------------------------------
GmRunMessenger::GmRunMessenger(GmRunManager* runMgr) :
  theRunManager(runMgr)
{
  randomSetSeedsCmd = new GmUIcmdWithAString("/gamos/random/setSeeds",this);
  randomSetSeedsCmd->SetGuidance("Set the global random seeds");
  randomSetSeedsCmd->SetParameterName("choice",true);
  randomSetSeedsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  randomRestoreSeedsCmd = new GmUIcmdWithAString("/gamos/random/restoreSeeds",this);
  randomRestoreSeedsCmd->SetGuidance("Restore the global random seeds from a file");
  randomRestoreSeedsCmd->SetParameterName("choice",true);
  randomRestoreSeedsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  randomEngineCmd = new GmUIcmdWithAString("/gamos/random/setEngine",this);
  randomEngineCmd->SetGuidance("Set the global random engine");
  randomEngineCmd->SetParameterName("choice",true);
  randomEngineCmd->AvailableForStates(G4State_PreInit);

}

//---------------------------------------------------------------------
GmRunMessenger::~GmRunMessenger() 
{
  if (randomSetSeedsCmd) delete randomSetSeedsCmd;
  if (randomRestoreSeedsCmd) delete randomRestoreSeedsCmd;
  if (randomEngineCmd) delete randomEngineCmd;
  // CLHEP::HepRandom hran;  
  //  float l = hran.flat();
  //  G4cout << "GamosRandom: Very last random number is " << l << "\n" << G4endl;
}

//---------------------------------------------------------------------
void GmRunMessenger::SetNewValue(G4UIcommand * command, G4String newValues)    
{ 
  if (command == randomSetSeedsCmd) {
    G4String indexStr;
    G4String nTriesStr;
    
    std::istringstream is((char*)newValues.c_str());
    is >> indexStr >> nTriesStr;
    
    G4int index = GmGenUtils::GetInteger(indexStr);
    G4int nTries = GmGenUtils::GetInteger(nTriesStr);

    //    G4cout << " GamosRandom indexStr " << indexStr << " nTriesStr " << nTriesStr << " index " << index << " nTries " << nTries << G4endl;
    theRunManager->SetRandomSeeds( index, nTries );

  } else if (command == randomRestoreSeedsCmd) {
    G4String indexStr;
    G4String nTriesStr;
    
    std::istringstream is((char*)newValues.c_str());
    is >> indexStr >> nTriesStr;
    
    G4int index = GmGenUtils::GetInteger(indexStr);
    G4int nTries = GmGenUtils::GetInteger(nTriesStr);

    //    G4cout << " GamosRandom indexStr " << indexStr << " nTriesStr " << nTriesStr << " index " << index << " nTries " << nTries << G4endl;
    theRunManager->RestoreRandomSeeds( index, nTries );

  }else if (command == randomEngineCmd) {
    if( newValues == "DualRand" ){  
      CLHEP::HepRandom::setTheEngine(new CLHEP::DualRand);
    } else if( newValues == "HepJamesRandom" ){  
      CLHEP::HepRandom::setTheEngine(new CLHEP::HepJamesRandom);
    } else if( newValues == "MTwistEngine" ){  
      CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
    } else if( newValues == "RanecuEngine" ){  
      CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    } else if( newValues == "RanluxEngine" ){  
      CLHEP::HepRandom::setTheEngine(new CLHEP::RanluxEngine);
    } else if( newValues == "Ranlux64Engine" ){  
      CLHEP::HepRandom::setTheEngine(new CLHEP::Ranlux64Engine);
    } else if( newValues == "RanshiEngine" ){  
      CLHEP::HepRandom::setTheEngine(new CLHEP::RanshiEngine);
    } else {
      G4Exception("GmRunMessenger::SetNewValue",
		  "Engine does not exists",
		  FatalErrorInArgument,
		  G4String("Engine= " + newValues + " Available engines: DualRand HepJamesRandom MTwistEngineRanecuEngine RanluxEngine Ranlux64Engine RanshiEngine").c_str());
    }
    
  }
    
}

  
