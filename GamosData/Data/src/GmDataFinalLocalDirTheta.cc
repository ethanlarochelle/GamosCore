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
#include "GmDataFinalLocalDirTheta.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataFinalLocalDirTheta::GmDataFinalLocalDirTheta()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalLocalDirTheta::~GmDataFinalLocalDirTheta()
{
}


//----------------------------------------------------------------
G4double GmDataFinalLocalDirTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( aStep->GetPostStepPoint()->GetMomentumDirection(), aStep->GetPostStepPoint()->GetTouchable()->GetHistory());

  return localDir.theta()/CLHEP::deg;

}

//----------------------------------------------------------------
G4double GmDataFinalLocalDirTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( aTrack->GetMomentumDirection(), aTrack->GetNextTouchable()->GetHistory());

  return localDir.theta()/CLHEP::deg;
}


