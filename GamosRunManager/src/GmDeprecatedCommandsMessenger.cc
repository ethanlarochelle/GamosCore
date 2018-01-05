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
#include "GmDeprecatedCommandsMessenger.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4UImanager.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"

//---------------------------------------------------------------------
GmDeprecatedCommandsMessenger::GmDeprecatedCommandsMessenger() 
{
  G4String fileName = "deprecatedCommands.lis";
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  fileName = GmGenUtils::FileInPath( path, fileName );

  GmFileIn fin = GmFileIn::GetInstance(fileName);
  std::vector<G4String> wl;
  for( ;; ){
    if( ! fin.GetWordsInLine(wl) ) break;
    G4String msg = "";
    if( wl.size() == 3 ) {
      msg = "Deprecated command, use instead " + wl[1];
    } else if( wl.size() == 2 ) { 
      msg = "Deprecated command";
    }
    G4UIcommand* cmd = 0;
    if( wl[wl.size()-1] == "GmUIcmdWithAString" ){
      cmd = new GmUIcmdWithAString(wl[0],this);
    } else if( wl[wl.size()-1] == "G4UIcmdWithoutParameter" ){
      cmd = new G4UIcmdWithoutParameter(wl[0],this);
    } else if( wl[wl.size()-1] == "G4UIcmdWithAnInteger" ){
      cmd = new G4UIcmdWithAnInteger(wl[0],this);
    } else {
      G4Exception("GmDeprecatedCommandsMessenger::GmDeprecatedCommandsMessenger",
		  "Error",
		  FatalException,
		  G4String("Command type not found " + wl[wl.size()-1]).c_str());
    }
    
    cmd->SetGuidance("Deprecated command");
    cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    theCommands[cmd] = msg;
  }
}

//---------------------------------------------------------------------
GmDeprecatedCommandsMessenger::~GmDeprecatedCommandsMessenger() 
{
  std::map<G4UIcommand*,G4String>::const_iterator ite;
  for( ite = theCommands.begin(); ite != theCommands.end(); ite++ ){
    delete (*ite).first;
  }
}

//---------------------------------------------------------------------
void GmDeprecatedCommandsMessenger::SetNewValue(G4UIcommand * command, G4String ) 
{ 
  std::map<G4UIcommand*,G4String>::const_iterator ite = theCommands.find(command);
  G4Exception("GmDeprecatedCommandsMessenger",
	      "Non existent command",
	      FatalException,
	      (*ite).second.c_str());

}
