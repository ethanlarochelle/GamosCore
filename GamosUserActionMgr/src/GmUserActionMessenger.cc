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
#include "GmUserActionMessenger.hh"
#include "GmUserActionMgr.hh"

#include "G4UImanager.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

GmUserActionMessenger::GmUserActionMessenger(GmUserActionMgr* myua): 
  myAction(myua) 
{
  listCmd = new G4UIcmdWithoutParameter("/gamos/userAction/dumpList",this);
  listCmd->SetGuidance("Dump the list of UserActions saying if they are enabled or Disabled");
  listCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  listONCmd = new G4UIcmdWithoutParameter("/gamos/userAction/dumpListON",this);
  listONCmd->SetGuidance("Dump the list of ENABLED UserActions");
  listONCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  listOFFCmd = new G4UIcmdWithoutParameter("/gamos/userAction/dumpListOFF",this);
  listOFFCmd->SetGuidance("Dump the list of DISABLED UserActions");
  listOFFCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  EnableCmd = new GmUIcmdWithAString("/gamos/userAction/Enable",this);
  EnableCmd->SetGuidance("Enable a User Action by name");
  EnableCmd->SetParameterName("choice",true);
  EnableCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  DisableCmd = new GmUIcmdWithAString("/gamos/userAction/Disable",this);
  DisableCmd->SetGuidance("Disable a User Action by name");
  DisableCmd->SetParameterName("choice",true);
  DisableCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

GmUserActionMessenger::~GmUserActionMessenger() 
{
  if (listCmd) delete listCmd;
  if (listONCmd) delete listONCmd;
  if (listOFFCmd) delete listOFFCmd;
  if (EnableCmd) delete EnableCmd;
  if (DisableCmd) delete DisableCmd;
}

void GmUserActionMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{ 
  if (command == listCmd) {
    myAction->DumpUserActionList( G4cout, 0 );
  }else if (command == listONCmd) {
    myAction->DumpUserActionList( G4cout, 1 );
  }else if (command == listOFFCmd) {
    myAction->DumpUserActionList( G4cout, -1 );
  }else if (command == EnableCmd) {
    myAction->EnableUserAction(newValues);
  }else if (command == DisableCmd) {
    myAction->DisableUserAction(newValues);
  }
  return;
}
