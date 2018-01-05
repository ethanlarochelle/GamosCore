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
#include "GmDataInitialLocalDirY.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialLocalDirY::GmDataInitialLocalDirY()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1.;
}

//----------------------------------------------------------------
GmDataInitialLocalDirY::~GmDataInitialLocalDirY()
{
}


//----------------------------------------------------------------
G4double GmDataInitialLocalDirY::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( aStep->GetPreStepPoint()->GetMomentumDirection(), aStep->GetPreStepPoint()->GetTouchable()->GetHistory());

  return localDir.y();

}

//----------------------------------------------------------------
G4double GmDataInitialLocalDirY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector pos = aTrack->GetVertexPosition();
  G4ThreeVector dir = aTrack->GetVertexMomentumDirection();
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, touch, false ); 

  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( dir, touch->GetHistory() );

  return localDir.y();
}

//----------------------------------------------------------------
G4double GmDataInitialLocalDirY::GetValueFromSecoTrack( const G4Track* aTrack1 , const G4Track* aTrack2, G4int )
{
  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( aTrack2->GetMomentumDirection(), aTrack1->GetNextTouchable()->GetHistory() );

  return localDir.y();
}

//----------------------------------------------------------------
G4double GmDataInitialLocalDirY::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4ThreeVector pos = anEvent->GetPrimaryVertex(0)->GetPosition();

  G4ThreeVector dir = anEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().unit();
  
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos , touch, false ); 

  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( dir, touch->GetHistory() );

  return localDir.y();
}

