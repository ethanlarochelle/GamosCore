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
#include "GmDataFinalSolid.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"

//----------------------------------------------------------------
GmDataFinalSolid::GmDataFinalSolid()
{
  bInitial = false;
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalSolid::~GmDataFinalSolid()
{
}

//----------------------------------------------------------------
G4String GmDataFinalSolid::GetStringValueFromStep( const G4Step* aStep )
{ 
  G4VPhysicalVolume* PV = aStep->GetPostStepPoint()->GetPhysicalVolume();
  if( PV ) { 
    return PV->GetLogicalVolume()->GetSolid()->GetName();
  } else {
    return G4String("OutOfWorld");
  }
}

//----------------------------------------------------------------
G4String GmDataFinalSolid::GetStringValueFromTrack( const G4Track* aTrack )
{ 
   G4VPhysicalVolume* PV = aTrack->GetNextVolume();return aTrack->GetNextVolume()->GetLogicalVolume()->GetSolid()->GetName();
 if( PV ) { 
    return PV->GetLogicalVolume()->GetSolid()->GetName();
  } else {
    return G4String("OutOfWorld");
  }
}
