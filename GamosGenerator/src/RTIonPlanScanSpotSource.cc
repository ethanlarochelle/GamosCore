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
#include "RTIonPlanScanSpotSource.hh"
#include "RTIonPlanBeam.hh"
#include "G4PrimaryVertex.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "GmGenerDistTimeConstant.hh"
#include "GmGenerDistDirectionConst.hh"
#include "GmGenerDistDirectionCone.hh"
#include "GmGenerDistPositionDisc.hh" 
#include "GmGenerDistPositionDiscGaussian.hh" 
#include "GmGenerDistEnergyConstant.hh"
#include "GmGenerDistEnergyGaussian.hh"
#include "GmGeneratorMgr.hh"

#include "G4PrimaryParticle.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"

#include "CLHEP/Random/RandFlat.h"
#ifndef GAMOS_NO_ROOT
#include "TColor.h"
#endif

//-----------------------------------------------------------------------
RTIonPlanScanSpotSource::RTIonPlanScanSpotSource( const G4String& name, const G4String& partName): RTVPlanSource( name )
{
  theType = "RTIonPlanScanSpotSource";

  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  thePlanFileName = parMgr->GetStringValue(theName+":PlanFileNamePrefix","RTIonPlan"); 
  theBeamFileName = parMgr->GetStringValue(theName+":BeamFileNamePrefix","RTIonPlanBeam"); 
  theCPFileName = parMgr->GetStringValue(theName+":ControlPointFileNamePrefix","RTIonPlanControlPoint");

  /*  G4String posDist = parMgr->GetStringValue(theName+":PositionDist","Disc");
  if( posDist == "Disc" ) {
    bDiscGaussian = false;
  } else {
    bDiscGaussian = true;
    } */
  
  InitializeDistributions(partName);
  
  iCurrentBeam = 0;
  theCurrentMetersetEvtID = 0;

  ReadPlanData();

}


//---------------------------------------------------------------------
void RTIonPlanScanSpotSource::ReadPlanData()
{
  GmFileIn fin = GmFileIn::GetInstance(thePlanFileName);
  ReadParams(fin);

  G4cout << "RTPlanSource::ReadPlanData() N PARAM " << theParams.size() << G4endl; //GDEB
  int ii = 1;
  for(;;ii++) { //  Check if various FractionGroup's exists (very rare)
    G4String fgstr = "FractionGroup_"+GmGenUtils::itoa(ii);
    if( ParamExists(fgstr+"_Number") ) { 
      G4int nBeams = GetParam(fgstr+"_NumberOfBeams",1);
      if( nBeams == 0 ) {
	 G4ExceptionSeverity es;
	 if( ii == 1 ) {
	   es = FatalException;
	 } else {
	   es = JustWarning;
	 }
	 G4Exception("RTIonPlanScanSpotSource::ReadPlanData",
		     "",
		     es,
		     ("NumberOfBeams is not 0 in fraction " + GmGenUtils::itoa(ii)).c_str());
      }
		    
      //      G4cout << "RTVPlanSource::ReadPlanData() nBeams " << nBeams << G4endl; //GDEB
      G4double beamPosZ = GmParameterMgr::GetInstance()->GetNumericValue(theName+":BeamPositionZ",0.);
      for( int ib = 1; ib <= nBeams; ib++ ) {
	std::ifstream fint( theBeamFileName + "_" +GmGenUtils::itoa(ib));
	if( fint.good() ) {
	  RTIonPlanBeam * beam = new RTIonPlanBeam( theBeamFileName + "_" +GmGenUtils::itoa(ib), theCPFileName );
	  theBeams.push_back( beam );
	  beam->SetBeamPosZ( beamPosZ );
	}
      }
    } else {
      if( ii == 1 )  {
	G4Exception("RTIonPlanScanSpotSource::ReadPlanData",
		    "",
		    FatalException,
		    "No FractionGroup number 1, please contact GAMOS authors");
      } else {
	break;
      }
    }
  }

  if( theBeams.size() == 0 ) {
    G4Exception("RTIonPlanScanSpotSource::ReadPlanData",
		"",
		FatalException,
		"No BEAM defined, check your data files");
  }
  
}

//-------------------------------------------------------------
G4double RTIonPlanScanSpotSource::GetSpotSize( ScanSpotData spData )
{
  return spData.SpotSize/2.;
}

//-------------------------------------------------------------
void RTIonPlanScanSpotSource::SetSpotSize( GmVGenerDistPosition2D* positionDistribution2D, const ScanSpotData& spdata )
{
  if(static_cast<GmGenerDistPositionDisc*>(positionDistribution2D) ) {
    static_cast<GmGenerDistPositionDisc*>(thePositionDistribution2D)->SetRadius(GetSpotSize(spdata));
  } else if(static_cast<GmGenerDistPositionDiscGaussian*>(positionDistribution2D) ) {
    static_cast<GmGenerDistPositionDiscGaussian*>(thePositionDistribution2D)->SetSigma(GetSpotSize(spdata));
  }

  thePositionDistribution2D->SetCentre( G4ThreeVector( spdata.PositionX, spdata.PositionY,spdata.PositionZ ));

}

//-------------------------------------------------------------
void RTIonPlanScanSpotSource::PlotScanSpots()
{
#ifndef GAMOS_NO_ROOT
  G4double minX = DBL_MAX;
  G4double maxX = -DBL_MAX;
  G4double minY = DBL_MAX;
  G4double maxY = -DBL_MAX;
  G4double maxMS = -DBL_MAX;
  G4double minmaxX = 0.; 
  G4double minmaxY = 0.; 
  for( size_t iib = 0; iib < theBeams.size(); iib++ ) {
    std::vector<RTVPlanControlPoint*> CPs = theBeams[iib]->GetControlPoints();
    for( size_t iicp = 0; iicp < CPs.size(); iicp++ ) {
      RTIonPlanControlPoint* CP = static_cast<RTIonPlanControlPoint*>(CPs[iicp]);
      std::vector<G4double> posXs = CP->GetPosXs();
      std::vector<G4double> posYs = CP->GetPosYs();
      std::vector<G4double> metersets = CP->GetMetersets();
      G4double spotSize = CP->GetScanningSpotSize();
      for( size_t iip = 0; iip < posXs.size(); iip++ ) {
	minX = std::min(minX,posXs[iip]-spotSize);
	maxX = std::max(maxX,posXs[iip]+spotSize);
	minY = std::min(minX,posXs[iip]-spotSize);
	maxY = std::max(maxX,posXs[iip]+spotSize);
	maxMS = std::max(maxMS,metersets[iip]);
      }
    }
  }
  minmaxX = std::max(fabs(minX),fabs(maxX));
  minmaxY = std::max(fabs(minY),fabs(maxY));

  for( G4int ii = 0; ii < 100; ii++ ) {
    float satur = ii/100.;
    new TColor( 1000+ii, satur, 0, 0 );
  }

  for( size_t iib = 0; iib < theBeams.size(); iib++ ) {
    std::vector<RTVPlanControlPoint*> CPs = theBeams[iib]->GetControlPoints();
    for( size_t iicp = 0; iicp < CPs.size(); iicp++ ) {
      CPs[iicp]->PlotScanSpots(minmaxX,minmaxY,maxMS);
    }
  }

#endif
}
