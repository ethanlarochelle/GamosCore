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
#include "G4ProcessAttribute.hh"
#include "G4ProcessManager.hh"

#include "GmBiasingMessenger.hh"
#include "GmBiasingMgr.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

//-----------------------------------------------------------------------
GmBiasingMessenger::GmBiasingMessenger() 
{
  theManager = GmBiasingMgr::GetInstance();
  
  operatorCmd = new GmUIcmdWithAString("/gamos/physics/biasing/createOperator",this);
  operatorCmd->SetGuidance("Creates a Geant4 biasing operator");
  operatorCmd->SetParameterName("choice",true);
  operatorCmd->AvailableForStates(G4State_PreInit);

  oper2LogVolCmd = new GmUIcmdWithAString("/gamos/physics/biasing/associateOper2LogVol",this);
  oper2LogVolCmd->SetGuidance("Associate a logical volume to a biasing operator");
  oper2LogVolCmd->SetParameterName("choice",true);
  oper2LogVolCmd->AvailableForStates(G4State_Idle);

  addProcCmd = new GmUIcmdWithAString("/gamos/physics/biasing/addProcesses2Oper",this);
  addProcCmd->SetGuidance("Add a process to be biased to an operator");
  addProcCmd->SetParameterName("choice",true);
  addProcCmd->AvailableForStates(G4State_PreInit);

  addPartCmd = new GmUIcmdWithAString("/gamos/physics/biasing/addParticles2Oper",this);
  addPartCmd->SetGuidance("Add a particle to be biased (all processes plus NonPhysicsBias) to an operator");
  addPartCmd->SetParameterName("choice",true);
  addPartCmd->AvailableForStates(G4State_PreInit);

  addPartProcCmd = new GmUIcmdWithAString("/gamos/physics/biasing/addParticleProcesses2Oper",this);
  addPartProcCmd->SetGuidance("Add a process of a particle to be biased to an operator");
  addPartProcCmd->SetParameterName("choice",true);
  addPartProcCmd->AvailableForStates(G4State_PreInit);

}

//-----------------------------------------------------------------------
GmBiasingMessenger::~GmBiasingMessenger() 
{
  if (operatorCmd) delete operatorCmd;
  if (oper2LogVolCmd) delete oper2LogVolCmd;
  if (addProcCmd) delete addProcCmd;
  if (addPartCmd) delete addPartCmd;
  if (addPartProcCmd) delete addPartProcCmd;
}

//-----------------------------------------------------------------------
void GmBiasingMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{ 
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
  
  if (command == operatorCmd) {
    theManager->CreateOperator( wl );
  } else if (command == oper2LogVolCmd) {
    theManager->AssocOper2LogVol( wl );
  } else if (command == addProcCmd) {
    theManager->AddProcesses2Oper( wl );
  } else if (command == addPartCmd) {
    theManager->AddParticles2Oper( wl );
  } else if (command == addPartProcCmd) {
    theManager->AddParticleProcesses2Oper( wl );
  }
}

