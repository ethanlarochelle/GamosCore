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
#include "GmDataInitialDirTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataInitialDirTheta::GmDataInitialDirTheta()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 180.;
  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialDirTheta::~GmDataInitialDirTheta()
{
}


//----------------------------------------------------------------
G4double GmDataInitialDirTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMomentumDirection().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialDirTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexMomentumDirection().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialDirTheta::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetMomentumDirection().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialDirTheta::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  G4ThreeVector mom = aEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().unit();
  return mom.theta();
}
