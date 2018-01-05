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
#include "GmEMStandardPhysicsMessenger.hh"

#include "GmEMStandardPhysics.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmEMStandardPhysicsMessenger::GmEMStandardPhysicsMessenger(GmEMStandardPhysics* pPhys)
:pPhysicsList(pPhys)
{
  physDir = new G4UIdirectory("/gamos/EMStandardPhysics/");
  physDir->SetGuidance("physics list commands");

  pListCmd = new G4UIcmdWithAString("/gamos/EMStandardPhysics/replacePhysics",this);  
  pListCmd->SetGuidance("Add modula physics list.");
  pListCmd->SetParameterName("PList",false);
  pListCmd->AvailableForStates(G4State_PreInit);
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmEMStandardPhysicsMessenger::~GmEMStandardPhysicsMessenger()
{
  delete pListCmd;
  delete physDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmEMStandardPhysicsMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  if( command == pListCmd )
   { 
     pPhysicsList->ReplacePhysicsList(newValue);
   }
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
