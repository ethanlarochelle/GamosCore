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
#include "GmNumericDataFilter.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Track.hh"
#include "G4UnitsTable.hh"

//----------------------------------------------------------------------------
GmNumericDataFilter::GmNumericDataFilter(G4String name)
  :GmVFilter(name)
{
}

//----------------------------------------------------------------------------
GmNumericDataFilter::~GmNumericDataFilter()
{
}

//----------------------------------------------------------------------------
G4bool GmNumericDataFilter::AcceptStep(const G4Step* aStep)
{
  G4double val = theData[0]->GetValueFromStep( aStep );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) )   G4cout << "  GmNumericDataFilter::AcceptStep " << val << " lowerLimit " << theLowerLimit << " upperLimit " << theUpperLimit << G4endl;
#endif
  if ( val < theLowerLimit ) return FALSE;
  if ( val > theUpperLimit ) return FALSE;
  if( DataVerb(debugVerb) )   G4cout << "  GmNumericDataFilter::AcceptStep  ACCEPTED " << G4endl;
  return TRUE;
  
}

//----------------------------------------------------------------------------
G4bool GmNumericDataFilter::AcceptTrack(const G4Track* aTrack)
{
  G4double val = theData[0]->GetValueFromTrack( aTrack );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) )   G4cout << "  GmNumericDataFilter::AcceptTrack " << val << " lowerLimit " << theLowerLimit << " upperLimit " << theUpperLimit << G4endl;
#endif
  if ( val < theLowerLimit ) return FALSE;
  if ( val > theUpperLimit ) return FALSE;
  return TRUE;
    
}

//----------------------------------------------------------------------------
void GmNumericDataFilter::show() 
{
}

//----------------------------------------------------------------------------
void GmNumericDataFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() == 0 ){
    G4Exception("GmNumericDataFilter::SetParameters",
		"There should be at least one parameter: DATA_NAME ",
		FatalErrorInArgument,
		"There are none");
 }

  GmVData* data = Build1Data( params[0] );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " GmNumericDataFilter::SetParameters " << params[0] << G4endl;
#endif
  data->SetName( params[0] );
  theData.push_back( data );
  
 if( theData[0]->GetCType() == "char" ) {
   G4Exception("GmNumericDataFilter::SetParameters",
		"Data is not of numeric type",
  		JustWarning,
		G4String("Data is " + params[0] + " Maybe you want to use GmStringDataFilter?").c_str());
  }

 if( params.size() != 3 ){
   G4String parastr;
   for( unsigned int ii = 0; ii < params.size(); ii++ ){
     parastr += params[ii] + " ";
   }
   G4Exception("GmNumericDataFilter::SetParameters",
	       "There should be three parameters: DATA_NAME LOWER_LIMIT UPPER_LIMIT ",
	       FatalErrorInArgument,
	       G4String("They are: "+parastr).c_str());
 }

 
 theLowerLimit = GmGenUtils::GetValue( params[1] );
 theUpperLimit = GmGenUtils::GetValue( params[2] );
 
}
