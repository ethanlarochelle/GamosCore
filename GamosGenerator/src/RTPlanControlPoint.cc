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
#include "RTPlanControlPoint.hh"
#include "RTPlanBeam.hh"
#include "GmGenerVerbosity.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "G4tgrUtils.hh"
#include "TH2F.h"
#include "TCanvas.h"
#include "TEllipse.h"
#include "TColor.h"

//-----------------------------------------------------------------------
RTPlanControlPoint::RTPlanControlPoint( const G4String& fileName, RTPlanBeam* beam)
  : theBeam(beam)
{
  theFileName = fileName;

  GmFileIn fin = GmFileIn::GetInstance(fileName);
  std::vector<G4String> wl;
  for(;;) {
    if( !fin.GetWordsInLine(wl) ) break;
    
    //------------------------------- parameter number
    if( wl[0]== ":P" )  {
      CheckIfNewParameter( wl, 0 );
      theParams[ wl[1] ] = G4tgrUtils::GetDouble( wl[2] );
      //      G4cout << this << " CPReadParams " << wl[1] << " = " << wl[2] << G4endl; //GDEB
   //------------------------------- parameter string
    } else if( wl[0] == ":PS" ) {
      CheckIfNewParameter( wl, 0 );
      theParamStrs[ wl[1] ] = wl[2];
    }
  }

  //G4int CPNo = GetParam("ControlPointIndex",1);
  //  theAccumulativeMeterset = GetParam("AccumulativeMeterset",1);
  theGantryAngle = GetParam("GantryAngle",1)*CLHEP::deg;
  theGantryPitchAngle = GetParam("GantryPitchAngle",0)*CLHEP::deg;
  //  theIsocenterPosition = G4ThreeVector( GetParam("IsocenterPosition_X",1), GetParam("IsocenterPosition_Y",1), GetParam("IsocenterPosition_Z",1) );
  theNominalBeamEnergy = GetParam("NominalBeamEnergy",1);
  theSourceAxisDistance = theBeam->GetParam("SourceAxisDistance",1);
  theIndex = G4int(GetParam("ControlPointIndex",1));

  theMetersets.push_back(GetParam("MetersetWeight",1));
  theNumberOfPaintings = 1; //only one meterset for RTPlan, several for RTPlan

  iCurrentScanSpot = 0;  

}

//ROTATIONS FOLLOW IEC 61217    ftp://dicom.nema.org/MEDICAL/dicom/2014c/output/chtml/part03/figures/PS3.3_C.8.8.25-2a.svg     ftp://dicom.nema.org/MEDICAL/dicom/2014c/output/chtml/part03/figures/PS3.3_C.8.8.25-2b.svg
//-----------------------------------------------------------------------
ScanSpotData RTPlanControlPoint::NextScanSpot( G4int& incp )
{
  //  G4cout << " RTPlanControlPoint::NextScanSpot " << incp << G4endl; //GDEB 
  ScanSpotData scanSpotData;
  G4int iNextScanSpot = iCurrentScanSpot + 1;
  
  if( GenerVerb(debugVerb) ) G4cout << theBeam->GetIndex() << " : " << GetIndex() << " RTPlanControlPoint::NextScanSpot iNextScanSpot " <<  iNextScanSpot << " NSCANSPOTS= " << theMetersets.size() << G4endl;
  
  if( iNextScanSpot <= 1 ) {
    // By default (GantryAngle Y) accelerator head local coordinates X,Y are X,Y in global coordinates : IEC 61217 
    scanSpotData.PositionX = 0.;
    scanSpotData.PositionY = 0.;
    scanSpotData.PositionZ = 0;
    scanSpotData.Energy = theNominalBeamEnergy;
    scanSpotData.RotAngleX = 0.;
    scanSpotData.RotAngleY = theGantryAngle;
    scanSpotData.RotAngleZ = theGantryPitchAngle;
    scanSpotData.SpotSize = -DBL_MAX; // change it in PlanSource
    scanSpotData.SourceAxisDistance = theSourceAxisDistance;
    scanSpotData.ControlPoint = this;
    scanSpotData.Beam = theBeam;
    iCurrentScanSpot = iNextScanSpot;
    scanSpotData.IsocenterPosition = G4ThreeVector(GetParam("IsocenterPosition_X"),GetParam("IsocenterPosition_Y"),GetParam("IsocenterPosition_Z"));
    theMetersets.push_back(GetParam("MetersetWeight",1));
  } else {
    incp++;
    iCurrentScanSpot = 0;  
  }

  return scanSpotData;
   
}

