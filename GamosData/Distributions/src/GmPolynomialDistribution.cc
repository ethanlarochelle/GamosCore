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
#include "GmPolynomialDistribution.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmHistoReaderCSV.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosData/Management/include/GmDataMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

#ifndef GAMOS_NO_ROOT
#include "TFile.h"
#include "TH1F.h"
#include "TAxis.h"
#endif
//---------------------------------------------------------------------
GmPolynomialDistribution::GmPolynomialDistribution(G4String name)
: GmVDistribution(name)
{
  BuildData();
}

//---------------------------------------------------------------------
void GmPolynomialDistribution::SetParameters( std::vector<G4String>& params )
{
  if( params.size() == 0 ) {
    G4Exception("GmPolynomialDistribution::SetParameters",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("There should be at least one parameter, there is none").c_str());
  }

  for( size_t ii = 0; ii < params.size(); ii++) {
    theParams.push_back( GmGenUtils::GetValue( params[ii] ) );
  } 


#ifndef GAMOS_NO_VERBOSE
  if( DistVerb(debugVerb) ) {
    G4cout << "GmPolynomialDistribution::SetParameters ";
    for( size_t ii = 0; ii < theParams.size(); ii++) {
      G4cout << "a_" << ii << "= " << theParams[ii];
    } 
    G4cout << G4endl;
  } 
#endif
}

//---------------------------------------------------------------------
void GmPolynomialDistribution::BuildData()
{  
  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  G4String dataName = parMgr->GetStringValue(theName+":Data","");
  theData = GmDataMgr::GetInstance()->BuildData(dataName);
  if( theData == 0 ) {
    G4Exception("GmPolynomialDistribution::GmPolynomialDistribution",
		"Data class not found",
		FatalErrorInArgument,
		G4String("DISTRIBUTION: "+theName 
			 + " Asking for data: " + dataName).c_str());
  } else {
    if( ! theData->CheckAvailability( DTStep ) ) {
      G4Exception("GmPolynomialDistribution::GmPolynomialDistribution",
		  "Data class not available for DTStep",
		  FatalErrorInArgument,
		  G4String("DISTRIBUTION: "+theName 
			   + " Asking for data: " + dataName).c_str());
    } 

    if( theData->GetCType() == "char" ){ 
      G4Exception("GmPolynomialDistribution::GmPolynomialDistribution",
		  "Data class cannot be of String type",
		  FatalErrorInArgument,
		  G4String("DISTRIBUTION: "+theName 
			   + " Asking for data: " + dataName).c_str());
      
    }

  }

}

//---------------------------------------------------------------------
G4double GmPolynomialDistribution::GetValueFromStep(const G4Step* aStep)
{
  if( !theData ) { 
    G4Exception("GmPolynomialDistribution::GetValueFromStep",
		"Data type is not defined",
		FatalException,
		G4String("Use command /gamos/setParam " + theName + ":Data").c_str());
  } 

  G4double indexVal = theData->GetValueFromStep( aStep );

  return GetNumericValueFromIndex( indexVal );
}

//---------------------------------------------------------------------
G4double GmPolynomialDistribution::GetValueFromTrack(const G4Track* aTrack)
{

  if( !theData ) { 
    G4Exception("GmPolynomialDistribution::GetValueFromTrack",
		"Data type is not defined",
		FatalException,
		G4String("Use command /gamos/setParam " + theName + ":Data").c_str());
  } 

  G4double indexVal = theData->GetValueFromTrack( aTrack );

  return GetNumericValueFromIndex( indexVal );
}

//---------------------------------------------------------------------
G4double GmPolynomialDistribution::GetNumericValueFromIndex(const G4double indexVal)
{
  G4double val = 0.;
  for( size_t ii = 0; ii < theParams.size(); ii++) {
    val += theParams[ii] * pow( double(indexVal), int(ii) );
  }

  return val;
}

//---------------------------------------------------------------------
void GmPolynomialDistribution::Normalize()
{
  G4Exception("GmPolynomialDistribution::Normalize",
	      "Error",
	      FatalException,
	      "GmPolynomialDistribution cannot be normalized");
  
}
