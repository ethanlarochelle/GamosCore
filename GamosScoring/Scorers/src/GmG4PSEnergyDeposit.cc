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
//
// GmG4PSEnergyDeposit
#include "GmG4PSEnergyDeposit.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
///////////////////////////////////////////////////////////////////////////////

GmG4PSEnergyDeposit::GmG4PSEnergyDeposit(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("CLHEP::MeV");
}

GmG4PSEnergyDeposit::~GmG4PSEnergyDeposit()
{;}

G4bool GmG4PSEnergyDeposit::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "  GmG4PSEnergyDeposit::ProcessHits edep " << edep 
	   << " edep " << edep
	   << G4endl;
#endif

 // use index from classifier, unless you are skipping borders of equal material voxels in G4RegularNavigation
  if( bUseClassifierIndex || !bSkipEqualMaterials || !theRegularParamUtils->IsPhantomVolume( aStep->GetPreStepPoint()->GetPhysicalVolume() ) ) {
    G4int index = GetIndex(aStep);
    FillScorer( aStep, index, edep, weight );
    
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb)) G4cout  << "GmG4PSEnergyDeposit::ProcessHits  eDepo " << edep*weight << " index " << index << G4endl;
#endif
  } else {
    
    //----- Distribute edep in voxels 
    G4int numberVoxelsInStep= theEnergySplitter->SplitEnergyInVolumes( aStep );
    
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << "GmG4PSEnergyDeposit::ProcessHits totalEdepo " << aStep->GetTotalEnergyDeposit() 
					<< " Nsteps " << G4RegularNavigationHelper::Instance()->theStepLengths.size() 
					<< " numberVoxelsInStep " <<  numberVoxelsInStep
					<< G4endl;
#endif
    
    G4int index = -1;
    G4double stepLength = 0.;
    G4double energyDepo = 0.;
    for ( G4int ii =0; ii < numberVoxelsInStep; ii++ ){ 
      theEnergySplitter->GetLengthAndEnergyDeposited( ii, index, stepLength, energyDepo);
      
      FillScorer( aStep, index, energyDepo, weight );
    }
    
    //?  G4RegularNavigationHelper::ClearStepLengths();
  }

  return TRUE;
} 


void GmG4PSEnergyDeposit::EndOfEvent(G4HCofThisEvent*)
{;}

void GmG4PSEnergyDeposit::DrawAll()
{;}

void GmG4PSEnergyDeposit::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  energy deposit: " << G4BestUnit(*(itr->second),"Energy") 
	   << G4endl;
  }
}

#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
G4int GmG4PSEnergyDeposit::GetIndex(G4Step* aStep ) 
 { 
 return theClassifier->GetIndexFromStep( aStep ); 
} 
