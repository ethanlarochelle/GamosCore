#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4Geantino.hh"
#include "G4VPhysicalVolume.hh"

#ifndef GAMOS_NO_ROOT
#include "TMarker.h"
#include "TStyle.h"
#include "TText.h"
#include "TLine.h"
#endif

#include "GmGeneratorScanVis.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

//------------------------------------------------------------------------
GmGeneratorScanVis::GmGeneratorScanVis()
{
#ifdef GAMOS_NO_ROOT
  G4Exception("GmGeneratorScanVis::GmGeneratorScanVis",
	      "",
	      FatalException,
	      "GmGeneratorScanVis cannot be used without ROOT");
#else
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();

  std::vector<G4String> params;
  params = parmgr->GetVStringValue("GmGeneratorScanVis:ScanParams",params);
  if( params.size() == 0 ) {
    G4Exception("GmGeneratorScanVis",
		"",
		FatalException,
		"No command found: /gamos/setParam GmGeneratorScanVis::ScanParams XY/XZ/YZ STEP_FIGURE STEP_SCAN ANGLES ");
  }
  if( params.size() < 4 ) {
    G4Exception("GmGeneratorScanVis",
		"",
		FatalException,
		("Number of parameters has to be at least 4, while they are " + GmGenUtils::itoa(params.size())).c_str());
  }

  theStepFig = GmGenUtils::GetValue(params[1]);
  if( theStepFig == 0 ) {
    G4Exception("GmGeneratorScanVis",
		"",
		FatalException,
		"Second argument (Step of figures) in /gamos/setParam GmGeneratorScanVis::ScanParams  cannot be 0");
  }
  theStepScan = GmGenUtils::GetValue(params[2]);
  for( size_t ii = 3; ii < params.size(); ii++ ) {
    theAngles.push_back(GmGenUtils::GetValue(params[ii]));
  }
  if( theStepScan == 0. ) {
    G4Exception("GmGeneratorScanVis",
		"",
		FatalException,
		"Third argument (Step of scanning) in /gamos/setParam GmGeneratorScanVis::ScanParams  cannot be 0");
  }
  if( params[0] == "XY" ) {
    thePlaneFig = PF_XY;
    thePlaneFigName = "XY";
  } else if( params[0] == "XZ" ) {
    thePlaneFig = PF_XZ;
    thePlaneFigName = "XZ";
  } else if( params[0] == "YZ" ) {
    thePlaneFig = PF_YZ;
    thePlaneFigName = "YZ";
  } else {
        G4Exception("GmGeneratorScanVis",
		"",
		    FatalErrorInArgument,
		    ("First argument has to be XY, XZ or YZ, while it is " + params[0]).c_str());
  }

#endif
  bPrintAllTogether = G4bool(parmgr->GetNumericValue("GmGeneratorScanVis:bPrintAllTogether", 0));

}


//------------------------------------------------------------------------
GmGeneratorScanVis::~GmGeneratorScanVis()
{
}

//------------------------------------------------------------------------
void GmGeneratorScanVis::GeneratePrimaries(G4Event* evt)
{
#ifndef GAMOS_NO_ROOT
  if( !IncrementPosTransv() ) return;
  
  //  G4cout << " thePlaneFig " << thePlaneFig << " theAngles " << theAngles.size() << G4endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) )   G4cout << "GmGeneratorScanVis::GeneratePrimaries  PosTransv " << thePosTransv << G4endl; //GDEB
#endif
  double sstep;
  if( thePlaneFig == PF_XY ) {
    for( size_t ii = 0; ii < theAngles.size(); ii++ ) {
      G4double angle = theAngles[ii];
      SetDir( angle );
      double cosAng = cos(angle);
      double sinAng = sin(angle);
      if( angle < 90*CLHEP::deg ) {
	//----- SCAN ALONG Y AXIS AT NEGATIVE X
	sstep = fabs(theStepScan/cosAng);
	double posY = theScanWorldMinY + sstep;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << "PF_XY 0posY " << posY << G4endl; //GDEB
#endif
	for( ; posY < theScanWorldMaxY; posY += sstep ) {
	  //	  G4cout << " posY " << posY << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(theScanWorldMinX, posY, thePosTransv), evt ); 
	}
	if( angle != 0*CLHEP::deg ) {	
	  //----- SCAN ALONG X AXIS AT NEGATIVE Y
	  sstep = fabs(theStepScan/sinAng);
	  double posX = theScanWorldMinX + sstep;
	  for( ; posX < theScanWorldMaxX; posX += sstep ) {
	    GenerateParticle( G4ThreeVector(posX, theScanWorldMinY, thePosTransv), evt ); 
	  }
	}
      } else if( angle < 180*CLHEP::deg ) {
	//----- SCAN ALONG X AXIS AT NEGATIVE Y
	sstep = fabs(theStepScan/cosAng);
	double posX = theScanWorldMinX + sstep;
	//	G4cout << " 0posY " << posY << G4endl; //GDEB
	for( ; posX < theScanWorldMaxX; posX += sstep ) {
	  //	  G4cout << " posY " << posY << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(posX, theScanWorldMinY, thePosTransv), evt ); 
	}
	if( angle != 90*CLHEP::deg ) {	
	  //----- SCAN ALONG Y AXIS AT POSITIVE X
	  sstep = fabs(theStepScan/sinAng);
	  double posY = theScanWorldMinY + sstep;
	  for( ; posY < theScanWorldMaxY; posY += sstep ) {
	    GenerateParticle( G4ThreeVector(theScanWorldMaxX, posY, thePosTransv), evt ); 
	  }
	}
      } else if( angle < 270*CLHEP::deg ) {
	//----- SCAN ALONG Y AXIS AT POSITIVE X
	sstep = fabs(theStepScan/cosAng);
	double posY = theScanWorldMinY + sstep;
	//	G4cout << " 0posY " << posY << G4endl; //GDEB
	for( ; posY < theScanWorldMaxY; posY += sstep ) {
	  //	  G4cout << " posY " << posY << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(theScanWorldMaxX, posY, thePosTransv), evt ); 
	}
	if( angle != 180*CLHEP::deg ) {	
	  //----- SCAN ALONG X AXIS AT POSITIVE Y
	  sstep = fabs(theStepScan/sinAng);
	  double posX = theScanWorldMinX + sstep;
	  sstep = fabs(theStepScan/sinAng);
	  for( ; posX < theScanWorldMaxX; posX += sstep ) {
	    //	    G4cout << " posX " << posX << G4endl; //GDEB
	    GenerateParticle( G4ThreeVector(posX, theScanWorldMaxY, thePosTransv), evt ); 
	  }
	}
      } else { // < 360
	//----- SCAN ALONG X AXIS AT POSITIVE Y
	sstep = fabs(theStepScan/cosAng);
	double posX = theScanWorldMinX + sstep;
	//	G4cout << " 0posY " << posY << G4endl; //GDEB
	for( ; posX < theScanWorldMaxX; posX += sstep ) {
	  //	  G4cout << " posY " << posY << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(posX, theScanWorldMaxY, thePosTransv), evt ); 
	}
	if( angle != 270*CLHEP::deg ) {	
	  //----- SCAN ALONG Y AXIS AT NEGATIVE X
	  sstep = fabs(theStepScan/sinAng);
	  double posY = theScanWorldMinY + sstep;
	  for( ; posY < theScanWorldMaxY; posY += sstep ) {
	    GenerateParticle( G4ThreeVector(theScanWorldMinX, posY, thePosTransv), evt ); 
	  }
	}
      }
    }
    //---------------------------------------    
  } else if( thePlaneFig == PF_XZ ) {
 #ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) G4cout << "PF_XZ NAngles " <<theAngles.size() << G4endl; //GDEB
#endif
   for( size_t ii = 0; ii < theAngles.size(); ii++ ) {
      G4double angle = theAngles[ii];
      SetDir( angle );
 #ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << ii << " Angle " << angle << G4endl; //GDEB
#endif
    double cosAng = cos(angle);
      double sinAng = sin(angle);
      if( angle < 90*CLHEP::deg ) {
	//----- SCAN ALONG Z AXIS AT NEGATIVE X
	sstep = fabs(theStepScan/cosAng);
	double posZ = theScanWorldMinZ + sstep;
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(testVerb) ) G4cout << "PF_XZ 0posZ " << posZ << " theScanWorldMaxZ " << theScanWorldMaxZ << " sstep " << sstep << G4endl; //GDEB
#endif
	for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	  //	  G4cout << " 0posZ " << posZ << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(theScanWorldMinX, thePosTransv, posZ), evt ); 
	}
	if( angle != 0*CLHEP::deg ) {	
	  //----- SCAN ALONG X AXIS AT NEGATIVE Z
	  sstep = fabs(theStepScan/sinAng);
	  double posX = theScanWorldMinX + sstep;
	  for( ; posX < theScanWorldMaxX; posX += sstep ) {
	    GenerateParticle( G4ThreeVector(posX, thePosTransv, theScanWorldMinZ), evt ); 
	  }
	}
      } else if( angle < 180*CLHEP::deg ) {
	//----- SCAN ALONG X AXIS AT NEGATIVE Z
	sstep = fabs(theStepScan/cosAng);
	double posX = theScanWorldMinX + sstep;
	//	G4cout << " 1-90posX " << posX << " theScanWorldMaxX " << theScanWorldMaxX << " sstep " << sstep << G4endl; //GDEB
	for( ; posX < theScanWorldMaxX; posX += sstep ) {
	  //	  G4cout << " 90posZ " << theScanWorldMinZ << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(posX, thePosTransv, theScanWorldMinZ), evt ); 
	}
	if( angle != 90*CLHEP::deg ) {	
	  //----- SCAN ALONG Z AXIS AT POSITIVE X
	  sstep = fabs(theStepScan/sinAng);
	  double posZ = theScanWorldMinZ + sstep;
	  for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	    GenerateParticle( G4ThreeVector(theScanWorldMaxX, thePosTransv, posZ), evt ); 
	  }
	}
      } else if( angle < 270*CLHEP::deg ) {
	//----- SCAN ALONG Z AXIS AT POSITIVE X
	sstep = fabs(theStepScan/cosAng);
	double posZ = theScanWorldMinZ + sstep;
	//	G4cout << " 0posZ " << posZ << G4endl; //GDEB
	for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	  //	  G4cout << " posZ " << posZ << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(theScanWorldMaxX, thePosTransv, posZ), evt ); 
	}
	if( angle != 180*CLHEP::deg ) {	
	  //----- SCAN ALONG X AXIS AT POSITIVE Z
	  sstep = fabs(theStepScan/sinAng);
	  double posX = theScanWorldMinX + sstep;
	  for( ; posX < theScanWorldMaxX; posX += sstep ) {
	    GenerateParticle( G4ThreeVector(posX, thePosTransv, theScanWorldMaxZ), evt ); 
	  }
	}
      } else { // < 360
	//----- SCAN ALONG X AXIS AT POSITIVE Z
	sstep = fabs(theStepScan/cosAng);
	double posX = theScanWorldMinX + sstep;
	//	G4cout << " 0posZ " << posZ << G4endl; //GDEB
	for( ; posX < theScanWorldMaxX; posX += sstep ) {
	  //	  G4cout << " posZ " << posZ << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(posX, thePosTransv, theScanWorldMaxZ), evt ); 
	}
	if( angle != 270*CLHEP::deg ) {	
	  //----- SCAN ALONG Z AXIS AT NEGATIVE X
	  sstep = fabs(theStepScan/sinAng);
	  double posZ = theScanWorldMinZ + sstep;
	  for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	    GenerateParticle( G4ThreeVector(theScanWorldMinX, thePosTransv, posZ), evt ); 
	  }
	}
      }
    }
        //---------------------------------------    
  } else if( thePlaneFig == PF_YZ ) {
    for( size_t ii = 0; ii < theAngles.size(); ii++ ) {
      G4double angle = theAngles[ii];
      SetDir( angle );
      double cosAng = cos(angle);
      double sinAng = sin(angle);
      if( angle < 90*CLHEP::deg ) {
	//----- SCAN ALONG Z AXIS AT NEGATIVE Y
	sstep = fabs(theStepScan/cosAng);
	double posZ = theScanWorldMinZ + sstep;
	//       	G4cout << " 0posZ " << posZ << G4endl; //GDEB
	for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	  //	  G4cout << " posZ " << posZ << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(thePosTransv, theScanWorldMinY, posZ), evt ); 
	}
	if( angle != 0*CLHEP::deg ) {	
	  //----- SCAN ALONG Y AXIS AT NEGATIVE Z
	  sstep = fabs(theStepScan/sinAng);
	  double posY = theScanWorldMinY + sstep;
	  for( ; posY < theScanWorldMaxY; posY += sstep ) {
	    GenerateParticle( G4ThreeVector(thePosTransv, posY, theScanWorldMinZ), evt ); 
	  }
	}
      } else if( angle < 180*CLHEP::deg ) {
	//----- SCAN ALONG Y AXIS AT NEGATIVE Z
	sstep = fabs(theStepScan/cosAng);
	double posY = theScanWorldMinY + sstep;
	//	G4cout << " 0posZ " << posZ << G4endl; //GDEB
	for( ; posY < theScanWorldMaxY; posY += sstep ) {
	  //	  G4cout << " posZ " << posZ << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(thePosTransv, posY, theScanWorldMinZ), evt ); 
	}
	if( angle != 90*CLHEP::deg ) {	
	  //----- SCAN ALONG Z AXIS AT POSITIVE Y
	  sstep = fabs(theStepScan/sinAng);
	  double posZ = theScanWorldMinZ + sstep;
	  for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	    GenerateParticle( G4ThreeVector(thePosTransv, theScanWorldMaxY, posZ), evt ); 
	  }
	}
      } else if( angle < 270*CLHEP::deg ) {
	//----- SCAN ALONG Z AXIS AT POSITIVE Y
	sstep = fabs(theStepScan/cosAng);
	double posZ = theScanWorldMinZ + sstep;
	//	G4cout << " 0posZ " << posZ << G4endl; //GDEB
	for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	  //	  G4cout << " posZ " << posZ << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(thePosTransv, theScanWorldMaxY, posZ), evt ); 
	}
	if( angle != 180*CLHEP::deg ) {	
	  //----- SCAN ALONG Y AXIS AT POSITIVE Z
	  sstep = fabs(theStepScan/sinAng);
	  double posY = theScanWorldMinY + sstep;
	  for( ; posY < theScanWorldMaxY; posY += sstep ) {
	    GenerateParticle( G4ThreeVector(thePosTransv, posY, theScanWorldMaxZ), evt ); 
	  }
	}
      } else { // < 360
	//----- SCAN ALONG Y AXIS AT POSITIVE Z
	sstep = fabs(theStepScan/cosAng);
	double posY = theScanWorldMinY + sstep;
	//	G4cout << " 0posZ " << posZ << G4endl; //GDEB
	for( ; posY < theScanWorldMaxY; posY += sstep ) {
	  //	  G4cout << " posZ " << posZ << G4endl; //GDEB
	  GenerateParticle( G4ThreeVector(thePosTransv, posY, theScanWorldMaxZ), evt ); 
	}
	if( angle != 270*CLHEP::deg ) {	
	  //----- SCAN ALONG Z AXIS AT NEGATIVE Y
	  sstep = fabs(theStepScan/sinAng);
	  double posZ = theScanWorldMinZ + sstep;
	  for( ; posZ < theScanWorldMaxZ; posZ += sstep ) {
	    GenerateParticle( G4ThreeVector(thePosTransv, theScanWorldMinY, posZ), evt ); 
	  }
	}
      }
    }
  }
#endif  
}
//------------------------------------------------------------------------
void GmGeneratorScanVis::UserSteppingAction(const G4Step* aStep)
{
#ifndef GAMOS_NO_ROOT
  if( aStep->GetPostStepPoint()->GetPhysicalVolume() ) {
    G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();
    G4String borderName;

    if( bColorByVolumeIndex ) {
      G4VPhysicalVolume* prePV = aStep->GetPreStepPoint()->GetPhysicalVolume();
      G4VPhysicalVolume* postPV = aStep->GetPostStepPoint()->GetPhysicalVolume();
      if( theColorByVolumeIndexType == CBVIT_LV ) {
	borderName = prePV->GetLogicalVolume()->GetName();
	  + ".-." +  postPV->GetLogicalVolume()->GetName();
      } else if( theColorByVolumeIndexType == CBVIT_PV ) {
	borderName = prePV->GetLogicalVolume()->GetName()
	  + ":" + GmGenUtils::itoa(prePV->GetCopyNo())
	  + ".-." +  postPV->GetLogicalVolume()->GetName()
	  + ":" + GmGenUtils::itoa(postPV->GetCopyNo());
      } else if( theColorByVolumeIndexType == CBVIT_T ) {
	borderName = theGeomUtils->GetG4TouchableLongName((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))
	  + ".-." + theGeomUtils->GetG4TouchableLongName((G4TouchableHistory*)(aStep->GetPostStepPoint()->GetTouchable()));
      }
    } else {
      G4String vol1Name = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
      G4String vol2Name = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
      if( vol1Name > vol2Name ) {
	borderName = vol2Name + ".-." + vol1Name;
      } else {
	borderName = vol1Name + ".-." + vol2Name;
      }
    }
      //-    G4cout << " BORDER NAME " << borderName << " " <<  (vol1Name > vol2Name) << G4endl; //GDEB
    switch (thePlaneFig) {
    case PF_XY:
      if( pos.x() >= theDrawMinH && pos.x() <= theDrawMaxH && pos.y() >= theDrawMinV && pos.y() <= theDrawMaxV ) {
	thePoints[borderName].push_back(G4ThreeVector(pos.x(),pos.y(),0.));
      }
      break;
    case PF_XZ:
      if( pos.x() >= theDrawMinH && pos.x() <= theDrawMaxH && pos.z() >= theDrawMinV && pos.z() <= theDrawMaxV ) {
	thePoints[borderName].push_back(G4ThreeVector(pos.x(),pos.z(),0.));
      }
      break;
    case PF_YZ:
      //      G4cout << " CHECK POS YZ " << pos.y() << " >= " << theDrawMinV << " && " << pos.y() << " <= " << theDrawMaxV <<" && " << pos.z()<< " >= " << theDrawMinZ << "&& " << pos.z() << " <= " << theDrawMaxZ <<G4endl;//GDEB
      if( pos.y() >= theDrawMinH && pos.y() <= theDrawMaxH && pos.z() >= theDrawMinV && pos.z() <= theDrawMaxV ) {
	thePoints[borderName].push_back(G4ThreeVector(pos.y(),pos.z(),0.));
      }
      break;
    }
    //    G4cout << "UserSteppingAction ADD POINT " << thePoints.size() << G4endl; //GDEB

    ColPoint pt;
    pt.x  = pos.x();
    pt.y  = pos.y();
    pt.z  = pos.z();
    strncpy(pt.tag, aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().c_str(), 64);
    
    fwrite(&pt, sizeof(ColPoint), 1, theFileOut);
  
  }
#endif
}


#ifndef GAMOS_NO_ROOT
//------------------------------------------------------------------------
void GmGeneratorScanVis::SetDir( G4double angle ) 
{
  switch (thePlaneFig ) {
  case PF_XY:
    theDir = G4ThreeVector(cos(angle),sin(angle),0.);
    break;
  case PF_XZ:
    theDir = G4ThreeVector(cos(angle),0.,sin(angle));
    break;
  case PF_YZ:
    theDir = G4ThreeVector(0.,cos(angle),sin(angle));
  }

}

//------------------------------------------------------------------------
void GmGeneratorScanVis::BeginOfRunAction(const G4Run* )
{
  SetColors();
   
  theGeomUtils = GmGeometryUtils::GetInstance();
  G4LogicalVolume* worldLV = theGeomUtils->GetTopLV();
  G4Box* world = static_cast<G4Box*>(worldLV->GetSolid());
  if( !world ) {
      G4Exception("GmGeneratorScanVis::BeginOfRunAction",
		  "",
		  FatalException,
		  (G4String("World has to be of type  Box, while it is ") + worldLV->GetSolid()->GetEntityType()).c_str());
  }
      
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  theScanWorldMinX = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMinX", -world->GetXHalfLength());
  theScanWorldMaxX = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMaxX", world->GetXHalfLength());
  theScanWorldMinY = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMinY", -world->GetYHalfLength());
  theScanWorldMaxY = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMaxY", world->GetYHalfLength());
  theScanWorldMinZ = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMinZ", -world->GetZHalfLength());
  theScanWorldMaxZ = parmgr->GetNumericValue("GmGeneratorScanVis:ScanWorldMaxZ", world->GetZHalfLength());
  
  theCanvas = new TCanvas;
  gStyle->SetOptStat(0);
  theCanvasNPixels = G4int(parmgr->GetNumericValue("GmGeneratorScanVis:NPixels", 500));
  theCanvas->SetCanvasSize(theCanvasNPixels,theCanvasNPixels);

  theMarkerStyle = G4int(parmgr->GetNumericValue("GmGeneratorScanVis:MarkerStyle", 1));
  theMarkerSize = parmgr->GetNumericValue("GmGeneratorScanVis:MarkerSize", 0.25);
  if( theMarkerSize < 0.25 ) {
    G4Exception("GmGeneratorScanVis::EndOfEventAction",
		"",
		JustWarning,
		"Too small marker size, it will be reset to 0.25. You may increase the NPixels to increase precision");
    theMarkerSize = 0.25;
  }

  switch (thePlaneFig) {
  case PF_XY:
    theHisto = new TH2F("","",100,theScanWorldMinX,theScanWorldMaxX,100,theScanWorldMinY,theScanWorldMaxY);
    thePosTransv = theScanWorldMinZ;
    theWorldNDigits = log10(fabs(std::max(theScanWorldMinZ,theScanWorldMaxZ)));
    theDrawMinH = theScanWorldMinX;
    theDrawMaxH = theScanWorldMaxX;
    theDrawMinV = theScanWorldMinY;
    theDrawMaxV = theScanWorldMaxY;
    break;
  case PF_XZ:
    theHisto = new TH2F("","",100,theScanWorldMinX,theScanWorldMaxX,100,theScanWorldMinZ,theScanWorldMaxZ);
    thePosTransv = theScanWorldMinY;
    theWorldNDigits = log10(fabs(std::max(theScanWorldMinY,theScanWorldMaxY)));
    theDrawMinH = theScanWorldMinX;
    theDrawMaxH = theScanWorldMaxX;
    theDrawMinV = theScanWorldMinZ;
    theDrawMaxV = theScanWorldMaxZ;
    break;
  case PF_YZ:
    theHisto = new TH2F("","",100,theScanWorldMinY,theScanWorldMaxY,100,theScanWorldMinZ,theScanWorldMaxZ);
    thePosTransv = theScanWorldMinX;
    theWorldNDigits = log10(fabs(std::max(theScanWorldMinX,theScanWorldMaxX)));
    theDrawMinH = theScanWorldMinY;
    theDrawMaxH = theScanWorldMaxY;
    theDrawMinV = theScanWorldMinZ;
    theDrawMaxV = theScanWorldMaxZ;
    break;
  }
  thePosTransv -= theStepFig;
  //  G4cout << " INITIAL thePosTransv " << thePosTransv << " " << theStepFig << G4endl; //GDEB
  
  G4int nd = log10(theStepFig);
  if( nd > 0 ) nd = 0;
  if( nd < 0 ) nd--; //take into account "."
  theNameNDigits = theWorldNDigits - nd;

  G4String fOutName = parmgr->GetStringValue("GmGeneratorScanVis:FileOutName","scanVis.out");
  theFileOut = fopen(fOutName.c_str(), "wb");

  bColorByVolumeIndex = false;
  theColorByVolumeIndexFN = parmgr->GetStringValue("GmGeneratorScanVis:ColorByVolumeIndexFileName","");
  if( theColorByVolumeIndexFN != "" ) {
    bColorByVolumeIndex = true;

    G4String colorByVolumeIndexTypeStr = parmgr->GetStringValue("GmGeneratorScanVis:ColorByVolumeIndexType","LogicalVolume");
    if( colorByVolumeIndexTypeStr == "LogicalVolume " ) {
      theColorByVolumeIndexType = CBVIT_LV;
    } else if( colorByVolumeIndexTypeStr == "PhysicalVolume " ) {
      theColorByVolumeIndexType = CBVIT_PV;
    } else if( colorByVolumeIndexTypeStr == "Touchable " ) {
      theColorByVolumeIndexType = CBVIT_T;
    } else {
      G4Exception("GmGeneratorScanVis::GmGeneratorScanVis",
		  "",
		  FatalException,
		  ("Parameter GmGeneratorScanVis:ColorByVolumeIndexType must be: LogicalVolume, PhysicalVolume or Touchable, while it is: " + colorByVolumeIndexTypeStr).c_str());
    }

    std::vector<G4String> wl;
    GmFileIn fin = GmFileIn::GetInstance(theColorByVolumeIndexFN);
    for(;;) {
      if( ! fin.GetWordsInLine(wl) ) break;
      if( wl.size() < 2 ) {
	G4Exception("GmGeneratorScanVis::GmGeneratorScanVis",
		    "",
		    FatalException,
		    ("Error reading file " + theColorByVolumeIndexFN + " lines must have two columns ").c_str());
      }
      theColorVolumeIdx[wl[0]] = GmGenUtils::GetInt(wl[1]);
    }
  }

  bDrawBorderNames = G4bool(parmgr->GetNumericValue("GmGeneratorScanVis:DrawBorderNames",0));

}

//------------------------------------------------------------------------
void GmGeneratorScanVis::EndOfRunAction(const G4Run* )
{
  fclose(theFileOut);
}

//------------------------------------------------------------------------
G4bool GmGeneratorScanVis::IncrementPosTransv()
{
  thePosTransv += theStepFig;
  G4bool bOK = true;
  switch (thePlaneFig) {
  case PF_XY:
    if( thePosTransv > theScanWorldMaxZ ) {
      bOK = false;
    }
    break;
  case PF_XZ:
    if( thePosTransv > theScanWorldMaxY ) {
      bOK = false;
    }
    break;
  case PF_YZ:
    if( thePosTransv > theScanWorldMaxX ) {
      bOK = false;
    }
    break;
  }

  if( !bOK ) {
    G4Exception("GmGeneratorScanVis::IncrementPosTransv",
		"",
		JustWarning,
		"Too many events, run will be stopped");
    G4RunManager::GetRunManager()->AbortRun();  //  /abortrun
  }

  return bOK;
}

//------------------------------------------------------------------------
void GmGeneratorScanVis::EndOfEventAction(const G4Event* event)
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(warningVerb) ) G4cout << "EndOfEventAction " << thePosTransv << " : " << thePoints.size() << G4endl; 
#endif

  if( thePoints.size() == 0 ) {
    G4Exception("GmGeneratorScanVis",
		"GSV001",
		JustWarning,
		"No intersection point found, please check your dimensions");
    return;
  }
  
  //----- DRAW FIGURE
  if( !bPrintAllTogether || event->GetEventID() == 0 ) {
    theCanvas->Draw();
    theHisto->Draw();
  }
  G4double isoc = 300;
  TLine* line = new TLine(0.,0., 0., isoc);
  line->Draw();
  TLine* line2 = new TLine(0.,0., 25, isoc);
  line2->Draw();
  TLine* line3 = new TLine(0.,0., -25, isoc);
  line3->Draw();
  std::map<G4String, std::vector<G4ThreeVector> >::const_iterator itep;
  G4int icol = 0;
  std::map<G4String,EColor> colorMapUsed;
  
  for( itep = thePoints.begin(); itep != thePoints.end(); itep++, icol++ ) {
    G4String borderName = (*itep).first;
    EColor color = kBlack;
    if( bColorByVolumeIndex ) {
      /*      std::map<G4String,EColor>::const_iterator itec = theColorMap.find(borderName);
      if( itec == theColorMap.end() ) {
	theColorMap[borderName] = GetColor( theColorMap.size() );
	itec = theColorMap.find(borderName);
      }
      color = (*itec).second;
      */
    } else {
      color = GetColor(icol);
    } 
    colorMapUsed[borderName] = color;
    std::vector<G4ThreeVector> points = (*itep).second;
    
    for( size_t ii = 0; ii < points.size(); ii++ ) {
      TMarker* mark = new TMarker(points[ii].x(),points[ii].y(),theMarkerStyle);
      mark->SetMarkerSize(theMarkerSize);
      mark->SetMarkerColor(color);
      mark->Draw();
      //  G4cout << " DRAW MARKER " <<mark->GetX() << " " << mark->GetY() << G4endl;//GDEB 
      //    delete mark;
    }
  }
  //---- Draw border names
  if( bDrawBorderNames ) {
    double XPos = theDrawMinH+(theDrawMaxH-theDrawMinH)*0.05;
    double YPos = theDrawMaxV-(theDrawMaxV-theDrawMinV)*0.02;
    //G4cout << " 0TEXT POS  " << XPos << "," << YPos << " " << theDrawMinH << " " << theDrawMaxV << G4endl; //GDEB
    UInt_t tx = 1;
    UInt_t ty = 1;
    UInt_t txPrev = 0;
    UInt_t tyPrev = 0;
    std::map<G4String,EColor>::const_iterator itec;
    for( itec = colorMapUsed.begin(); itec != colorMapUsed.end(); itec++ ) {
      XPos += (txPrev/0.75)*(theDrawMaxH-theDrawMinH)/theCanvasNPixels;
      YPos += 0.;
      TText* text = new TText( XPos, YPos, (*itec).first );
      text->SetTextColor((*itec).second);
      text->SetTextSize(0.015);
      text->GetTextExtent(tx,ty,(*itec).first);
      double XPosEnd = XPos + (tx/0.75)*(theDrawMaxH-theDrawMinH)/theCanvasNPixels;
      if( XPosEnd > theDrawMaxH ) {
	XPos = theDrawMinH+(theDrawMaxH-theDrawMinH)*0.05;
	YPos -= (ty/0.8)*(theDrawMaxV-theDrawMinV)/theCanvasNPixels;
	//      G4cout << XPosEnd << " TEXT POS CORR " << XPos << "," << YPos << " " << tx << " * " << theCanvasNPixels << " / ( " << theDrawMaxV << " - " << theDrawMinV<< " " << YPos << G4endl; //GDEB
      }
      text->SetX(XPos);
      text->SetY(YPos);
      //    G4cout << " TEXT POS  " << XPos << "," << YPos << " " << tx << " * " << theCanvasNPixels << " / ( " << theDrawMaxV << " - " << theDrawMinV<< " " << YPos << G4endl; //GDEB
      
      text->Draw();
      text->GetTextExtent(txPrev,tyPrev,(*itec).first.c_str());
      //    G4cout << " TEXT EXTENT " << (*itec).first << " " << tx << " " << ty << " : " << (*itec).first.length() << G4endl;//GDEB
    }
  }
  
  G4String posTransvStr = GmGenUtils::ftoa(thePosTransv);
  G4int posNDigits = log10(fabs(thePosTransv));
  if( thePosTransv == 0 ) posNDigits = 1;
  if( posNDigits < 0 ) posNDigits = 0;
  //  G4cout << " posNDigits " << posNDigits << " " << theWorldNDigits << G4endl; //GDEB
  for( int ii = posNDigits; ii < theWorldNDigits; ii++ ) {
    if( thePosTransv > 0. ) {
      posTransvStr = "0" + posTransvStr;
    } else {
      posTransvStr = "-0" + posTransvStr.substr(1,posTransvStr.length());
    }
  }
  int nDigits = theNameNDigits - int((GmGenUtils::sign(thePosTransv)-1)/2) +1;
  //  G4cout << "NDIG " << nDigits << " = " << theNameNDigits << " - " << int(GmGenUtils::sign(thePosTransv)-1)/2 << " LENGTH " << posTransvStr.length() << G4endl; //GDEB
  
  if( int(posTransvStr.length()) < nDigits ) {
    if( posTransvStr.find(".") == std::string::npos ) posTransvStr += ".";
    G4int nptd = posTransvStr.length();
    for( int ii = 0; ii < nDigits-nptd; ii++ ) {
      posTransvStr += "0";
    }
  }
  //  G4cout << " theWorldNDigits " << theWorldNDigits << " " << posNDigits << " : " << posTransvStr << G4endl;//GDEB

  G4String figName = "ScanVis"+thePlaneFigName+"."+posTransvStr+".gif";
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " EndOfEventAction " << figName << G4endl;
#endif
  if( !bPrintAllTogether || event->GetEventID() == G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed()-1 ) {
    theCanvas->Print(figName.c_str());
  }
  
  thePoints.clear();
}
  
//------------------------------------------------------------------------
void GmGeneratorScanVis::GenerateParticle(G4ThreeVector pos, G4Event* evt )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << "  GmGeneratorScanVis::GenerateParticle " << pos << " dir " << theDir << G4endl;
#endif
  //  G4cout << " GenerateParticle pos " << pos << G4endl; //GDEB
  G4PrimaryVertex* vtx = new G4PrimaryVertex( pos, 0. );
  evt->AddPrimaryVertex( vtx );
  G4ParticleDefinition* partDef = G4Geantino::Geantino();
  G4PrimaryParticle* particle = new G4PrimaryParticle( partDef, theDir.x(), theDir.y(), theDir.z());

  vtx->SetPrimary(particle);

}

 
//--------------------------------------------------------------------------
void GmGeneratorScanVis::SetColors()
{
  theGoodColors[0] = EColor(kBlack+0);
  theGoodColors[1] = EColor(kBlack+1);
  theGoodColors[2] = EColor(kBlack+2);
  theGoodColors[3] = EColor(kBlack+3);
  theGoodColors[4] = EColor(kBlack+5);
  theGoodColors[5] = EColor(kBlack+6);
  theGoodColors[6] = EColor(kBlack+7);
  theGoodColors[7] = EColor(kBlack+8);
  theGoodColors[8] = EColor(kBlack+10);
  theGoodColors[9] = EColor(kBlack+11);
  theGoodColors[10] = EColor(kBlack+43);
  theGoodColors[11] = EColor(kBlack+19);
  theGoodColors[12] = EColor(kBlack+27);
  theGoodColors[13] = EColor(kBlack+28);
  theGoodColors[14] = EColor(kBlack+31);
  theGoodColors[15] = EColor(kBlack+15);
  theGoodColors[16] = EColor(kBlack+34);
  theGoodColors[17] = EColor(kBlack+40);
  theGoodColors[18] = EColor(kBlack+37);
  theGoodColors[19] = EColor(kBlack+23);
  theGoodColors[20] = EColor(kBlack+41);
  theGoodColors[21] = EColor(kBlack+32);
  theGoodColors[22] = EColor(kBlack+45);
  theGoodColors[23] = EColor(kBlack+29);
  theGoodColors[24] = EColor(kBlack+38);
  theGoodColors[25] = EColor(kBlack+47);
}

//--------------------------------------------------------------------------
EColor GmGeneratorScanVis::GetColor( int icol )
{
  icol = icol%25;

  return theGoodColors[icol];
}
#endif
