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
#include "GmPSVolumeFlux.hh"
#include "GmScoringSurfaceBox.hh"
#include "GmScoringSurfaceTubs.hh"
#include "GmScoringSurfaceSphere.hh"
#include "GmScoringSurfaceOrb.hh"

#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

//-----------------------------------------------------------------------
GmPSVolumeFlux::GmPSVolumeFlux(G4String name)
 :GmVPrimitiveScorer(name)
{
  theUnit = CLHEP::mm*CLHEP::mm/(CLHEP::cm*CLHEP::cm);
  theUnitName = G4String("CLHEP::cm-2");

  theGeometryUtils = GmGeometryUtils::GetInstance();  

  theTotalVolume = 0.;

  bUseVolume = true;
}


//-----------------------------------------------------------------------
G4bool GmPSVolumeFlux::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{

  if( !AcceptByFilter( aStep ) ) return false;

  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4double volume = GetVolume(aStep);

  G4double flux  = aStep->GetStepLength()/volume;

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmPSVolumeFlux::ProcessHits flux= " << flux << " stepLength " << aStep->GetStepLength() << " volume= " << volume << G4endl;
#endif     
  FillScorer( aStep, flux, preStepPoint->GetWeight() );

  return TRUE;
}

//-----------------------------------------------------------------------
void GmPSVolumeFlux::EndOfEvent(G4HCofThisEvent*)
{;}


//-----------------------------------------------------------------------
void GmPSVolumeFlux::DrawAll()
{;}

//-----------------------------------------------------------------------
void GmPSVolumeFlux::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() <<G4endl; 
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  flux  : " << *(itr->second)
	   << G4endl;
  }
}
 #include "GamosCore/GamosBase/Base/include/GmVClassifier.hh" 
G4int GmPSVolumeFlux::GetIndex(G4Step* aStep ) 
 { 
 return theClassifier->GetIndexFromStep( aStep ); 
} 
