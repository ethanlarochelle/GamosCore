#include "GmPSEdep_LET.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

GmPSEdep_LET::GmPSEdep_LET(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV*MeV/mm");
}

GmPSEdep_LET::~GmPSEdep_LET()
{;}
//AVOID THIS : G4Track Information:   Particle = O16[6129.890],   Track ID = 15,   Parent ID = 13
//        0   -0.349   0.0397     -159  0.000728        0        0         0     phantom initStep
//        1   -0.349   0.0397     -159         0 0.000728 2.66e-308 2.66e-308     phantom RadioactiveDecay
//    GmPSEdep_LET::ProcessHits EdepLET 1.993642016683088e+301 edep 0.0007282841543201357 edep/length 2.737450766787839e+304

G4bool GmPSEdep_LET::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event
  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  G4double length = aStep->GetStepLength();
  G4double let;
  if( length > 1.e-100 ) {
    let = edep * edep/length;
  } else {
    let = 0.;
  }
    G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "  GmPSEdep_LET::ProcessHits EdepLET " << let	 
	   << " edep " << edep
	   << " edep/length " << edep/length
	   << G4endl;
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, let, weight );
  
  return TRUE;
} 

