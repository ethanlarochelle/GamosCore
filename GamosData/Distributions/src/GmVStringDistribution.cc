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
#include "GmVStringDistribution.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosData/Management/include/GmDataMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

//---------------------------------------------------------------------
GmVStringDistribution::GmVStringDistribution(G4String name)
  : GmVDistribution(name)
{
  bAllValuesDefined = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":AllValuesDefined",0));

  ReadFile();
}

//---------------------------------------------------------------------
void GmVStringDistribution::ReadFile()
{
  //  G4cout << " GmVStringDistribution::ReadFile " << G4endl;
  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  G4String fileName = parMgr->GetStringValue(theName+":FileName","");

  if( fileName != "" ) {
    ReadFileText( fileName );
  }
  else {
    G4Exception("GmVStringDistribution::GmVStringDistribution",
		"No file name given",
		FatalErrorInArgument,
		G4String("DISTRIBUTION: "+theName).c_str());
  }
  
  if( theStringValues.size() < 1 ) {
    G4Exception("GmVStringDistribution::GmVStringDistribution",
		"Only one value found in file!",
		FatalErrorInArgument,
		G4String("DISTRIBUTION: "+theName).c_str());
  }
  
}

//---------------------------------------------------------------------
void GmVStringDistribution::BuildData()
{
  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  G4String dataName = parMgr->GetStringValue(theName+":Data","");
  theData = Build1StringData(dataName);
}


//---------------------------------------------------------------------
GmVData* GmVStringDistribution::Build1StringData(const G4String& dataName)
{

  GmVData* data = GmDataMgr::GetInstance()->BuildData(dataName);
  if( data == 0 ) {
    G4Exception("GmVDistribution::GmVDistribution",
		"Data class not found",
		FatalErrorInArgument,
		G4String("DISTRIBUTION: "+theName 
			 + "Asking for data: " + dataName).c_str());
  } else {
    if( ! data->CheckAvailability( DTStep ) ) {
      G4Exception("GmVDistribution::GmVDistribution",
		  "Data class not available for DTStep",
		  FatalErrorInArgument,
		  G4String("DISTRIBUTION: "+theName 
			   + "Asking for data: " + dataName).c_str());
    } 

    if( data->GetCType() != "char" ){ 
      G4Exception("GmVDistribution::GmVDistribution",
		  "Data class has to be of String type",
		  FatalErrorInArgument,
		  G4String("DISTRIBUTION: "+theName 
			   + "Asking for data: " + dataName).c_str());
      
    }
  }

  return data;
}

//---------------------------------------------------------------------
void GmVStringDistribution::ReadFileText( G4String& fileName )
{
  G4cout << " GmVStringDistribution::ReadFileText: " << fileName << G4endl;

  G4String pathc = getenv("GAMOS_SEARCH_PATH");
  fileName = GmGenUtils::FileInPath( pathc, fileName);
  GmFileIn fin = GmFileIn::GetInstance(fileName);
  std::vector<G4String> wl;
  G4int ii = 1;
  for( ;; ){
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl.size() != 2) {
      G4Exception("GmVStringDistribution::ReadFileText",
		  "Error in number of words in line",
		  FatalErrorInArgument,
		  G4String("Reading file " + fileName 
			   + ", line number " + GmGenUtils::itoa(ii) 
			   + " All lines must have two words: VALUE PROBABILITY").c_str());
    }
    theStringValues[ wl[0] ] = GmGenUtils::GetValue( wl[1] );
    //      G4cout << " theStringValues " << wl[0] << " = " << wl[1] << G4endl;
    ii++;
  }
  
}


//---------------------------------------------------------------------
G4double GmVStringDistribution::GetValueFromTrack(const G4Track* aTrack)
{

  if( !theData ) { 
    G4Exception("GmVStringDistribution::GetValueFromTrack",
		"Data type is not defined",
		FatalException,
		G4String("Use command /gamos/setParam " + theName + ":Data").c_str());
  } 

  G4String indexVal = theData->GetStringValueFromTrack( aTrack );

  return GetStringValueFromIndex( indexVal );
}


//---------------------------------------------------------------------
G4double GmVStringDistribution::GetStringValueFromIndex(const G4String& indexVal)
{
  G4double value = -1.;

  std::map<G4String,G4double>::const_iterator ite = theStringValues.find(indexVal);

  if( ite == theStringValues.end() ){
    for( ite = theStringValues.begin(); ite != theStringValues.end(); ite++ ){
      if( GmGenUtils::AreWordsEquivalent( (*ite).first, indexVal ) ) break;
    }
  }

  if( ite == theStringValues.end() ){
    G4ExceptionSeverity severity = FatalException;
    if( !bAllValuesDefined ) {
      severity = JustWarning;
    }
    G4Exception("GmVStringDistribution::GetStringValueFromIndex",
		"Value not found in list!",
		severity,
		G4String("VALUE= "+indexVal+" DISTRIBUTION= "+theName).c_str());

  } else {
    value = (*ite).second;
  }

  return value;

}


//---------------------------------------------------------------------
void GmVStringDistribution::Normalize()
{

  std::map<G4String,G4double>::iterator ite;
  G4double valMax = -DBL_MAX;
  for( ite = theStringValues.begin(); ite != theStringValues.end(); ite++ ){
    valMax = std::max(valMax,(*ite).second);
  }

  for( ite = theStringValues.begin(); ite != theStringValues.end(); ite++ ){
    (*ite).second /= valMax;
  }

}

