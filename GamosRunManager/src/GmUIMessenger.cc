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
#include "GmUIMessenger.hh"
#include "GmUIterminal.hh"

#include "G4UImanager.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
GmUIMessenger::GmUIMessenger(GmUIterminal* ses) : theCommand(ses)
{
  setCoutCmd = new GmUIcmdWithAString("/gamos/log/setCoutFile",this);
  setCoutCmd->SetGuidance("Select the file where the standard output will be copied ");
  setCoutCmd->SetParameterName("choice",true);
  setCoutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  setCerrCmd = new GmUIcmdWithAString("/gamos/log/setCerrFile",this);
  setCerrCmd->SetGuidance("Select the file where the standard error will be copied ");
  setCerrCmd->SetParameterName("choice",true);
  setCerrCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  writeLogCmd = new G4UIcmdWithABool("/gamos/log/writeFiles",this);
  writeLogCmd->SetGuidance("Select if the log files are written or not ");
  writeLogCmd->SetParameterName("choice",true);
  writeLogCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
GmUIMessenger::~GmUIMessenger() 
{
  if (setCoutCmd) delete setCoutCmd;
  if (setCerrCmd) delete setCerrCmd;
  if (writeLogCmd) delete writeLogCmd;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void GmUIMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{ 
  if (command == setCoutCmd) {
    theCommand->SetCoutFile( newValues );
  }
  else if (command == setCerrCmd) {
    theCommand->SetCerrFile( newValues );
  }
  else if (command == writeLogCmd) {
    theCommand->SetWriteLog( writeLogCmd->GetNewBoolValue(newValues) );
  }

}
