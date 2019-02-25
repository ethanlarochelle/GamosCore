#include "GmPSEdep_LET.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

GmPSEdep_LET::GmPSEdep_LET(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("CLHEP::MeV*CLHEP::MeV/CLHEP::mm");
}

GmPSEdep_LET::~GmPSEdep_LET()
{;}

G4bool GmPSEdep_LET::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event
  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  G4double length = aStep->GetStepLength();
  G4double let = edep*edep/length;
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "  GmPSEdep_LET::ProcessHits EdepLET " << let	 
	   << " edep*edep " << edep*edep
	   << " / length " << length
	   << G4endl;
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, let, weight );
  
  return TRUE;
} 

