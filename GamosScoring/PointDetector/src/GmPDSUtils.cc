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
#include "GmPDSUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//------------------------------------------------------------------
G4double GetWeight( const G4Track* aTrack, G4bool bWeightMustExist )
{
  G4VUserTrackInformation* trkInfo = aTrack->GetUserInformation();
  GmTrackInfo* trkInfow = dynamic_cast<GmTrackInfo*>(trkInfo);

  if( !trkInfow){
    if( bWeightMustExist ) {
      G4Exception("GmPDSUtils::GetWeight",
                  "Wrong argument",
                  FatalErrorInArgument,
                  "Track has no GmTrackInfo");
    } else {
      return aTrack->GetWeight();
    }
  } else if( trkInfow->GetType() != "GmPDSTrackInfo" ) { 
    return aTrack->GetWeight();
  }
  

  return trkInfow->GetWeight();
}


//------------------------------------------------------------------
void SetWeight( G4Track* aTrack, G4double wei )
{
  G4VUserTrackInformation* trkInfo = aTrack->GetUserInformation();
  GmTrackInfo* trkInfow = dynamic_cast<GmTrackInfo*>(trkInfo);

  if(!trkInfow){
    G4Exception("GmPDSUtils::SetWeight",
		"Wrong argument",
		FatalErrorInArgument,
		"Track has no GmTrackInfo");
  }
  
  trkInfow->SetWeight(wei);
  aTrack->SetWeight( wei );
}

//------------------------------------------------------------------
G4double GetPDSNumericParameter(const G4String& param, G4String partName, G4double defVal)
{
  if( partName == "G4Neutron" ) {
    partName = "neutron";
  } else if( partName == "G4Gamma" ) {
    partName = "gamma";
  }

  GmParameterMgr* theParamMgr = GmParameterMgr::GetInstance();
  G4String paramFull = "GmPDS:"+partName+":"+param;
  //  G4cout << "  GetPDSNumericParameter " << param << " " << partName << " " << defVal << " FULL " << paramFull << G4endl; 
  if( theParamMgr->IsParameterInScript(paramFull) ) {
    return theParamMgr->GetNumericValue(paramFull,defVal);
  } else {
    paramFull = "GmPDS:"+param;
    return theParamMgr->GetNumericValue(paramFull,defVal);
  }

}

//------------------------------------------------------------------
G4String GetPDSStringParameter(const G4String& param, G4String partName, G4String defVal)
{
  if( partName == "G4Neutron" ) {
    partName = "neutron";
  } else if( partName == "G4Gamma" ) {
    partName = "gamma";
  }

  GmParameterMgr* theParamMgr = GmParameterMgr::GetInstance();
  G4String paramFull = "GmPDS:"+partName+":"+param;
  //  G4cout << "  GetPDSStringParameter " << param << " " << partName << " " << defVal << " FULL " << paramFull << G4endl;
  if( theParamMgr->IsParameterInScript(paramFull) ) {
    return theParamMgr->GetStringValue(paramFull,defVal);
  } else {
    paramFull = "GmPDS:"+param;
    return theParamMgr->GetStringValue(paramFull,defVal);
  }

}


