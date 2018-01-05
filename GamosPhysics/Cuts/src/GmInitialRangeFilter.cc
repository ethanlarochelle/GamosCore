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
#include "GmInitialRangeFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"
#include "G4EmCalculator.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "GamosCore/GamosPhysics/Cuts/include/GmCutsEnergy2Range.hh"

//-----------------------------------------------------------
GmInitialRangeFilter::GmInitialRangeFilter(G4String name)
  :GmVFilter(name)
{
  theCutsEnergy2Range = new GmCutsEnergy2Range;
}

//-----------------------------------------------------------
GmInitialRangeFilter::~GmInitialRangeFilter()
{;}

//-----------------------------------------------------------
G4bool GmInitialRangeFilter::AcceptTrack(const G4Track* aTrack)
{
  G4EmCalculator* calculator = new G4EmCalculator();
  if( aTrack->GetTrackStatus() == fStopAndKill ) return bAccept;

  //is wrong  G4double range = G4EnergyLossTables::GetRange(aTrack->GetDefinition(),aTrack->GetVertexKineticEnergy(),aTrack->GetVolume()->GetLogicalVolume()->GetMaterialCutsCouple());
  for( G4double zz = -5.; zz < 5; zz+=0.01) {
    G4double ener = pow(10.,zz);
    G4double range = theCutsEnergy2Range->ConvertSlow( ener, aTrack->GetDefinition(),aTrack->GetVolume()->GetLogicalVolume()->GetMaterialCutsCouple());
    
    G4double range2 = G4LossTableManager::Instance()->GetRange(aTrack->GetDefinition(),ener,aTrack->GetVolume()->GetLogicalVolume()->GetMaterialCutsCouple());
    
    G4cout << " particle= " << aTrack->GetDefinition()->GetParticleName() 
	   << " material= " << aTrack->GetVolume()->GetLogicalVolume()->GetMaterial()->GetName() 
	   << G4endl 
	   << " energy= " << ener 
	   << " r_LossTables= " << range2 
	   << " r_Calcu= " << calculator->GetRange(ener,aTrack->GetDefinition(),aTrack->GetVolume()->GetLogicalVolume()->GetMaterial())
	   << " r_CalcuCSDA= " << calculator->GetCSDARange(ener,aTrack->GetDefinition(),aTrack->GetVolume()->GetLogicalVolume()->GetMaterial())
	   << " r_GAMOS= " << range << G4endl;
  }

  G4double range = theCutsEnergy2Range->ConvertSlow( aTrack->GetVertexKineticEnergy(), aTrack->GetDefinition(),aTrack->GetVolume()->GetLogicalVolume()->GetMaterialCutsCouple());
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) {
      G4cout << "  GmInitialRangeFilter::AcceptTrack range= " << range << " energy= " <<aTrack->GetVertexKineticEnergy() << " volume= " << aTrack->GetVolume()->GetLogicalVolume()->GetName() << " lowLimit= " << fLowLimit << " highLimit= " << fHighLimit << G4endl;
  }
#endif
  bAccept = TRUE;
  if ( range < fLowLimit  ) bAccept = FALSE;
  if ( range > fHighLimit ) bAccept = FALSE;
  
  return bAccept;
}

//-----------------------------------------------------------
G4bool GmInitialRangeFilter::AcceptStep(const G4Step*)
{
  return bAccept;
}

//-----------------------------------------------------------
void GmInitialRangeFilter::show() 
{
    G4cout << " GmInitialRangeFilter:: " << GetName()
	 << " LowE  " << G4BestUnit(fLowLimit,"Position") 
	 << " HighE " << G4BestUnit(fHighLimit,"Position")
	 <<G4endl;
}


//-----------------------------------------------------------
void GmInitialRangeFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmInitialRangeFilter::SetParameters","There should be two  parameters: min_range max_range",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
 }

 fLowLimit  = GmGenUtils::GetValue( params[0] );
 fHighLimit = GmGenUtils::GetValue( params[1] );

}
