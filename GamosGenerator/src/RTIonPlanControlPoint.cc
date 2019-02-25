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
#include "RTIonPlanControlPoint.hh"
#include "RTIonPlanBeam.hh"
#include "GmGenerVerbosity.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "G4tgrUtils.hh"
#ifndef GAMOS_NO_ROOT
#include "TH2F.h"
#include "TCanvas.h"
#include "TEllipse.h"
#include "TColor.h"
#endif


//-----------------------------------------------------------------------
RTIonPlanControlPoint::RTIonPlanControlPoint( const G4String& fileName, RTIonPlanBeam* beam)
  : theBeam(beam)
{
  theFileName = fileName;

  GmFileIn fin = GmFileIn::GetInstance(fileName);
  std::vector<G4String> wl;
  //  G4cout << this << " RTIonPlanControlPoint " << fileName << G4endl; //GDEB
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
    } else if( wl[0] == "ScanSpotPositions" ) {
      G4int nSCs = GetParam("NumberOfScanSpotPositions",1);
      int ii = 0;
      for( ; ii < nSCs; ii++ ) {
	if( !fin.GetWordsInLine(wl) ) break;
	if( wl.size() != 3 ) {
	  G4Exception("RTIonPlanControlPoint",
		      "",
		      FatalException,
		      ("ScanSpotPosition should have 3 words (POS_X POS_Y METERSET), but it has "
		       + GmGenUtils::itoa(wl.size())).c_str());
	}
	thePosXs.push_back(GmGenUtils::GetValue(wl[0]));
	thePosYs.push_back(GmGenUtils::GetValue(wl[1]));
	G4double beamMS = 0.;  
	if( theBeam->ParamExists("BeamMeterset") ) {
	  beamMS = theBeam->GetParam("BeamMeterset",0);
	} else {
	  beamMS = theBeam->GetParam("FinalCumulativeMetersetWeight",1);
	}
	theMetersets.push_back(GmGenUtils::GetValue(wl[2])*beamMS);
	//	G4cout<< this << " " << ii << " ADD SCANSPOT " << theMetersets[ii] << " " << theMetersets.size() << G4endl; //GDEB
      } 
      if( ii != nSCs ) {
	G4Exception("RTIonPlanControlPoint",
		    "",
		    FatalException,
		    ("Number of ScanSpotPositions " + GmGenUtils::itoa(ii)
		     + " is different than expected " + GmGenUtils::itoa(nSCs)).c_str());
      }
    
    }
  }
    
  //G4int CPNo = GetParam("ControlPointIndex",1);
  //  theAccumulativeMeterset = GetParam("AccumulativeMeterset",1);
  theGantryAngle = GetParam("GantryAngle",1)*CLHEP::deg;
  theGantryPitchAngle = GetParam("GantryPitchAngle",1)*CLHEP::deg;
  //  theIsocenterPosition = G4ThreeVector( GetParam("IsocenterPosition_X",1), GetParam("IsocenterPosition_Y",1), GetParam("IsocenterPosition_Z",1) );
  theNominalBeamEnergy = GetParam("NominalBeamEnergy",1);
  theNumberOfPaintings = GetParam("NumberOfPaintings",1);
  theScanningSpotSize = GetParam("ScanningSpotSize",1);
  theSourceAxisDistance = theBeam->GetParam("VirtualSourceAxisDistances",1);
  theIndex = G4int(GetParam("ControlPointIndex",1));
 //theSnoutPosition = GetParam("SnoutPosition",1);
  /*  if(theNumberOfPaintings != 1 ) {
      G4Exception("RTIonPlanControlPoint::RTIonPlanControlPoint()",
                  "",
		  FatalException,
		  "NumberOfPaintings not treated, please contact GAMOS authors");
		  }*/

  iCurrentScanSpot = -1;
  iCurrentPainting = 1;

}

    
//ROTATIONS FOLLOW IEC 61217    ftp://dicom.nema.org/MEDICAL/dicom/2014c/output/chtml/part03/figures/PS3.3_C.8.8.25-2a.svg     ftp://dicom.nema.org/MEDICAL/dicom/2014c/output/chtml/part03/figures/PS3.3_C.8.8.25-2b.svg
//-----------------------------------------------------------------------
ScanSpotData RTIonPlanControlPoint::NextScanSpot( G4int& incp )
{
  //  G4cout << " RTIonPlanControlPoint::NextScanSpot " << incp << G4endl; //GDEB 
  ScanSpotData scanSpotData;
  G4int iNextScanSpot = iCurrentScanSpot + 1;
  
  if( GenerVerb(debugVerb) ) G4cout << "RTIonPlanControlPoint::NextScanSpot iNextScanSpot " <<  iNextScanSpot << " X= " <<  thePosXs[iNextScanSpot] << " NSCANSPOTS= " << thePosXs.size() << G4endl;
  
  if( iNextScanSpot < G4int(thePosXs.size()) ) {
    // By default (GantryAngle Z) accelerator head local coordinates X,Y are X,Y in global coordinates  ftp://dicom.nema.org/MEDICAL/dicom/2014c/output/chtml/part03/figures/PS3.3_C.8.8.25-2a.svg  
    scanSpotData.PositionX = thePosXs[iNextScanSpot];
    scanSpotData.PositionY = thePosYs[iNextScanSpot];
    scanSpotData.PositionZ = theBeam->GetBeamPosZ();
    scanSpotData.Energy = theNominalBeamEnergy;
    scanSpotData.RotAngleX = 0.;
    scanSpotData.RotAngleY = theGantryAngle;
    scanSpotData.RotAngleZ = theGantryPitchAngle;
    scanSpotData.SpotSize = theScanningSpotSize;
    scanSpotData.SourceAxisDistance = theSourceAxisDistance;
    scanSpotData.ControlPoint = this;
    scanSpotData.Beam = theBeam;
    iCurrentScanSpot = iNextScanSpot;
    scanSpotData.IsocenterPosition = G4ThreeVector(GetParam("IsocenterPosition_X"),GetParam("IsocenterPosition_Y"),GetParam("IsocenterPosition_Z"));
    //    G4cout << iNextScanSpot <<"  RTIonPlanControlPoint::NextScanSpot PositionX " <<   scanSpotData.PositionX << " Y " <<  scanSpotData.PositionY  << G4endl; //GDEB
  } else {
    if( iCurrentPainting >= theNumberOfPaintings ) {
      incp++;
    }
    iCurrentPainting++;
    iCurrentScanSpot = -1;
  }
  
  return scanSpotData; 
}

//-------------------------------------------------------------
void RTIonPlanControlPoint::PlotScanSpots(G4double minmaxX, G4double minmaxY, G4double maxMeterset)
{
#ifndef GAMOS_NO_ROOT
  G4String histoName = "RTIonPlanScanSpot_" + GmGenUtils::itoa(theBeam->GetIndex()) + "_" + GmGenUtils::itoa(theIndex);
  TH2F* histo = new TH2F( histoName.c_str(), histoName.c_str(), 100, -minmaxX, minmaxX, 100, -minmaxY, minmaxY );

  TCanvas* theCanvas = new TCanvas("");
  theCanvas->SetCanvasSize(1000,1000);
  theCanvas->Draw();
  histo->Draw();
  std::map<G4double,int> orderedSets;
  for( size_t ii = 0; ii < theMetersets.size(); ii++ ) {
    orderedSets[theMetersets[ii]] = ii;
  }
  
  std::map<G4double,int>::const_iterator ite;
  for( ite = orderedSets.begin(); ite != orderedSets.end(); ite++ ) {
    G4int ii = (*ite).second;
    TEllipse* el = new TEllipse(thePosXs[ii], thePosYs[ii], theScanningSpotSize/2., theScanningSpotSize/2. );
    G4int isatur = theMetersets[ii]/maxMeterset*100;
    el->SetFillColor(1000+isatur);
    el->SetFillStyle(1001);
    el->SetLineColor(1000+isatur);
    el->Draw();
  }

  theCanvas->Print((histoName+".gif").c_str());  
#endif
}
