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
#include "GmDataAccumulatedKerma.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

//----------------------------------------------------------------
GmDataAccumulatedKerma::GmDataAccumulatedKerma()
{
  bInitial = false;
  theHMax = 1.;

  theExcludedTypes.clear(); // delete DTRun
  theExcludedTypes.insert(DTSeco);

  theGeomUtils= GmGeometryUtils::GetInstance();
}

//----------------------------------------------------------------
GmDataAccumulatedKerma::~GmDataAccumulatedKerma()
{
}

//----------------------------------------------------------------
void GmDataAccumulatedKerma::Accumulate( const G4Step* aStep, G4int index )
{
  if( aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0 ) return;
  G4TrackVector secondaryTracks = GmG4Utils::GetSecondariesOfCurrentStep();
  G4TrackVector::const_iterator ite;
  G4double eseco = 0.;
  for( ite = secondaryTracks.begin(); ite != secondaryTracks.end(); ite++ ){
    G4Track* secoTrack = *ite;
    if( secoTrack->GetDefinition()->GetPDGCharge() != 0 ){
      eseco += secoTrack->GetKineticEnergy();
    }
  }
  if ( eseco == 0. ) return;

  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
  theDataAccumulated[index] += eseco/ ( density * theGeomUtils->GetCubicVolume( aStep->GetTrack()->GetVolume() ));
}

//----------------------------------------------------------------
G4double GmDataAccumulatedKerma::GetValueFromStep( const G4Step* aStep, G4int )
{
  if( aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0 ) return 0.;
  G4TrackVector secondaryTracks = GmG4Utils::GetSecondariesOfCurrentStep();
  G4TrackVector::const_iterator ite;
  G4double eseco = 0.;
  for( ite = secondaryTracks.begin(); ite != secondaryTracks.end(); ite++ ){
    G4Track* secoTrack = *ite;
    if( secoTrack->GetDefinition()->GetPDGCharge() != 0 ){
      eseco += secoTrack->GetKineticEnergy();
    }
  }
  if ( eseco == 0. ) return 0.;

  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
  return eseco/ ( density * theGeomUtils->GetCubicVolume( aStep->GetTrack()->GetVolume() ));
}
