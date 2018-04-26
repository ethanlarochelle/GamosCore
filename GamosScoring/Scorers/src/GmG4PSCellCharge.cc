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
#include "GmG4PSCellCharge.hh"
#include "G4Track.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"


///////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring cell charge.
//   The Cell Charge is defined by  a sum of deposited charge inside the cell.
//
// 
///////////////////////////////////////////////////////////////////////////////

GmG4PSCellCharge::GmG4PSCellCharge(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnitName = " [e]";
}

GmG4PSCellCharge::~GmG4PSCellCharge()
{;}

G4bool GmG4PSCellCharge::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmG4PSCellCharge::ProcessHits " << G4endl;
#endif
  if( !AcceptByFilter( aStep ) ) return false;

  // Enter or First step of primary.
  if( aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary 
      || ( aStep->GetTrack()->GetParentID() == 0 &&
	   aStep->GetTrack()->GetCurrentStepNumber() == 1 ) ){
    G4double charge = aStep->GetPreStepPoint()->GetCharge();
    if( charge != 0. ) {
      FillScorer( aStep, charge, aStep->GetPreStepPoint()->GetWeight() );
    }
  }
  
  // Exit
  if( aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary){
    G4double charge = aStep->GetPreStepPoint()->GetCharge();
    if( charge != 0. ) {
      FillScorer( aStep, -charge, aStep->GetPreStepPoint()->GetWeight() );
    } 
  }
  
  return TRUE;
}

