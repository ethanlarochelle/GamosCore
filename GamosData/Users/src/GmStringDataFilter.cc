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
#include "GmStringDataFilter.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Track.hh"
#include "G4UnitsTable.hh"

//----------------------------------------------------------------------------
GmStringDataFilter::GmStringDataFilter(G4String name)
  :GmVFilter(name)
{
}

//----------------------------------------------------------------------------
GmStringDataFilter::~GmStringDataFilter()
{
}

//----------------------------------------------------------------------------
G4bool GmStringDataFilter::AcceptStep(const G4Step* aStep)
{
  G4bool bFound = FALSE;
  std::set<G4String>::const_iterator ite;
  G4String stringData = theData[0]->GetStringValueFromStep( aStep );
  for( ite = theStrings.begin(); ite != theStrings.end(); ite++ ) {
    if( GmGenUtils::AreWordsEquivalent( *ite, stringData ) ) {
      bFound = TRUE;
      break;
    }
  }
    
  if( bFound ) {
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmStringDataFilter::AcceptStep 1 " <<  theData[0]->GetStringValueFromStep( aStep ) << G4endl;
#endif
    return TRUE;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmStringDataFilter::AcceptStep 0 " <<  theData[0]->GetStringValueFromStep( aStep ) << G4endl;
#endif
    return FALSE;
  }
}

//----------------------------------------------------------------------------
G4bool GmStringDataFilter::AcceptTrack(const G4Track* aTrack)
{
  if( theStrings.find( theData[0]->GetStringValueFromTrack( aTrack ) ) != theStrings.end() ){
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmStringDataFilter::AcceptTrack 1 " <<  theData[0]->GetStringValueFromTrack( aTrack ) << G4endl;
#endif
    return TRUE;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmStringDataFilter::AcceptTrack 0 " <<  theData[0]->GetStringValueFromTrack( aTrack ) << G4endl;
#endif
    return FALSE;
  }
   
}

//----------------------------------------------------------------------------
void GmStringDataFilter::show() 
{
}

//----------------------------------------------------------------------------
void GmStringDataFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() == 0 ){
    G4Exception("GmStringDataFilter::SetParameters",
		"There should be at least one parameter: DATA_NAME ",
		FatalErrorInArgument,
		"There are none");
 }

  GmVData* data = Build1Data( params[0] );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " GmStringDataFilter::SetParameters " << params[0] << G4endl;
#endif
  data->SetName( params[0] );
  theData.push_back( data );

 if( theData[0]->GetCType() != "char" ) {
   G4Exception("GmStringDataFilter::SetParameters",
	       "Data is not of string type",
	       JustWarning,
	       G4String("Data is " + params[0] + " Maybe you want to use GmNumericDataFilter?").c_str());
 }

 if( params.size() == 1 ){
   G4String parastr;
   for( unsigned int ii = 0; ii < params.size(); ii++ ){
     parastr += params[ii] + " ";
   }
   G4Exception("GmStringDataFilter::SetParameters",
	       "There should at least two parameters: DATA_NAME VALUE_1 VALUE_2 ... ",
	       FatalErrorInArgument,
	       G4String("They are: "+parastr).c_str());
 }
   
 for( unsigned int ii = 0; ii < params.size(); ii++ ){
   theStrings.insert( params[ii] );
 }

}
