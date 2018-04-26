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
#include "GmPSSurfaceFlux.hh"
#include "GmScoringSurfaceBox.hh"
#include "GmScoringSurfaceTubs.hh"
#include "GmScoringSurfaceSphere.hh"
#include "GmScoringSurfaceOrb.hh"

#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4StepStatus.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4GeometryTolerance.hh"

//-----------------------------------------------------------------------
GmPSSurfaceFlux::GmPSSurfaceFlux(G4String name)
 :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("");
  
  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();

  G4String direction = paramMgr->GetStringValue(primitiveName+":Direction","InOut");
  if( direction == "InOut" ) {
    theDirection = fFlux_InOut;
  } else if( direction == "In" ) {
    theDirection = fFlux_In;
  } else if( direction == "Out" ) {
    theDirection = fFlux_Out;
  } else {
    G4Exception("GmPSSurfaceFlux::GmPSSurfaceFlux",
		"Invalid direction supplied",
		FatalErrorInArgument,
		("Valid directions are  In / Out / InOut , you are using "+direction).c_str());
  }
  
  theSurfaces = paramMgr->GetVStringValue(primitiveName+":Surfaces",theSurfaces);
  if( theSurfaces.size() == 0 ) {
    G4Exception("GmPSSurfaceFlux::GmPSSurfaceFlux",
		"No surface type supplied",
		FatalErrorInArgument,
		("Please use coCLHEP::mmand /gamos/setParam "+primitiveName + ":Surfaces SURFACE_1 SURFACE_2 ... SURFACE_N").c_str());
    
  }

  bArea = G4bool(paramMgr->GetNumericValue(primitiveName+":DivideByArea",1));
  if( bArea ) {
    theUnit = CLHEP::mm*CLHEP::mm/(CLHEP::cm*CLHEP::cm);
    theUnitName = G4String("CLHEP::cm-2");
  }

  bAngleFactor = G4bool(paramMgr->GetNumericValue(primitiveName+":DivideByAngle",1));
  
}
  

//-----------------------------------------------------------------------
G4bool GmPSSurfaceFlux::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  G4VSolid* solid = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetSolid();
  std::map<G4VSolid*, GmVScoringSurfaceSolid*>::const_iterator ite = theScoringSolids.find(solid);
  
  if( ite == theScoringSolids.end() ) {
    if( solid->GetEntityType() == "G4Box" ) {
      theScoringSolid = new GmScoringSurfaceBox(solid);
    } else if( solid->GetEntityType() == "G4Tubs" ) {
      theScoringSolid = new GmScoringSurfaceTubs(solid);
    } else if( solid->GetEntityType() == "G4Sphere" ) {
      theScoringSolid = new GmScoringSurfaceSphere(solid);
    } else if( solid->GetEntityType() == "G4Orb" ) {
      theScoringSolid = new GmScoringSurfaceOrb(solid);
    } else {
      G4Exception("GmPSSurfaceFlux::ProcessHits",
		  "Invalid solid type",
		  FatalErrorInArgument,
		  "Solid types can be BOX TUBS SPHERE ORB");
    }   
    theScoringSolids[solid] = theScoringSolid;
    theScoringSolid->SetSurfaces( theSurfaces );
    if( bArea ) theScoringSolid->ComputeArea();
  } else {
    theScoringSolid = (*ite).second;
  }
 

  // one step may enter and exit volume, so check boths
  if( theDirection == fFlux_InOut ) {
    G4bool bpre = ProcessHits( aStep->GetPreStepPoint(), aStep, fFlux_In ) ;
    G4bool bpost = ProcessHits( aStep->GetPostStepPoint(), aStep, fFlux_Out ) ;
    return (bpre || bpost);
      //ProcessHits( aStep->GetPreStepPoint(), aStep, fFlux_In ) 
      //	    || ProcessHits( aStep->GetPostStepPoint(), aStep, fFlux_Out ) );
  } else if( theDirection == fFlux_In ) {
    return ProcessHits( aStep->GetPreStepPoint(), aStep, fFlux_In );
  } else if( theDirection == fFlux_Out ) {
    return ProcessHits( aStep->GetPostStepPoint(), aStep, fFlux_Out );
  }

  return TRUE;

}

//-----------------------------------------------------------------------
G4bool GmPSSurfaceFlux::ProcessHits(G4StepPoint* aStepPoint, G4Step* aStep, G4int aDirFlag )
{
  G4int dirFlag = -1;
  G4bool bFlux = theScoringSolid->IsSelectedSurface( aStepPoint, aStep );
  if( bFlux ) dirFlag = aDirFlag;

  if ( dirFlag > 0 ) {
    if ( theDirection == fFlux_InOut || theDirection == dirFlag ){

      G4StepPoint* stepPoint=0;
      if ( dirFlag == fFlux_In ){
	stepPoint = aStep->GetPreStepPoint();
      }else if ( dirFlag == fFlux_Out ){
	stepPoint = aStep->GetPostStepPoint();
      }else{
	return FALSE;
      }

      G4double flux  = 1;
      if( bArea ) {     
	G4double area = theScoringSolid->GetArea();
	flux /= area;
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmPSSurfaceFlux::ProcessHits flux after area= " << flux << " area " << area << G4endl;
#endif     

      }
      if( bAngleFactor ) {
	G4double angleFactor = theScoringSolid->GetAngleFactor( stepPoint );
	flux /= angleFactor;
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmPSSurfaceFlux::ProcessHits flux after angleFactor= " << flux << " angleFactor " << angleFactor << G4endl;
#endif     
      }
      
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << " GmPSSurfaceFlux::ProcessHits flux= " << flux << G4endl;
#endif     
      FillScorer( aStep, flux, stepPoint->GetWeight() );
    }
  }

  return TRUE;
}
