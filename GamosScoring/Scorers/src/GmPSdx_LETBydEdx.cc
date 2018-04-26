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
#include "GmPSdx_LETBydEdx.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4EmCalculator.hh"


GmPSdx_LETBydEdx::GmPSdx_LETBydEdx(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("CLHEP::MeV");
  theEmCalculator = new G4EmCalculator;
}

GmPSdx_LETBydEdx::~GmPSdx_LETBydEdx()
{;}

G4bool GmPSdx_LETBydEdx::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  G4StepPoint* stepPoint = aStep->GetPreStepPoint();
  G4ProductionCuts* prodCuts = stepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetRegion()->GetProductionCuts();
  G4ParticleDefinition* part =  aStep->GetTrack()->GetDefinition();
  G4double let = theEmCalculator->ComputeElectronicDEDX( stepPoint->GetKineticEnergy(), 
							 part, 
							 stepPoint->GetMaterial(), 
							 prodCuts->GetProductionCut(part->GetParticleName()));

  G4double stepLen = aStep->GetStepLength();
  G4double dxlet = stepLen*let;
  
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) {
    G4cout << "  GmPSdx_LETBydEdx::ProcessHits dxlet " << dxlet
	   << " let " << let
	   << " stepLength " << stepLen
	   << G4endl;
    G4cout << aStep->GetTrack()->GetKineticEnergy() << "  GmPSdx_LETBydEdx::ProcessHits let TEST " << (1.-let/(aStep->GetTotalEnergyDeposit()/aStep->GetStepLength())) << " " << let << " = " << aStep->GetTotalEnergyDeposit()/aStep->GetStepLength() << G4endl;
  }
#endif
  
  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, dxlet, weight );
  
  return TRUE;
} 

