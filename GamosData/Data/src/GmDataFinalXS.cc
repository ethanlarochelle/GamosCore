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
#include "GmDataFinalXS.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"

//----------------------------------------------------------------
GmDataFinalXS::GmDataFinalXS()
{
  bInitial = false;
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalXS::~GmDataFinalXS()
{
}
#include "G4ProcessManager.hh"
#include "G4HadronicProcess.hh"
#include "G4VEmProcess.hh"
#include "G4VEnergyLossProcess.hh"

//----------------------------------------------------------------
G4String GmDataFinalXS::GetStringValueFromStep( const G4Step* aStep )
{ 
  const G4Track* aTrack = aStep->GetTrack();
  G4ProcessManager* pmanager = aTrack->GetDefinition()->GetProcessManager();
  G4ProcessVector* pvect = pmanager->GetProcessList();
  G4double totalXS = 0.;
  int jj, sizproc = pvect->size();
  for( jj = 0; jj < sizproc; jj++ ) {
    G4VProcess* proc = (*pvect)[jj];
    if( static_cast<G4HadronicProcess*>(proc) ) {
      G4ForceCondition* fCond = 0;
      totalXS += 1./(static_cast<G4HadronicProcess*>(proc))->GetMeanFreePath(*aTrack,0.,fCond);
    } else if( static_cast<G4VEmProcess*>(proc) ) {
      totalXS += 1./(static_cast<G4VEmProcess*>(proc))->MeanFreePath(*aTrack);
    } else if( static_cast<G4VEnergyLossProcess*>(proc) ) {
      totalXS += 1./(static_cast<G4VEnergyLossProcess*>(proc))->MeanFreePath(*aTrack);
    } else {
      G4Exception("GmDataFinalXS::GetStringValueFromStep",
		  "",
		  FatalException,
		  ("Unknown type for process: " + proc->GetProcessName()).c_str());
    }
    
  }

  return totalXS;
}

//----------------------------------------------------------------
G4String GmDataFinalXS::GetStringValueFromTrack( const G4Track* aTrack )
{ 
   G4VPhysicalVolume* PV = aTrack->GetNextVolume();return aTrack->GetNextVolume()->GetLogicalVolume()->GetSolid()->GetName();
 if( PV ) { 
    return PV->GetLogicalVolume()->GetSolid()->GetName();
  } else {
    return G4String("OutOfWorld");
  }
}
