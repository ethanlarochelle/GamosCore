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
#include "RTPlanMessenger.hh"
#include "RTVPlanSource.hh"
#include "GmGenerator.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4UImanager.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4NucleiProperties.hh"
#include "G4ProcessManager.hh"
#include "GmParticleSource.hh"
#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "G4ProcessManager.hh"

//------------------------------------------------------------------------
RTPlanMessenger::RTPlanMessenger(RTVPlanSource* myua) 
   : myAction(myua) 
{
  AssociatePlanVolumesCmd = new GmUIcmdWithAString("/gamos/generator/RTPlan/associatePlanVolumes",this);
  AssociatePlanVolumesCmd->SetParameterName("NAME PARTICLE_NAME ENERGY",false);
  AssociatePlanVolumesCmd->AvailableForStates(G4State_Idle,G4State_PreInit);

  DefineAcceleratorVolumeCmd = new GmUIcmdWithAString("/gamos/generator/RTPlan/defineAcceleratorVolume",this);
  DefineAcceleratorVolumeCmd->SetParameterName("NAME PARTICLE_NAME ENERGY",false);
  DefineAcceleratorVolumeCmd->AvailableForStates(G4State_Idle,G4State_PreInit);

}

//------------------------------------------------------------------------
RTPlanMessenger::~RTPlanMessenger() 
{
  if (AssociatePlanVolumesCmd) delete AssociatePlanVolumesCmd;
  if (DefineAcceleratorVolumeCmd) delete DefineAcceleratorVolumeCmd;
}


//------------------------------------------------------------------------
void RTPlanMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
  if (command == AssociatePlanVolumesCmd) {
    //    GmGenUtils::CheckNWords(newValues,3,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 3 arguments: name particle_name energy energy_unit"); 
    myAction->AssociatePlanVolumes(wl);
  } else if (command == DefineAcceleratorVolumeCmd) {
    //    GmGenUtils::CheckNWords(newValues,3,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 3 arguments: name particle_name energy energy_unit"); 
    myAction->DefineAcceleratorVolume(wl);
  }

  return;
}

