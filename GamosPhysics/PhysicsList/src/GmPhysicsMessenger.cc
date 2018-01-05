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
#include "GmPhysicsMessenger.hh"
#include "GmEMPhysics.hh"
#include "G4UIdirectory.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

// ----------------------------------------------------------------------------
GmPhysicsMessenger::GmPhysicsMessenger(GmEMPhysics * physList)
:theGmPhysicsList(physList)
{  
 listDir = new G4UIdirectory("/gamos/GmPhysics/");

 theGmPhysicsListCmd = new GmUIcmdWithAString("/gamos/GmPhysics/replacePhysics",this);  
 theGmPhysicsListCmd->SetGuidance("Replace a set of process models of a particle");
 theGmPhysicsListCmd->SetParameterName("physList",false);
 theGmPhysicsListCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

 theGmPhysicsListCmdOld = new GmUIcmdWithAString("/gamos/GmPhysics/addPhysics",this);  
 theGmPhysicsListCmdOld->SetGuidance("Replace a set of process models of a particle");
 theGmPhysicsListCmdOld->SetParameterName("physList",false);
 theGmPhysicsListCmdOld->AvailableForStates(G4State_PreInit,G4State_Idle);

}

// ----------------------------------------------------------------------------
GmPhysicsMessenger::~GmPhysicsMessenger()
{
  delete theGmPhysicsListCmd;
  delete theGmPhysicsListCmdOld;
  delete listDir;
}

// ----------------------------------------------------------------------------
void GmPhysicsMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if (command == theGmPhysicsListCmd){
    G4bool bFound = theGmPhysicsList->ReplacePhysicsList(newValue);
    if( !bFound ) {
      G4Exception("GmPhysicsMessenger::SetNewValue","ERROR in /gamos/GmPhysics/replacePhysics command, process model set",FatalErrorInArgument,newValue);
    }
  }else if (command == theGmPhysicsListCmdOld){
    G4Exception("GmPhysicsMessenger::SetNewValue",
		"WARNING",
		JustWarning,
		"You are using a deprecated command /gamos/GmPhysics/addPhysics , please use /gamos/GmPhysics/replacePhysics");
    G4bool bFound = theGmPhysicsList->ReplacePhysicsList(newValue);
    if( !bFound ) {
      G4Exception("GmPhysicsMessenger::SetNewValue","ERROR in /gamos/GmPhysics/replacePhysics command, process model set",FatalErrorInArgument,newValue);
    }
  } 
}
