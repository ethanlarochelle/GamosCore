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
#include "GmGeometricalBiasingDistribution.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//---------------------------------------------------------------------
GmGeometricalBiasingDistribution::GmGeometricalBiasingDistribution(G4String name)
  : GmVStringDistribution(name)
{
  BuildData();
}

//---------------------------------------------------------------------
void GmGeometricalBiasingDistribution::BuildData()
{
  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  G4String dataName = parMgr->GetStringValue(theName+":DataPre","");
  theDataPre = Build1StringData( dataName );

  dataName = parMgr->GetStringValue(theName+":DataPost","");
  theDataPost = Build1StringData( dataName );

  theMaxValue = parMgr->GetNumericValue(theName+":MaxValue",10);

  bNoRepeatWeight = G4bool( parMgr->GetNumericValue(theName+":NoRepeatWeight",0));

}


//---------------------------------------------------------------------
G4double GmGeometricalBiasingDistribution::GetValueFromStep(const G4Step* aStep)
{
  if( !theDataPre ) { 
    G4Exception("GmGeometricalBiasingDistribution::GetValueFromStep",
		"Data type is not defined",
		FatalException,
		G4String("Use command /gamos/setParam " + theName + ":DataPre").c_str());
  } 

  if( !theDataPost ) { 
    G4Exception("GmGeometricalBiasingDistribution::GetValueFromStep",
		"Data type is not defined",
		FatalException,
		G4String("Use command /gamos/setParam " + theName + ":DataPost").c_str());
  } 


  G4String indexPre = theDataPre->GetStringValueFromStep( aStep );
  G4String indexPost = theDataPost->GetStringValueFromStep( aStep );

  G4double valuePre = GetStringValueFromIndex( indexPre );
  G4double valuePost = GetStringValueFromIndex( indexPost );
  if( bNoRepeatWeight && 1.001/aStep->GetTrack()->GetWeight() >= valuePost )  {
#ifndef GAMOS_NO_VERBOSE
    if( DistVerb(debugVerb) && valuePost > valuePre ) {
      G4cout << " GmGeometricalBiasingDistribution NoRepeatWeight " << 1.001/aStep->GetTrack()->GetWeight() << " " << valuePost << G4endl;
    }
#endif
    return 1;
  }

  if( valuePre == 0. ) {
    return 0;
  } else {
    G4double indexVal = valuePost/valuePre;
    if( indexVal > theMaxValue ) {
      /*t      G4Exception("GmGeometricalBiasingDistribution::GetValueFromStep",
		  "Too big return value",
		  JustWarning,
		  G4String("Value = "+GmGenUtils::ftoa(indexVal)+ " >= " + GmGenUtils::ftoa(theMaxValue)).c_str()); */
      indexVal = theMaxValue;
    }
#ifndef GAMOS_NO_VERBOSE
    if( DistVerb(debugVerb)) {
      G4cout << "GmDistributionRatio::GetValueFromStep " << indexVal
	     << " indexPre " << indexPre 
	     << " indexPost " << indexPost
	     << " valuePre " << valuePre 
	     << " valuePost " << valuePost
	     << G4endl;
    }
#endif
    return indexVal;
  }

}
