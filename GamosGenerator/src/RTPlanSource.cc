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
#include "RTPlanSource.hh"
#include "RTPlanBeam.hh"
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

//-----------------------------------------------------------------------
RTPlanSource::RTPlanSource( const G4String& name, const G4String& partName): RTVPlanSource( name )
{
  theType = "RTPlanSource";

  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  thePlanFileName = parMgr->GetStringValue(theName+":PlanFileNamePrefix","RTPlan"); 
  theBeamFileName = parMgr->GetStringValue(theName+":BeamFileNamePrefix","RTPlanBeam"); 
  theCPFileName = parMgr->GetStringValue(theName+":ControlPointFileNamePrefix","RTPlanControlPoint");

  /*G4String posDist = GmParameterMgr::GetInstance()->GetStringValue(theName+":PositionDist","DiscGaussian");
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
void RTPlanSource::ReadPlanData()
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
	 G4Exception("RTPlanSource::ReadPlanData",
		     "",
		     es,
		     ("NumberOfBeams is not 0 in fraction " + GmGenUtils::itoa(ii)).c_str());
      }
		    
      //      G4cout << "RTPlanSource::ReadPlanData() nBeams " << nBeams << G4endl; //GDEB      
      G4bool bOneFile = false;
      for( int ib = 1; ib <= nBeams; ib++ ) {
	std::ifstream fint(theBeamFileName + "_" +GmGenUtils::itoa(ib));
	if( fint.good() ) {
	  theBeams.push_back( new RTPlanBeam( theBeamFileName + "_" +GmGenUtils::itoa(ib), theCPFileName ));
	  bOneFile = true;
	} else {
	  G4Exception("RTPlanSource::RTPlanSource",
		  "",
		  JustWarning,
		  ("Beam file not found: " + theBeamFileName
		   + "_" +GmGenUtils::itoa(ib)).c_str());
	}
	if( !bOneFile ) {
	  G4Exception("RTPlanSource::RTPlanSource",
		      "",
		      JustWarning,
		      ("No Beam file found with prefix: " + theBeamFileName
		       + "_" +GmGenUtils::itoa(ib)).c_str());
	  
	}
      }
    } else {
      if( ii == 1 )  {
	G4Exception("RTPlanSource::ReadPlanData",
		    "",
		    FatalException,
		    "No FractionGroup number 1, please contact GAMOS authors");
      } else {
	break;
      }
    }
  }

  if( theBeams.size() == 0 ) {
    G4Exception("RTPlanSource::ReadPlanData",
		"",
		FatalException,
		"No BEAM defined, check your data files");
  }

}


/*//-------------------------------------------------------------
G4double RTPlanSource::GetSpotSize( ScanSpotData )
{
  if( !bDiscGaussian ) {
    return theDiscSigma;
  } else {
    return theDiscRadius;
  }
}
*/

//-------------------------------------------------------------
void RTPlanSource::SetSpotSize( GmVGenerDistPosition2D* , const ScanSpotData& )
{
  return;
  // do nothing (taken from distribution defined in input script)
  
}

//-------------------------------------------------------------
void RTPlanSource::PlotScanSpots()
{
  return;

}
