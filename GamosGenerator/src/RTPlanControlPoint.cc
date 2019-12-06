#include "RTPlanControlPoint.hh"
#include "RTPlanBeam.hh"
#include "GmGenerVerbosity.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "G4tgrUtils.hh"

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

  G4double beamMS = 0.;  
  if( theBeam->ParamExists("BeamMeterset") ) {
    beamMS = theBeam->GetParam("BeamMeterset",0);
  } else {
    beamMS = theBeam->GetParam("FinalCumulativeMetersetWeight",1);
  }
  theMetersets.push_back(GetParam("MetersetWeight",1)*beamMS);
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
    theMetersets.push_back(GetParam("MetersetWeight",1)*theBeam->GetParam("BeamMeterset",1));
  } else {
    incp++;
    iCurrentScanSpot = 0;  
  }

  return scanSpotData;
   
}

