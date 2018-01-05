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
#include "GmG4PSKerma.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4UnitsTable.hh"
#include "G4VSolid.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring kerma (energy of charged secondaries from neutral primaries)
///////////////////////////////////////////////////////////////////////////////

GmG4PSKerma::GmG4PSKerma(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = CLHEP::gray;
  theUnitName = G4String("Gy");

  bUseVolume = true;
}

GmG4PSKerma::~GmG4PSKerma()
{;}

G4bool GmG4PSKerma::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return FALSE;

  const G4Track* aTrack = aStep->GetTrack();
  if( aTrack->GetDefinition()->GetPDGCharge() != 0 ) return FALSE;

  G4TrackVector secondaryTracks = GmG4Utils::GetSecondariesOfCurrentStep();
  G4TrackVector::const_iterator ite;
  G4double eseco = 0.;
  for( ite = secondaryTracks.begin(); ite != secondaryTracks.end(); ite++ ){
    G4Track* secoTrack = *ite;
    if( secoTrack->GetDefinition()->GetPDGCharge() != 0 ){
      eseco += secoTrack->GetKineticEnergy();
    }
  }

  if ( eseco == 0. ) return FALSE;
  G4double volume = GetVolume(aStep);
  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
  G4double kerma   = eseco / ( density * volume );

  FillScorerAtPostCheckingRegular( aStep, kerma, aStep->GetPreStepPoint()->GetWeight() );

  return TRUE;
}


void GmG4PSKerma::EndOfEvent(G4HCofThisEvent*)
{;}

void GmG4PSKerma::DrawAll()
{;}

void GmG4PSKerma::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  energy of secondaries: " << G4BestUnit(*(itr->second),"Energy") 
	   << G4endl;
  }
}
 #include "GamosCore/GamosBase/Base/include/GmVClassifier.hh" 
G4int GmG4PSKerma::GetIndex(G4Step* aStep ) 
 { 
 return theClassifier->GetIndexFromStep( aStep ); 
} 
