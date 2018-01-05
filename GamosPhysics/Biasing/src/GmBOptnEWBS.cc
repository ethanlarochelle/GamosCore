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
#include "GmBOptnEWBS.hh"
#include "GmBiasingVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4BiasingProcessInterface.hh"

#include "G4ParticleChangeForLoss.hh"

#include "GmBiasingMgr.hh"
#include "GmBOptnEWBremsstrahlung.hh"
#include "GmBOptnEWAnnihilation.hh"
#include "GmBOptnEWCompton.hh"
#include "GmBOptnEWPairProduction.hh"
#include "GmBOptnEWPhotoElectric.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWBS::GmBOptnEWBS(G4String name)
: GmVEWBiasingOperation(name)
{
  addPartProcCmd = new GmUIcmdWithAString("/gamos/physics/biasing/EWBS/addParticleProcesses2Oper",this);
  addPartProcCmd->SetGuidance("Add a process of a particle to be biased to EWBS  operator");
  addPartProcCmd->SetParameterName("choice",true);
  addPartProcCmd->AvailableForStates(G4State_PreInit);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWBS::~GmBOptnEWBS()
{
  if (addPartProcCmd) delete addPartProcCmd;
}

//-----------------------------------------------------------------------
void GmBOptnEWBS::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{ 
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
  
  if (command == addPartProcCmd) {
    GmBiasingMgr::GetInstance()->AddParticleProcesses2Oper( wl );
  }
  if( wl[1] == "eBrem" ) {
    theOperationNames["biasWrapper("+wl[1]+")"] = new GmBOptnEWBremsstrahlung(wl[2]);
  } else if( wl[1] == "annihil" ) {
    theOperationNames["biasWrapper("+wl[1]+")"] = new GmBOptnEWAnnihilation(wl[2]);
  } else if( wl[1] == "compt" ) {
    theOperationNames["biasWrapper("+wl[1]+")"] = new GmBOptnEWCompton(wl[2]);
  } else if( wl[1] == "conv" ) {
    theOperationNames["biasWrapper("+wl[1]+")"] = new GmBOptnEWPairProduction(wl[2]);
  } else if( wl[1] == "phot" ) {
    theOperationNames["biasWrapper("+wl[1]+")"] = new GmBOptnEWPhotoElectric(wl[2]);
  } else {
    G4Exception("GmBOptnEWBS::SetNewValue",
		"BOEWBS1",
		FatalErrorInArgument,
		("process does not exists: " + wl[1] + " Please contact GAMOS authors").c_str());
  }
}

void GmBOptnEWBS::SetNSplit(G4int splittingFactor)
{
  std::map<G4String,GmVEWBiasingOperation*>::const_iterator ites;
  //  G4cout << " GmBOptnEWBS::SetNSplit " << theOperationNames.size() << G4endl; //GDEB
  for( ites = theOperationNames.begin(); ites != theOperationNames.end(); ites++ ) {
    (*ites).second->SetNSplitAndInverse( splittingFactor );
  }
}

G4VParticleChange*  
GmBOptnEWBS::
ApplyFinalStateBiasing( const G4BiasingProcessInterface* callingProcess,
                        const G4Track*                            track,
                        const G4Step*                              step,
                        G4bool&    bval                               )
{
  //   G4cout << " GmBOptnEWBS " << callingProcess->GetProcessName() << G4endl; //GDEB
  std::map<const G4BiasingProcessInterface*,GmVEWBiasingOperation*>::const_iterator ite = theOperations.find(callingProcess);
  if( ite == theOperations.end() ) {
    std::map<G4String,GmVEWBiasingOperation*>::const_iterator ites = theOperationNames.find(callingProcess->GetProcessName());
    if( ites == theOperationNames.end() ) {
      return callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);
      theOperations[callingProcess] = (GmVEWBiasingOperation*)0;
    } else {
      theOperations[callingProcess] = (*ites).second;
      return theOperations[callingProcess]->ApplyFinalStateBiasing( callingProcess, track, step, bval );
    }
  } else {
    if( (*ite).second == 0 ) {
      return callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);
    } else {
      return (*ite).second->ApplyFinalStateBiasing( callingProcess, track, step, bval );
    }
  }

  
}
 

