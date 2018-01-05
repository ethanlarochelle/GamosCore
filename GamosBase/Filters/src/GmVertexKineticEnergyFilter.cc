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
#include "GmVertexKineticEnergyFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

GmVertexKineticEnergyFilter::GmVertexKineticEnergyFilter(G4String name)
  :GmVFilter(name)
{
}

GmVertexKineticEnergyFilter::~GmVertexKineticEnergyFilter()
{;}

G4bool GmVertexKineticEnergyFilter::AcceptStep(const G4Step* aStep)
{
  G4double kinetic = aStep->GetTrack()->GetVertexKineticEnergy();
  //  G4cout << "  GmVertexKineticEnergyFilter::AcceptTrack " << kinetic << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
  if ( kinetic < fLowEnergy  ) return FALSE;
  if ( kinetic > fHighEnergy ) return FALSE;
  return TRUE;
}

G4bool GmVertexKineticEnergyFilter::AcceptTrack(const G4Track* aTrack)
{
  G4double kinetic = aTrack->GetVertexKineticEnergy();
  //  G4cout << "  GmVertexKineticEnergyFilter::AcceptTrack " << kinetic << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
  if ( kinetic < fLowEnergy  ) return FALSE;
  if ( kinetic >= fHighEnergy ) return FALSE;
  return TRUE;
}

void GmVertexKineticEnergyFilter::show() 
{
    G4cout << " GmVertexKineticEnergyFilter:: " << GetName()
	 << " LowE  " << G4BestUnit(fLowEnergy,"Energy") 
	 << " HighE " << G4BestUnit(fHighEnergy,"Energy")
	 <<G4endl;
}


void GmVertexKineticEnergyFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmVertexKineticEnergyFilter::SetParameters",
		"There should be two  parameters: min_energy max_energy",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
 }

 fLowEnergy  = GmGenUtils::GetValue( params[0] );
 fHighEnergy = GmGenUtils::GetValue( params[1] );

}
