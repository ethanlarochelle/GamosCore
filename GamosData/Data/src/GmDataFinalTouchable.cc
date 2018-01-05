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
#include "GmDataFinalTouchable.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4VTouchable.hh"

//----------------------------------------------------------------
GmDataFinalTouchable::GmDataFinalTouchable()
{
  bInitial = false;
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalTouchable::~GmDataFinalTouchable()
{
}

//----------------------------------------------------------------
G4String GmDataFinalTouchable::GetStringValueFromStep( const G4Step* aStep )
{ 
  return GetTouchableLongName( aStep->GetPostStepPoint()->GetTouchable() );
}

//----------------------------------------------------------------
G4String GmDataFinalTouchable::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  return GetTouchableLongName( aTrack->GetNextTouchable() );
}


//----------------------------------------------------------------
G4String GmDataFinalTouchable::GetTouchableLongName( const G4VTouchable* touch )
{
  if( touch && touch->GetVolume(0) ) { 
   G4int nAnces = touch->GetHistoryDepth();
   G4String name;
   for( G4int ii = nAnces; ii >= 0; ii-- ) {
     name += "/" + touch->GetVolume(ii)->GetName();
     name += ":" + GmGenUtils::itoa( touch->GetCopyNumber(ii) );
   }
   return name;
  } else {
    return G4String("OutOfWorld");
  }

}
