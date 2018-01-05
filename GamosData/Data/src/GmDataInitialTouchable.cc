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
#include "GmDataInitialTouchable.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4TransportationManager.hh"
#include "G4TouchableHistory.hh"
#include "G4Navigator.hh"

//----------------------------------------------------------------
GmDataInitialTouchable::GmDataInitialTouchable()
{
  bInitial = true;
  theNChar = 25;
  theNBytes = theNChar;

  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialTouchable::~GmDataInitialTouchable()
{
}

//----------------------------------------------------------------
G4String GmDataInitialTouchable::GetStringValueFromStep( const G4Step* aStep )
{ 
  return GetTouchableLongName( aStep->GetPreStepPoint()->GetTouchable() );
}

//----------------------------------------------------------------
G4String GmDataInitialTouchable::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aTrack->GetVertexPosition(), touch, false ); 

  G4String name = GetTouchableLongName( touch );
 
  delete touch;

  return name;
}

//----------------------------------------------------------------
G4String GmDataInitialTouchable::GetStringValueFromSecoTrack( const G4Track* aTrack1, const G4Track* )
{
  return GetTouchableLongName( aTrack1->GetNextTouchable() );
}

//----------------------------------------------------------------
G4String GmDataInitialTouchable::GetStringValueFromEvent( const G4Event* anEvent )
{ 
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( anEvent->GetPrimaryVertex(0)->GetPosition(), touch, false ); 

  G4String name = GetTouchableLongName( touch );
 
  delete touch;

  return name;
}

//----------------------------------------------------------------
G4String GmDataInitialTouchable::GetTouchableLongName( const G4VTouchable* touch )
{
  G4int nAnces = touch->GetHistoryDepth();
  G4String name;
  for( G4int ii = nAnces; ii >= 0; ii-- ) {
    name += "/" + touch->GetVolume(ii)->GetName();
    name += ":" + GmGenUtils::itoa( touch->GetCopyNumber(ii) );
  }

  return name;
}
