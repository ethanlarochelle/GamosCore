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
#include "GmPositionYFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

//------------------------------------------------------------
GmPositionYFilter::GmPositionYFilter(G4String name)
  :GmVFilter(name)
{
}

//------------------------------------------------------------
GmPositionYFilter::~GmPositionYFilter()
{
}

//------------------------------------------------------------
G4bool GmPositionYFilter::AcceptStep(const G4Step* aStep)
{
  G4double val = aStep->GetPostStepPoint()->GetPosition().y();
  if ( val < theLowLimit  ) return FALSE;
  if ( val > theHighLimit ) return FALSE;
  return TRUE;
}

//------------------------------------------------------------
G4bool GmPositionYFilter::AcceptTrack(const G4Track* aTrack)
{
  G4double val = aTrack->GetPosition().y();
  if ( val < theLowLimit  ) return FALSE;
  if ( val > theHighLimit ) return FALSE;
  return TRUE;
}

//------------------------------------------------------------
void GmPositionYFilter::show() 
{
    G4cout << " GmPositionYFilter:: " << GetName()
	 << " LowLimiE  " << G4BestUnit(theLowLimit,"Position") 
	 << " HighLimit " << G4BestUnit(theHighLimit,"Position")
	 <<G4endl;
}

//------------------------------------------------------------
void GmPositionYFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPositionYFilter::SetParameters",
		"There should be two  parameters: low_limit high_limit",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
 }

 theLowLimit  = GmGenUtils::GetValue( params[0] );
 theHighLimit = GmGenUtils::GetValue( params[1] );

}
