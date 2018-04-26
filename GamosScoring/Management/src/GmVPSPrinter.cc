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
#include "GmVPSPrinter.hh"
#include "GmScoringVerbosity.hh"
#include "GmVPrimitiveScorer.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-------------------------------------------------------------------
GmVPSPrinter::GmVPSPrinter(G4String name) : theName( name )
{
  theNewUnit = -1;
}

//-------------------------------------------------------------------
void GmVPSPrinter::SetParameters( const std::vector<G4String>& params )
{
  if( params.size() != 2 && params.size() != 0 ){ //if 0 parameters, scorer unit/unitname is taken
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception((theName+"::SetParameters").c_str(),
		"There should be two parameters: UNIT UNIT_NAME",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }

  if( params.size() == 2 ) {
    theNewUnit = GmGenUtils::GetValue( params[0] );
    theUnitName = params[1];
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) )
      G4cout << this << theName + "::SetParameters " << theUnit << " " << theUnitName << G4endl;
#endif
  }

}

//-------------------------------------------------------------------
void GmVPSPrinter::SetUnit(GmVPrimitiveScorer* theScorer )
{
  theUnit = theScorer->GetUnit();
  if( theNewUnit == -1 ) {
    theNewUnit = theScorer->GetUnit();
    theUnitName = theScorer->GetUnitName();
    //    G4cout << "SetUnit " <<  theName << " " << theNewUnit << " Name " << theUnitName << G4endl; //GDEB
  }
  theUnitRatio = theUnit/theNewUnit;
  theUnitRatio2 = theUnitRatio*theUnitRatio;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << " GmVPSPrinter::SetUnit " << theName << " " << theUnit << " New " << theNewUnit << " Ratio " << theUnitRatio << G4endl;
#endif
}
