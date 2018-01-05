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
#include "GmSDSimpleExactPos.hh"
#include "GmHit.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"
#include "GmSDVerbosity.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"

#include "CLHEP/Random/RandGauss.h"

//--------------------------------------------------------------------
GmSDSimpleExactPos::GmSDSimpleExactPos(const char* sdtype): GmVSD( sdtype )
{

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << "GmSDSimpleExactPos: SD type ->" << theSDType << " SD name " << SensitiveDetectorName << G4endl;
#endif

  theDetUnitIDBuilder = new GmDetUnitIDBuilderFromAncestors(theSDType);

}

GmSDSimpleExactPos::GmSDSimpleExactPos(G4String sdtype): GmVSD( sdtype )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << "GmSDSimpleExactPos: SD type ->" << theSDType << " SD name " << SensitiveDetectorName << G4endl;
#endif

  theDetUnitIDBuilder = new GmDetUnitIDBuilderFromAncestors(theSDType);

}

//--------------------------------------------------------------------
GmSDSimpleExactPos::~GmSDSimpleExactPos(){}

//--------------------------------------------------------------------
unsigned long long GmSDSimpleExactPos::GetDetUnitID( G4Step* aStep )
{
  return theDetUnitIDBuilder->BuildID( aStep );

}

//--------------------------------------------------------------------
void GmSDSimpleExactPos::UpdateHit( GmHit* hit, G4Step* aStep  )
{
  G4double energy = GetEnergyInSD( aStep );

  //--- Set position as barycenter
  G4ThreeVector posold = hit->GetPosition();
  G4ThreeVector posnew = aStep->GetPostStepPoint()->GetPosition();
  G4double enerold = hit->GetEnergy();
  G4double enernew = energy;
  G4ThreeVector pos = (posold*enerold+posnew*enernew)/(enerold+enernew);
  hit->SetPosition( pos );

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << hit->GetDetUnitID() << " position old " << posold << " new " << posnew << " FINAL " << pos << " enerold " << enerold << " new " << enernew << G4endl;
#endif
  hit->UpdateMe( aStep, energy );
}

//-------------------------------------------------------------------
void GmSDSimpleExactPos::CalculateAndSetPosition( GmHit* , G4Step* )
{
  return;
  //  G4cout << " GmSDSimpleExactPos::CalculateAndSetPosition pos " << 
} 


//-------------------------------------------------------------------
void GmSDSimpleExactPos::EndOfEvent(G4HCofThisEvent*HCE)
{
#ifndef GAMOS_NO_VERBOSE
  if(theHitsInEvent.size() != 0) if( SDVerb(debugVerb) ) G4cout << "GmSDSimpleExactPos::EndOfEvent SD: " << SensitiveDetectorName << " Nhits= " << theHitsInEvent.size() << G4endl;
#endif
  GmVSD::EndOfEvent( HCE );
}
