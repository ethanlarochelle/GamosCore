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
//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// GmG4PSTrackCounter
#include "GmG4PSTrackCounter.hh"


///////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring number of tracks in a cell.
//
// Created: 2007-02-02  Tsukasa ASO, Akinori Kimura.
//
///////////////////////////////////////////////////////////////////////////////

GmG4PSTrackCounter::GmG4PSTrackCounter(G4String name)
  :GmVPrimitiveScorer(name)
{
}

GmG4PSTrackCounter::~GmG4PSTrackCounter()
{;}

G4bool GmG4PSTrackCounter::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4StepPoint* posStep = aStep->GetPostStepPoint();

  if( !IsRegularScoring( aStep ) ) {
    
    G4bool IsEnter = preStep->GetStepStatus()==fGeomBoundary;
    G4bool IsExit  = posStep->GetStepStatus()==fGeomBoundary;
    
    // Regular : count in prestep volume.
    //  G4cout << " trk " << aStep->GetTrack()->GetTrackID()
    //	 << " index " << index << " In " << IsEnter << " Out " <<IsExit
    //	 << " PreStatus " << preStep->GetStepStatus() 
    //	 << " PostStatus " << posStep->GetStepStatus() 
    //	 << " w " << aStep->GetPreStepPoint()->GetWeight()
    // <<    G4endl;
    
    G4bool flag = FALSE;
    
    if ( IsEnter && fDirection == fCurrent_In ) flag = TRUE;
    else if ( IsExit  && fDirection == fCurrent_Out ) flag = TRUE;
    else if ( (IsExit||IsEnter) && fDirection == fCurrent_InOut  ) flag = TRUE;
    
    if ( flag ){
      FillScorer( aStep, 1., aStep->GetPreStepPoint()->GetWeight());
    }

  } else {
    std::vector< std::pair<G4int,G4double> >::const_iterator ite;
    std::vector< std::pair<G4int,G4double> >::const_iterator iteend = G4RegularNavigationHelper::Instance()->theStepLengths.end();
    iteend--;
    for( ite = G4RegularNavigationHelper::Instance()->theStepLengths.begin(); ite != G4RegularNavigationHelper::Instance()->theStepLengths.end(); ite++ ){
      G4bool IsEnter = ite != G4RegularNavigationHelper::Instance()->theStepLengths.begin() || preStep->GetStepStatus()==fGeomBoundary;
      G4bool IsExit = ite != iteend || posStep->GetStepStatus()==fGeomBoundary;

      G4bool flag = FALSE;
      
      if ( IsEnter && fDirection == fCurrent_In ) flag = TRUE;
      else if ( IsExit  && fDirection == fCurrent_Out ) flag = TRUE;
      else if ( (IsExit||IsEnter) && fDirection == fCurrent_InOut  ) flag = TRUE;
      
      G4cout << " TRACK COUNTER " << (*ite).first << " " << (*ite).second << " " << flag  << G4endl;
      if ( flag ){
	FillScorer( aStep, (*ite).first, 1., aStep->GetPreStepPoint()->GetWeight());
      }
      
    }
  
  }

  return TRUE;
}

void GmG4PSTrackCounter::EndOfEvent(G4HCofThisEvent*)
{;}

void GmG4PSTrackCounter::DrawAll()
{;}

void GmG4PSTrackCounter::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  track count: " << *(itr->second)
	   << G4endl;
  }
}
 #include "GamosCore/GamosBase/Base/include/GmVClassifier.hh" 
G4int GmG4PSTrackCounter::GetIndex(G4Step* aStep ) 
 { 
 return theClassifier->GetIndexFromStep( aStep ); 
} 
