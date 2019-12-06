////////////////////////////////////////////////////////////////////////
// Optical Photon Tracklength Fluence Implementation
////////////////////////////////////////////////////////////////////////
//
// File G4PSTrackLengthFluence.cc
// Description: Scores the tracklength fluence.
// Created: 2013-02-22
// Author: Adam Glaser
// Updated: 2019-12-06 by Ethan LaRochelle, port to 6.1
//
// This subroutine will perform record the fluence in voxelised geometry.
//
// mail:  adam.k.glaser@dartmouth.edu
//
////////////////////////////////////////////////////////////////////////

#include "GmPSTrackLengthFluence.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

GmPSTrackLengthFluence::GmPSTrackLengthFluence(G4String name)
:GmVPrimitiveScorer(name)
{
}

GmPSTrackLengthFluence::~GmPSTrackLengthFluence()
{;}

G4bool GmPSTrackLengthFluence::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;
  
  G4double trklength  = aStep->GetStepLength();
  if ( trklength == 0. ) return FALSE;

  G4Track* aTrack = aStep->GetTrack();
  const G4DynamicParticle* aParticle = aTrack->GetDynamicParticle();
  const G4Material* aMaterial = aTrack->GetMaterial();
  G4MaterialPropertiesTable* aMaterialPropertyTable =
	  aMaterial->GetMaterialPropertiesTable();
  G4double thePhotonEnergy = aParticle->GetTotalEnergy();
  
  G4double AttenuationLength1 = DBL_MAX;
  G4double AttenuationLength2 = DBL_MAX;
  G4double AttenuationLengthTotal = DBL_MAX;
  G4MaterialPropertyVector* AttenuationLengthVector;
  G4MaterialPropertyVector* AttenuationLengthVector2;  

  if ( aMaterialPropertyTable ) {
	   AttenuationLengthVector = aMaterialPropertyTable->
                                                GetProperty("ABSCOEF");
      if ( AttenuationLengthVector ){
             AttenuationLength1 = AttenuationLengthVector->
                                         Value(thePhotonEnergy);
      }
      else {
		AttenuationLength1=1.0;
      }
  } 
  else {
	   AttenuationLength1=1.0;
  }
  
  
  if ( aMaterialPropertyTable ) {
	   AttenuationLengthVector2 = aMaterialPropertyTable->
                                                GetProperty("FLUOR_ABSCOEF");
      if ( AttenuationLengthVector2 ){
             AttenuationLength2 = AttenuationLengthVector2->
                                         Value(thePhotonEnergy);
      }
      else {
		AttenuationLength2=1.0; 
      }
  } 
  else {
	    AttenuationLength2=1.0;
  }
  
  G4double mua1=1./AttenuationLength1;
  G4double mua2=1./AttenuationLength2;
  G4double mua=mua1+mua2;
  AttenuationLengthTotal=1./mua;

  trklength=trklength*AttenuationLengthTotal;

  if( !IsRegularScoring(aStep) ) {    
    FillScorer( aStep, trklength, aStep->GetPreStepPoint()->GetWeight());
    
  } else {
    std::vector< std::pair<G4int,G4double> >::const_iterator ite;
    for( ite = G4RegularNavigationHelper::Instance()->theStepLengths.begin(); ite != G4RegularNavigationHelper::Instance()->theStepLengths.end(); ite++ ){
      G4double trklength  = (*ite).second;      
      FillScorer( aStep, trklength, aStep->GetPreStepPoint()->GetWeight());
    }
  }
  
  return TRUE;
  
}

void GmPSTrackLengthFluence::EndOfEvent(G4HCofThisEvent*)
{;}

void GmPSTrackLengthFluence::DrawAll()
{;}

void GmPSTrackLengthFluence::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first << "  fluence: " ;
    G4cout  << "";
    G4cout << G4endl;
  }
}

