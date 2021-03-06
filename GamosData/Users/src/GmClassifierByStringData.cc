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
#include "GmClassifierByStringData.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//------------------------------------------------------------------
GmClassifierByStringData::GmClassifierByStringData(G4String name) : GmVClassifier( name )
{
}

//------------------------------------------------------------------
void GmClassifierByStringData::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 1 ) {
    G4String parastr = "";
    for( unsigned int ii=0; ii > params.size(); ii++ ){
      parastr += " " + params[ii];
    }
    G4Exception("GmClassifierByStringData::SetParameters"
		,G4String("Error in number of parameters at classifier"+theName).c_str()
  		,FatalErrorInArgument
		,G4String("There should be 1: DATA_NAME, and they are: "+parastr).c_str());
  }

  GmVData* data = Build1Data( params[0] );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout <<  "GmClassifierByStringData::SetParameters " << params[0] << G4endl;
#endif
  data->SetName( params[0] );
  theData.push_back( data );
 
  if( theData[0]->GetCType() != "char" ) {
    G4Exception("GmClassifierByStringData::SetParameters",
		"Data is not of string type",
  		JustWarning,
		G4String("Data is " + params[0] + " Maybe you want to use GmClassifierByNumericData?").c_str());
  }

}


//------------------------------------------------------------------
G4int GmClassifierByStringData::GetIndexFromStep(const G4Step* aStep)
{
  G4String val = theData[0]->GetStringValueFromStep( aStep );
  return GetIndexFromValue( val );
}

//------------------------------------------------------------------
G4int GmClassifierByStringData::GetIndexFromTrack(const G4Track* aTrack)
{
  G4String val = theData[0]->GetStringValueFromTrack( aTrack );
  return GetIndexFromValue( val );
}


//---------------------------------------------------------------
G4int GmClassifierByStringData::GetIndexFromSecoTrack(const G4Track* aTrack1, const G4Track* aTrack2)
{
  G4String val = theData[0]->GetStringValueFromSecoTrack( aTrack1, aTrack2 );
  return GetIndexFromValue(val);
}

//---------------------------------------------------------------
G4int GmClassifierByStringData::GetIndexFromValue(const G4String val )
{
  G4int index;
  std::map<G4String,G4int>::const_iterator ite = theIndexMap.find(val);
  if( ite == theIndexMap.end() ){
    index = theIndexMap.size()+1+theMaxIndex;
    theIndexMap[val] = index;
  } else {
    index = (*ite).second;
  }

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmClassifierByStringData::GetIndexFromValue " << theName << " : " << index << " key " << val << " index " << index << G4endl;
#endif

  return index;
}


//--------------------------------------------------------------
G4String GmClassifierByStringData::GetIndexName(G4int index)
{
  G4String name = "NOT_FOUND";
  std::map<G4String,G4int>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    //    G4cout << " (*ite).second == index " << (*ite).second << " == " << index << G4endl;
    if((*ite).second == index ){
      name = (*ite).first;
      break;
    }
  }
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmClassifierByStringData::GetIndexName " << theName << " : " << name << " index " << index << G4endl;
#endif
  return name;
}

//---------------------------------------------------------------
GmClassifierByStringData::~GmClassifierByStringData()
{
  //print names of each index 
  G4cout << "%%%%% Table of indices for GmClassifierByStringData " << theName << G4endl;
  std::map<G4String,G4int>::const_iterator ite;
  for( ite = theIndexMap.begin(); ite != theIndexMap.end(); ite++ ){
    G4cout << theName << ": " << (*ite).first << " = " << (*ite).second << G4endl;
  }
}

//-------------------------------------------------------------
void GmClassifierByStringData::SetIndices( std::vector<G4String> wl )
{
  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
    G4int index = G4int(GmGenUtils::GetValue(wl[ii+1]));
    theIndexMap[wl[ii]] = index;
    if( theMaxIndex < index) theMaxIndex = index;
  }
    
  theMaxIndex -= theIndexMap.size()+1;  
}
