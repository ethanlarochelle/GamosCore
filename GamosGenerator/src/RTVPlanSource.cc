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
#include "RTVPlanSource.hh"
#include "RTIonPlanBeam.hh"
#include "GmGenerDistTimeConstant.hh"
#include "GmGenerDistDirectionConst.hh"
#include "GmGenerDistDirectionCone.hh"
#include "GmGenerDistDirectionCone2D.hh"
#include "GmGenerDistPositionDisc.hh" 
#include "GmGenerDistPositionDiscGaussian.hh" 
#include "GmGenerDistEnergyConstant.hh"
#include "GmGenerDistEnergyGaussian.hh"
#include "GmGeneratorMgr.hh"
#include "RTPlanMessenger.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmVModule.hh"
#include "GamosCore/GamosGeometry/include/GmModuleJaws.hh"
#include "GamosCore/GamosGeometry/include/GmModuleMLC.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmModuleMgr.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosReadDICOM/include/GmReadPhantomGeometry.hh"
#include "GamosCore/GamosRunManager/include/GmRunManager.hh"

#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4tgrVolumeMgr.hh"
#include "G4tgrVolume.hh"
#include "G4tgrPlace.hh"
#include "G4tgrUtils.hh"
#include "G4GeometryManager.hh"
#include "G4VVisManager.hh"
#include "G4Box.hh"
#include "G4UImanager.hh"

#include "CLHEP/Random/RandFlat.h"

//-----------------------------------------------------------------------
RTVPlanSource::RTVPlanSource( const G4String& name): GmParticleSource( name )
{
  new RTPlanMessenger(this);

  bInitialized = false;
  thePositionDistribution = 0;
  theDirectionDistribution = 0;
  theEnergyDistribution = 0;
  theTimeDistribution = 0;
  
}

//-----------------------------------------------------------------------
void RTVPlanSource::InitializeDistributions( const G4String& partName)
{
  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  theParticleDef = partTable->FindParticle( partName);
  if( !theParticleDef ) {
    theParticleDef = (G4ParticleDefinition*)(GmGetParticleMgr::GetInstance()->CreateIon(partName));

  }

  if( !theParticleDef ) {
    G4Exception("RTVPlanSource::RTVPlanSource",
		"Wrong argument",
		FatalErrorInArgument,
		G4String(GetName() + "  particle does not exist: " + partName).c_str());
  }

  theTimeDistribution = new GmGenerDistTimeConstant;
  theActivity = 1.*CLHEP::becquerel;
  thePositionDistribution2D = 0;  
  if( !thePositionDistribution ) {
    thePositionDistribution = new GmGenerDistPositionDisc;
    static_cast<GmGenerDistPositionDisc*>(thePositionDistribution)->SetRadius(1.);
    G4Exception("RTVPlanSource::RTVPlanSource",
		"",
		JustWarning,
		G4String(GetName() + " No position distribution defined, using GmGenerDistPositionDisc with radius=SacnSpotSize/2.").c_str());    

  }
  thePositionDistribution2D = static_cast<GmVGenerDistPosition2D*>(thePositionDistribution);
  if( !thePositionDistribution2D ) {
    G4Exception("RTVPlanSource::GenerateVertex",
		  "",
		  JustWarning,
		  ("Position distribution must be a 2D one (Disc, DiscGaussian, Square, Rectangle) while it is of type " + thePositionDistribution->GetName()).c_str() );
  }
  if( !theDirectionDistribution ) {
    theDirectionDistribution = new GmGenerDistDirectionConst;
    std::vector<G4String> paramsD;
    paramsD.push_back("0.");
    paramsD.push_back("0.");
    paramsD.push_back("-1.");
    theDirectionDistribution->SetParams( paramsD );
    G4Exception("RTVPlanSource::RTVPlanSource",
		"",
		JustWarning,
		G4String(GetName() + " No direction distribution defined, using GmGenerDistDirectionConst with direction (0,0,-1)").c_str());    
  }
  
  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  theEnergySigma = parMgr->GetNumericValue(theName+":EnergySigma",0.);
  if( theEnergySigma == 0. ) {
    bEGauss = false;
    //dummy distribution, not used
    theEnergyDistribution = new GmGenerDistEnergyConstant;
  } else {
    bEGauss = true;
    theEnergyDistribution = new GmGenerDistEnergyGaussian;
  }

  CheckDistributionsExist();

  bCoordSystemPatient = G4bool(parMgr->GetNumericValue(theName+":CoordinateSystemPatient",1));
  
  theNEventsPerMSUnit = parMgr->GetNumericValue(theName+":NEventsPerMSUnit",-1);
  theMSUnitsToSkip = parMgr->GetNumericValue(theName+":MSUnitsToSkip",0.);
  theNEventsToSkip = G4int(parMgr->GetNumericValue(theName+":NEventsToSkip",0));
  if( theMSUnitsToSkip != 0 && theNEventsToSkip != 0 ) {
    G4Exception("RTVPlanSource",
		"",
		FatalException,
		"It is not allowed to define both parameters: theMSUnitsToSkip && theNEventsToSkip");
  }
  iCurrentBeam = 0;
  theCurrentMetersetEvtID = 0;

  theG4RunManager = G4RunManager::GetRunManager();
  
}


//-------------------------------------------------------------
void RTVPlanSource::CalculateMetersetEvts()
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << "@@@@  RTVPlanSource::CalculateMetersetEvts() N BEAMS " << theBeams.size() << G4endl;
#endif
  //----- Calculate accumulated metersets
  G4double CPAccumMS = 0.;
  
  G4double beamAccumMS = 0.;
  for( size_t iib = 0; iib < theBeams.size(); iib++ ) {
    std::vector<RTVPlanControlPoint*> CPs = theBeams[iib]->GetControlPoints();
    G4double beamMS = 0.;
    G4double beamCMW = theBeams[iib]->GetParam("FinalCumulativeMetersetWeight",1);
    if( theBeams[iib]->ParamExists("BeamMeterset") ) {
      beamMS = theBeams[iib]->GetParam("BeamMeterset",0);
    } else {
      beamMS = beamCMW;
    }
    beamAccumMS += beamMS*beamCMW;

#ifndef GAMOS_NO_VERBOSE
if( GenerVerb(debugVerb) ) G4cout << "ACCUMULATING BEAM METERSETS " << iib << " = " << beamAccumMS << " <- " << theBeams[iib]->GetParam("FinalCumulativeMetersetWeight",1) << " NCPS " << CPs.size() << G4endl;
#endif
    for( size_t iicp = 0; iicp < CPs.size(); iicp++ ) {
      std::vector<G4double> meterSets = CPs[iicp]->GetMetersets();
      G4double numberOfPaintings = CPs[iicp]->GetNumberOfPaintings();
      //      G4cout << iib << " " << iicp << " nPaintins " << numberOfPaintings << G4endl; //GDEB
      for( size_t iims = 0; iims < meterSets.size(); iims++ ) {
	CPAccumMS += meterSets[iims]*numberOfPaintings;
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(testVerb) ) G4cout << "ACCUMULATING CONTROL POINT METERSETS " << iib << " : " << iicp << " : " << iims << " = " << CPAccumMS << " <- " << meterSets[iims] << G4endl; 
#endif
      }
    }
  }

  if( fabs(CPAccumMS - beamAccumMS) > 1.e-6*beamAccumMS ) {
    G4Exception("RTVPlanSource::CalculateMetersetEvts",
		"",
		JustWarning,
		("Number of accumulated metersets is different summing Beams FinalCumulativeMetersetWeight = "
		 + GmGenUtils::ftoa(beamAccumMS) + " and summing ScanSpots = "
		 + GmGenUtils::ftoa(CPAccumMS)
		 + " DIFF = " + GmGenUtils::ftoa(beamAccumMS-CPAccumMS) ).c_str());
  }


  //----- Calculate accumulated metersets events
  G4int nEventsTotal = theG4RunManager->GetNumberOfEventsToBeProcessed();
  if(theNEventsPerMSUnit == -1 ) {
    theNEventsPerMSUnit = nEventsTotal / CPAccumMS;
  }
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " theNEventsPerMSUnit " << theNEventsPerMSUnit << " :: " << nEventsTotal << " / " << CPAccumMS << G4endl;
#endif
  G4int msEvt = 0;
  G4double meterSet = 0.;
  CPAccumMS = 0.;
  //  theMetersetEvt.push_back(0);
  G4bool bFirstScanSpot = true;
  for( size_t iib = 0; iib < theBeams.size(); iib++ ) {
    std::vector<RTVPlanControlPoint*> CPs = theBeams[iib]->GetControlPoints();
    for( size_t iicp = 0; iicp < CPs.size(); iicp++ ) {
      G4double numberOfPaintings = CPs[iicp]->GetNumberOfPaintings();
      std::vector<G4double> meterSets = CPs[iicp]->GetMetersets();
      for( size_t iims = 0; iims < meterSets.size(); iims++ ) {
	CPAccumMS += meterSets[iims]*numberOfPaintings;
	if( CPAccumMS > theMSUnitsToSkip ) {
	  if( bFirstScanSpot ) {
	    meterSet += CPAccumMS - theMSUnitsToSkip;
	  } else {
	    meterSet += meterSets[iims]*numberOfPaintings;
	  }
	  msEvt = meterSet*theNEventsPerMSUnit;
	  theMetersetEvt.push_back(msEvt);
	  bFirstScanSpot = false;
	} else {
	  theMetersetEvt.push_back(0);
	}
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << theMetersetEvt.size() << " METERSET EVTS " << iib << " : " << iicp << " : " << iims << " = " << theMetersetEvt[theMetersetEvt.size()-1] << " = MS= " << meterSet << " *NEvtsPerMSUnit= " << theNEventsPerMSUnit << " added " << meterSets[iims] << " *NPaintings= " << numberOfPaintings << G4endl; 
	//	if( GenerVerb(debugVerb) ) G4cout << theMetersetEvt.size() << " METERSET EVTS " << iib << " : " << iicp << " : " << iims << " MetersetCP " << meterSets[iims] << " -> " << meterSet << " NPaintings= " << numberOfPaintings << " *NEvtsPerMSUnit= " << theMetersetEvt[theMetersetEvt.size()-1] << " events " << G4endl; //<- " <<  " if 1st accumMS " << CPAccumMS << " added " << G4endl; 
#endif
      }
    }

  }
}


//-----------------------------------------------------------------------
G4PrimaryVertex* RTVPlanSource::GenerateVertex( G4double time )
{
  if( !bInitialized ) {
    theGeomUtils = GmGeometryUtils::GetInstance();
    theModuleMgr = GmModuleMgr::GetInstance();
    theEvtMgr = G4EventManager::GetEventManager(); 
    CalculateMetersetEvts();
    if( GmParameterMgr::GetInstance()->GetNumericValue(theName+":PlotScanSpots",0)) {
      PlotScanSpots();
    }
    theEventID = -1;

    G4tgrUtils::bAllowRepetitions = true;
    //    G4cout << "G4tgrUtils::bAllowRepetitions0  " << G4tgrUtils::bAllowRepetitions  << G4endl; //GDEB
    BuildRTPlan2G4Associations();

    bInitialized = true;

    //--- Store original phantom translation and rotation 
    std::vector<G4VPhysicalVolume*> PVs = theGeomUtils->GetPhysicalVolumes("phantomContainer",0);
    if( PVs.size() > 0 ) {
      G4VPhysicalVolume* pv = PVs[0];
      theOrigPhantomTranslation = pv->GetTranslation();
      theOrigPhantomRotMat = *(pv->GetRotation());
      bPhantom = true;
    } else {
      bPhantom = false;
    }
  }
  //----- Get Position, direction angle and energy from ScanSpot's
  //  G4cout <<" theEvtMgr->GetConstCurrentEvent() " <<  theEvtMgr->GetConstCurrentEvent() << G4endl;//GDEB
  if( theEvtMgr->GetConstCurrentEvent() ) {
    theEventID = theEvtMgr->GetConstCurrentEvent()->GetEventID();
  } else {
    theEventID++;
  }
  theEventID += theNEventsToSkip;
  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "@@ RTVPlanSource::GenerateVertex change event data theEventID .eq. theNEventsToSkip || theEventID .eq. theMetersetEvt[theCurrentMetersetEvtID] " << G4endl
				    << " " << theEventID << " =? " << theNEventsToSkip << " || " << theEventID << " =? " << theMetersetEvt[theCurrentMetersetEvtID] << " ; theCurrentMetersetEvtID " << theCurrentMetersetEvtID << " time " << time << G4endl;
#endif
  // theCurrentMetersetEvtID is pointing to the next EventID when Data should change
  if( theEventID == theNEventsToSkip // theEventID=0 (or +theNEventsToSkip)
      || theEventID == theMetersetEvt[theCurrentMetersetEvtID] ) { // change event data
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) G4cout << " CHANGING EVENT DATA Evt " << theEventID << " Beam: " << iCurrentBeam <<G4endl;
#endif
    G4int iNextBeam = iCurrentBeam;
    ScanSpotData spdata;
    G4int ie = theCurrentMetersetEvtID;
    /*    if( theCurrentMetersetEvtID != 0 ){
      G4Exception("RTVPlanSource::GenerateVertex",
		  "",
		  JustWarning,
		  ("All ScanSpot's from all beams have been run, and still asking for more events! Run will be aborted at event " + GmGenUtils::itoa(theEventID) + ", while " + GmGenUtils::itoa(theG4RunManager->GetNumberOfEventsToBeProcessed())+" have been requested").c_str() );
      theG4RunManager->AbortRun();
      return (G4PrimaryVertex*)0;
      } */

    G4int ieMax = G4int(theMetersetEvt.size());
    if( theMetersetEvt.size() == 1 && theEventID == theNEventsToSkip ) ieMax = 2; // loop once to get at least one NextScanSpot
    
    for( ; ie < ieMax; ie++ ){
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "@@@ RTVPlanSource::GenerateVertex Looping to metersets which correspond to same EventID than the one from " << theCurrentMetersetEvtID << " :+ie " << ie << G4endl; 
#endif
      //--- If several theMeterSetEvts are equal, run until the last one
      if( theEventID == theNEventsToSkip && theMetersetEvt[theCurrentMetersetEvtID] == 0 ) { //--- Skip first events if meterset = 0
	//      if( theEventID == theNEventsToSkip && ie == 0 ) { //--- Skip first events if meterset = 0
	theCurrentMetersetEvtID++;
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(testVerb) ) G4cout << "@ RTVPlanSource::GenerateVertex Add Data because Skip first events if meterset = 0, evt " << theEventID << " theCurrentMetersetEvtID " << theCurrentMetersetEvtID << G4endl;	
#endif
      } else { 
	if( theMetersetEvt[ie] != theMetersetEvt[theCurrentMetersetEvtID] ) {
	  //	if( theMetersetEvt[ie+(theEventID!=theNEventsToSkip)] != theMetersetEvt[theCurrentMetersetEvtID+(theEventID!=theNEventsToSkip)] ) {
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(testVerb) ) G4cout << "@ RTVPlanSource::GenerateVertexStop adding Data DIFFERENT EvtsID [" << ie << "] = " << theMetersetEvt[ie] << " != [" << theCurrentMetersetEvtID << "] = " << theMetersetEvt[theCurrentMetersetEvtID] << G4endl;
	  //	  if( GenerVerb(testVerb) ) G4cout << "@@ DIFFERENT EvtsID [" << ie+(theEventID!=theNEventsToSkip) << "] = " << theMetersetEvt[ie+(theEventID!=theNEventsToSkip)] << " != [" << theCurrentMetersetEvtID+(theEventID!=theNEventsToSkip) << "] = " << theMetersetEvt[theCurrentMetersetEvtID+(theEventID!=theNEventsToSkip)] << G4endl;
#endif
	  break;
	} else {
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(testVerb) ) G4cout << "@ RTVPlanSource::GenerateVertex Add data EQUAL EvtsID [" << ie << "] = " << theMetersetEvt[ie] << " == [" << theCurrentMetersetEvtID << "] = " << theMetersetEvt[theCurrentMetersetEvtID] << G4endl;
	  //	  if( GenerVerb(testVerb) ) G4cout << "@@ EQUAL EvtsID [" << ie+(theEventID!=theNEventsToSkip) << "] = " << theMetersetEvt[ie+(theEventID!=theNEventsToSkip)] << " == [" << theCurrentMetersetEvtID+(theEventID!=theNEventsToSkip) << "] = " << theMetersetEvt[theCurrentMetersetEvtID+(theEventID!=theNEventsToSkip)] << G4endl;
#endif
	}
      }
      spdata = theBeams[iCurrentBeam]->NextScanSpot(iNextBeam);
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "@@!!  RTVPlanSource::GenerateVertex CHANGED TO NEXT SCAN SPOT in event " << theEventID << " CurrentMetersetEvtID " << ie << G4endl; 
      if( GenerVerb(testVerb) ) G4cout << "@@!!  RTVPlanSource::GenerateVertex NextBeam " << iNextBeam << " !=?Current " << iCurrentBeam << G4endl; 
#endif
      if( iNextBeam != iCurrentBeam ) {  // Beam ControlPoint's exhausted, check next beam
	if( GenerVerb(debugVerb) ) G4cout << " @ RTVPlanSource::GenerateVertex MOVE TO NEXT BEAM iNextBeam != iCurrentBeam " <<  iNextBeam << " != " << iCurrentBeam << " NBEAMS= " << theBeams.size() << G4endl; 
	if( iNextBeam >= G4int(theBeams.size()) ) {
	  G4Exception("RTVPlanSource::GenerateVertex",
		      "",
		      JustWarning,
		      "All ScanSpot's from all beams have been run, and still asking for more events! Run will be aborted");
	  theG4RunManager->AbortRun();
	  return (G4PrimaryVertex*)0;
	}
	
	spdata = theBeams[iNextBeam]->NextScanSpot(iNextBeam);
	if( GenerVerb(testVerb) ) G4cout << " RTVPlanSource::GenerateVertex AFTER NextScanSpot Beam changes? " << iNextBeam << " !=? " << iCurrentBeam << G4endl;
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(testVerb) )
	  G4cout << " GOT NEW BEAM spdata "
		 << " X= " << spdata.PositionX 
		 << " Y= " << spdata.PositionY 
		 << " Z= " << spdata.PositionZ
		 << " E= " << spdata.Energy 
		 << " AngleX= " << spdata.RotAngleX/CLHEP::deg 
		 << " AngleY= " << spdata.RotAngleY/CLHEP::deg 
		 << " AngleZ= " << spdata.RotAngleZ/CLHEP::deg 
		 << " SpotSize= " << spdata.SpotSize 
		 << " SourceAxisDistance= " << spdata.SourceAxisDistance
		 << " ControlPoint " << spdata.ControlPoint 
		 << " Beam " << spdata.Beam << G4endl; 
#endif
	iCurrentBeam = iNextBeam;
      }
    } // loop to CPs with same theMetersetEvt
    
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(-testVerb) )
      G4cout << " GOT BEAM spdata "
	     << " X= " << spdata.PositionX 
	     << " Y= " << spdata.PositionY 
	     << " Z= " << spdata.PositionZ
	     << " E= " << spdata.Energy 
	     << " AngleX= " << spdata.RotAngleX/CLHEP::deg 
	     << " AngleY= " << spdata.RotAngleY/CLHEP::deg 
	     << " AngleZ= " << spdata.RotAngleZ/CLHEP::deg 
	     << " SpotSize= " << spdata.SpotSize 
	     << " SourceAxisDistance= " << spdata.SourceAxisDistance
	     << " ControlPoint " << spdata.ControlPoint 
	     << " Beam " << spdata.Beam << G4endl;
#endif

    //    theLastEventID = theEventID;
    theCurrentMetersetEvtID = ie;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) G4cout << "@ RTVPlanSource::GenerateVertex ENDED ie loop " << ie << " theCurrentMetersetEvtID " << theCurrentMetersetEvtID << G4endl; 
#endif
    //    if( theEventID == theNEventsToSkip && theMetersetEvt[0] != 0 ) theCurrentMetersetEvtID--; // first event: it was forced to take the first metersetEvt
    if( theEventID == theNEventsToSkip ) theCurrentMetersetEvtID--; // first event: it was forced to take the first metersetEvt

    //---- New ScanSpot data
    std::vector<G4String> paramsE;
    //    G4ThreeVector position = G4ThreeVecor(spdata.PositionX,spdata.PositionY,spdata.PositionZ);

    //    G4double sourceAxisDist;    sourceAxisDist = spdata.SourceAxisDistance;

    theEnergy = spdata.Energy;
    if( bEGauss ) { 
      paramsE.push_back(GmGenUtils::ftoa(theEnergy));
      paramsE.push_back(GmGenUtils::ftoa(theEnergySigma));
    } else {
      paramsE.push_back(GmGenUtils::ftoa(theEnergy));
    }
    theEnergyDistribution->SetParams( paramsE );
    SetSpotSize( thePositionDistribution2D, spdata );
    
    MoveAccelerator(spdata);
    if( bPhantom ) MovePhantom( spdata );
    
  } // END NEW spdata

  thePosition = thePositionDistribution->GeneratePosition( this );
  theDirection = theDirectionDistribution->GenerateDirection(this);
  theEnergy = theEnergyDistribution->GenerateEnergy(this);

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << "RTVPlanSource::GenerateVertex pos " << thePosition << G4endl;
#endif

  G4PrimaryVertex* vtx = new G4PrimaryVertex( thePosition, time );
  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << "RTVPlanSource::GenerateVertex ener " << theEnergy << G4endl;
#endif
  
  G4ThreeVector mom = theDirection * std::sqrt(theEnergy*theEnergy + 2.*theParticleDef->GetPDGMass()*theEnergy);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " RTVPlanSource::GenerateVertex mom " << mom << " theEnergy " << theEnergy << G4endl;
#endif
  G4PrimaryParticle* particle = new G4PrimaryParticle( theParticleDef, mom.x(), mom.y(), mom.z() );

  particle->SetWeight( theWeight );

  vtx->SetPrimary(particle);

  return vtx;

}


//------------------------------------------------------------------------
void RTVPlanSource::AssociatePlanVolumes( std::vector<G4String> wl )
{
  if( wl.size() != 3 ) {
    G4Exception("RTVPlanSource::AssociatePlanVolumes",
		"",
		FatalException,
		("Command should have three words: SOURCE_NAME RTPLAN_VOLUME_NAME GEOMETRY_VOLUME_NAME , while it has " + GmGenUtils::itoa(wl.size())).c_str());
  }

  theRTPlanVoluAssoc[wl[1]] = wl[2];
  
}

//------------------------------------------------------------------------
void RTVPlanSource::DefineAcceleratorVolume( std::vector<G4String> wl )
{
  if( wl.size() != 2 ) {
    G4Exception("RTVPlanSource::AssociatePlanVolumes",
		"",
		FatalException,
		("Command should have two words: SOURCE_NAME VOLUME_NAME , while it has " + GmGenUtils::itoa(wl.size())).c_str());
  }

  theAcceleratorVolume = wl[1];
  
}


//------------------------------------------------------------------------
void RTVPlanSource::BuildRTPlan2G4Associations()
{
  for( size_t iib = 0; iib < theBeams.size(); iib++ ) {
    //---- Look for Geant4 volumes
    std::map<G4String,G4double> beamParams = theBeams[iib]->GetParams();
    std::map<G4String,G4double>::const_iterator itep;
    theGeomUtils = GmGeometryUtils::GetInstance();
    for( itep = beamParams.begin() ; itep != beamParams.end(); itep++ ) {
      G4String pname = (*itep).first;
      if( pname.substr(0,4) == "BLD_" && pname.substr(pname.length()-2,2) == "_Z" ) {
	G4String rtVName = pname.substr(4,pname.length()-6);
	std::map<G4String,G4String>::const_iterator itepg = theRTPlanVoluAssoc.find(rtVName);
	G4String g4VName = "";
	if( itepg == theRTPlanVoluAssoc.end() ) {
	  G4Exception("RTVPlanSource::BuildRTPlan2G4Associations",
		      "",
		      JustWarning,
		      ("No association of RT volume " + rtVName + " through an associatePlanVolumes user command, this volume will not be moved").c_str());
	  continue;
	} else {
	  g4VName = (*itepg).second;
	}
	std::vector<G4VPhysicalVolume*> PVs = theGeomUtils->GetPhysicalVolumes(g4VName+"*", 1);
	G4double nLeafJaws = theBeams[iib]->GetParam("BLD_"+rtVName+"_NumberOfLeafJawPairs",1) * 2;
	if( PVs.size() != nLeafJaws ) {
	  G4Exception("RTVPlanSource::BuildRTPlan2G4Associations",
		      "",
		      FatalException,
		      ("Number of G4VPhysicalVolume's for volume " + g4VName + " = "
		       + GmGenUtils::itoa(PVs.size()) + " != number of volumes in RTPlan = "
		       + GmGenUtils::itoa(nLeafJaws) + "Please contact GAMOS authors").c_str());
	}
	theRTPlanG4PVsAssoc[rtVName] = PVs;
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::AssociatePlanVolumes  Beam " << theBeams[iib]->GetIndex() << " " << rtVName << " associated to N PVs " << PVs.size() << G4endl;
#endif
	GmVModule* module = theModuleMgr->GetModule(g4VName,0);
	if( !module ) {
	  G4Exception("RTVPlanSource::BuildRTPlan2G4Associations",
		      "",
		      FatalException,
		      ("Volume must be a GmModule (i.e. constructed with :MODULE in an ASCII geometry file: " + g4VName).c_str());
	}
	theRTPlanGmModuleAssoc[rtVName] = module;
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::AssociatePlanVolumes  Beam. " << theBeams[iib]->GetIndex() << " " << rtVName << " associated to module " << module->GetName() << G4endl;
#endif
      }
    }
  }
 
}

//------------------------------------------------------------------------
void RTVPlanSource::MoveAccelerator(const ScanSpotData& spdata )
{
  G4GeometryManager* geomMgr = G4GeometryManager::GetInstance();
  geomMgr->OpenGeometry();

  //----- MOVE ACCELERATOR
  //--- GET POSITION AND ROTATION FROM RTPlan 
  //  G4ThreeVector position(0.,0.,sourceAxisDist); // if not moved it is in Z axis (IEC 61217)
  G4RotationMatrix* rotation = new G4RotationMatrix;
  rotation->rotateZ(-spdata.RotAngleZ); // GantryPitchAngle
#ifndef GAMOS_NO_VERBOSE
  //  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::GenerateVertex position after GantryPitchAngle " << position << " GantryPitchAngle " << spdata.RotAngleZ/CLHEP::deg << " Rotation " << *rotation << G4endl;
#endif

  //  position.rotateY(spdata.RotAngleY); // GantryAngle
  rotation->rotateY(-spdata.RotAngleY); // GantryAngle
#ifndef GAMOS_NO_VERBOSE
  //   if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator Accelerator position after GantryAngle " << position << " GantryAngle " << spdata.RotAngleY/CLHEP::deg  << " Rotation " << *rotation << G4endl;
#endif

  G4VPhysicalVolume* pv;
  //----- MOVE BLD'S (JAWS and MLC's)
  //--- Loop to all RTPlan BLD's found
  std::map<G4String,std::vector<G4VPhysicalVolume*> >::const_iterator itespv;
  for( itespv = theRTPlanG4PVsAssoc.begin(); itespv != theRTPlanG4PVsAssoc.end(); itespv++ ) {
    G4String rtVol2G4Name = (*itespv).first;
    std::vector<G4VPhysicalVolume*> PVs = (*itespv).second;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) {
      for( size_t ipv = 0; ipv < PVs.size(); ipv++ ) {
	G4cout << " PVs TO MOVE " << ipv << " = " << PVs[ipv]->GetName() << G4endl; 
      }
    }
#endif
    //--- Get position from beam and control point
    G4double posZ; // Z position of center
    
    std::map<G4String,G4double> beamParams = spdata.Beam->GetParams();
    //    std::map<G4String,G4double> cpParams = spdata.ControlPoint->GetParams();
    std::map<G4String,G4double>::const_iterator itep;

    //--- LOOK FOR THE posZ IN RTBeam PARAMETERS
    for( itep = beamParams.begin() ; itep != beamParams.end(); itep++ ) {
      G4String pname = (*itep).first;
      if( pname.substr(0,4) == "BLD_" && pname.substr(pname.length()-2,2) == "_Z" ) {
	G4String rtVName = pname.substr(4,pname.length()-6);
	if( rtVName == rtVol2G4Name ) {
	  posZ = -(*itep).second; // DICOM gives Positions of botomm of JAWS/MLC as a positive number (distance to origin)
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator " << rtVol2G4Name << " position Z " << posZ << G4endl;
#endif
	  break;
	}
      }
    }
    if( itep == beamParams.end() ) {
      G4Exception("RTVPlanSource::MoveAccelerator",
		  "",
		  FatalException,
		  ("VOLUME " + rtVol2G4Name + " NOT FOUND IN CURRENT BEAM: THIS SHOULD NOT HAPPEN, PLEASE CONTACT GAMOS AUTHORS").c_str());
    }

    //--- NOW LOOK FOR posX/Y IN RTPlanControlPoint PARAMETERS
    std::map<G4String,GmVModule*>::const_iterator itemod = theRTPlanGmModuleAssoc.find(rtVol2G4Name);
    GmVModule* gmModule = (*itemod).second;
    G4String g4PVName = gmModule->GetName();
    if( GenerVerb(infoVerb) ) G4cout << " g4PVName = gmModule->GetName() " << g4PVName << G4endl; //GDEB
	  
    G4double nLeafJaws = spdata.Beam->GetParam("BLD_"+rtVol2G4Name+"_NumberOfLeafJawPairs",1) * 2;
    if( dynamic_cast<GmModuleJaws*>(gmModule) ) {
      if( nLeafJaws != 2 ) {
	G4Exception("RTVplanSource::MoveAccelerator",
		    "",
		    FatalException,
		    ("Error processing volume of type GmModuleJaws: there should be two leafs in RTPlan, while there are "
		     + GmGenUtils::itoa(nLeafJaws)).c_str());
      }
      
      G4double posN_1 = spdata.ControlPoint->GetParam(rtVol2G4Name+"_1_POS",1);
      G4double posP_2 = spdata.ControlPoint->GetParam(rtVol2G4Name+"_2_POS",1);
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator " << rtVol2G4Name << " position leaf1 " << posN_1 << " leaf2 " << posP_2 << G4endl;
#endif
      /*      // check that parameters in RTPlan have correct sign
      if( posN_1 > 0 || posP_2 < 0) {
	G4Exception("RTVplanSource::MoveAccelerator",
		    "",
		    FatalException,
		    ("Error processing volume of type GmModuleJaws: first position should be negative and second positive, while they are "
		     + GmGenUtils::ftoa(posN_1) + " , " + GmGenUtils::ftoa(posP_2)).c_str());
		     } */
      //      G4int ngmParams = gmModule->GetParams().size();
      gmModule->ReplaceParam("FIELD_NEG", GmGenUtils::ftoa(posN_1));
      gmModule->ReplaceParam("FIELD_POS", GmGenUtils::ftoa(posP_2));
      posZ += GmGenUtils::GetValue(gmModule->GetParam("ZHDIM"));// add half_length_Z (position in RTPlanBeam is of bottom=farthest to beam)
      gmModule->ReplaceParam("Z_CENTRE", GmGenUtils::ftoa(posZ)); 
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator " << rtVol2G4Name << " position Z " << posZ << G4endl;
#endif
      //      G4cout << " posZ-gmModule->GetParam(5)) " << gmModule->GetParam("ZHDIM") << " -> " << GmGenUtils::GetValue(gmModule->GetParam("ZHDIM")) << G4endl; //GDEB
      gmModule->BuildObjects(); // BUILD G4tgr OBJECTS

      //find new G4tgrPlace
      /*      std::vector<G4tgrVolume*> tgrVolumes =  G4tgrVolumeMgr::GetInstance()->GetVolumeList();
      for( size_t itv = 0; itv < tgrVolumes.size(); itv++ ) {
	if( tgrVolumes[itv]->GetName() == g4PVName ) {
	  std::vector<G4tgrPlace*> tgrPlaces = tgrVolumes[itv]->GetPlacements();
	  G4cout << " FOUND " << g4PVName << " = " << tgrVolumes[itv] << " NPLACES " << tgrPlaces.size() << G4endl; //GDEB
	  for( size_t ip = 0; ip < tgrPlaces.size(); ip++ ) {
	    G4cout << " PLACE COPYNO " << tgrPlaces[ip]->GetCopyNo() << G4endl;
	  }
	}
	}*/
      std::vector<G4tgrVolume*> tgrVolumes = G4tgrVolumeMgr::GetInstance()->GetVolumeList();
      for( size_t itv = 0; itv < tgrVolumes.size(); itv++ ) {
#ifndef GAMOS_NO_VERBOSE
	//	if( GenerVerb(testVerb) ) G4cout << " tgrVolumes[itv]->GetName() == g4PVName " << tgrVolumes[itv]->GetName()<< " == " << g4PVName << G4endl;
#endif
	if( tgrVolumes[itv]->GetName().find(g4PVName) != std::string::npos ) {
	  std::vector<G4tgrPlace*> tgrPlaces = tgrVolumes[itv]->GetPlacements();
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator " << rtVol2G4Name << " Number of tgrPlaces " << tgrPlaces.size() << G4endl;
#endif
	  //--- Get last two
	  G4tgrPlace* placeN = tgrPlaces[tgrPlaces.size()-2];
	  G4tgrPlace* placeP = tgrPlaces[tgrPlaces.size()-1];
	  //--- Get unique two
	  if( tgrPlaces.size() != 2 ) {
	    G4Exception("RTVPlanSource::MoveAccelerator",
		       "",
		       FatalException,
		       ("Not only two placement found for volume " + g4PVName
			+ "  Number of placements found= " + GmGenUtils::itoa(tgrPlaces.size())).c_str());
	  }
	  //--- Now change placements in G4VPhysicalVolume's
	  //--- Move volume
	  //t	  geomMgr->OpenGeometry( PVs[0]);
	  PVs[0]->SetTranslation(placeN->GetPlacement());
	  //t	  geomMgr->CloseGeometry(true,false,PVs[0]);
	  //t	  geomMgr->OpenGeometry( PVs[1]);
	  PVs[1]->SetTranslation(placeP->GetPlacement());
	  //t	  geomMgr->CloseGeometry(true,false,PVs[1]);

	  std::vector<GmTouchable*> touchs2 = GmGeometryUtils::GetInstance()->GetTouchables( PVs[0]->GetName(), true );
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(testVerb) ) {
	    G4cout << " PRINTING TOUCHABLES " << PVs[0]->GetName() << " : " << touchs2.size() << G4endl; 
	    for( unsigned int jj = 0; jj < touchs2.size(); jj++ ) {
	      G4cout << " TOUCHABLE " << *(touchs2[jj]) << G4endl;
	    }
	  }
#endif

	}
      }
      
    } else if( dynamic_cast<GmModuleMLC*>(gmModule) ) {

      std::map<G4String,G4double> mlcParams = spdata.ControlPoint->GetParams();
      for( std::map<G4String,G4double>::const_iterator itemp = mlcParams.begin(); itemp != mlcParams.end(); itemp++ ) {
	//-	G4cout << " mlcParams " << (*itemp).first << " = " << (*itemp).second << G4endl; //GDEB
      }
      //----- STORE NEGATIVE LEAFS AND POSITIVE LEAFS POSITIONS FROM RTControlPoint
      std::vector<G4double> posPos, posNeg;
      for( size_t ipv = 1; ipv <= PVs.size()/2; ipv++ ) { //FIRST HALF POSITIONS ARE NEGATIVE LEAFS
	//	G4cout << " SEARCH mlcParam " << rtVol2G4Name+"_"+GmGenUtils::itoa(ipv)+"_POS" << G4endl; //GDEB
	posNeg.push_back(spdata.ControlPoint->GetParam(rtVol2G4Name+"_"+GmGenUtils::itoa(ipv)+"_POS",1)); //Numbering starts at 1 in DICOM
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator position leaf negative " << rtVol2G4Name+"_"+GmGenUtils::itoa(ipv)+"_POS  = " << posNeg[posNeg.size()-1] << G4endl;
#endif
      }
      for( size_t ipv = PVs.size()/2+1; ipv <= PVs.size(); ipv++ ) { //SECOND HALF POSITIONS ARE POSITIVE LEAFS
	posPos.push_back(spdata.ControlPoint->GetParam(rtVol2G4Name+"_"+GmGenUtils::itoa(ipv)+"_POS",1));
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator position leaf positive " << rtVol2G4Name+"_"+GmGenUtils::itoa(ipv)+"_POS = " << posPos[posPos.size()-1] << G4endl;
#endif
      }

      //----- REPLACE LEAF POSITIONS IN GmModuleMLC
      for( size_t ii = 1; ii < posNeg.size()+1; ii++ ) {
	gmModule->ReplaceParam("OPEN_POSITION_NEG["+GmGenUtils::itoa(ii)+"]", GmGenUtils::ftoa(posNeg[ii-1]));
	gmModule->ReplaceParam("OPEN_POSITION_POS["+GmGenUtils::itoa(ii)+"]", GmGenUtils::ftoa(posPos[ii-1]));
      }

      //----- REPLACE Z POSITION IN GmModuleMLC
      posZ -= GmGenUtils::GetValue(gmModule->GetParam("Z_GAP"));// substract length_Z (Z_GAP is interpreted to be = leaf length !!! (Z_GAP is negative and position in RTPlanBeam is of bottom=farthest to beam)
      gmModule->ReplaceParam("Z_TOP", GmGenUtils::ftoa(posZ)); 

      //----- BUILD G4tgr OBJECTS
      gmModule->BuildObjects(); 
      
      //find new G4tgrPlace
      std::vector<G4tgrVolume*> tgrVolumes =  G4tgrVolumeMgr::GetInstance()->GetVolumeList();
      //	:PLACE MLC_X_80_POS 1 ACCEL RM90Y180Z_MLC 95.1766 0 311.77FF
      for( size_t ii = 1; ii <= posNeg.size()+posPos.size(); ii++ ) {
	G4String suffix = "";
	G4int iip = ii;
	if( ii <= posNeg.size() ) {
	  suffix = "_NEG";
	} else {
	  suffix = "_POS";
	  iip -= posNeg.size();
	}
	G4String g4PVNameLeaf = g4PVName+"_Leaf_"+GmGenUtils::itoa(iip)+suffix;
	for( size_t itv = 0; itv < tgrVolumes.size(); itv++ ) {
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(testVerb) ) G4cout << " tgrVolumes[itv]->GetName() == g4PVName " << tgrVolumes[itv]->GetName()<< " == " << g4PVNameLeaf << G4endl;
#endif
	  if( tgrVolumes[itv]->GetName() == G4String(g4PVNameLeaf) ) {
	    std::vector<G4tgrPlace*> tgrPlaces = tgrVolumes[itv]->GetPlacements();
#ifndef GAMOS_NO_VERBOSE
	    if( GenerVerb(testVerb) ) {
	      G4cout << " FOUND " << g4PVNameLeaf << " = " << tgrVolumes[itv] << " NPLACES " << tgrPlaces.size() << G4endl; 
	      for( size_t ip = 0; ip < tgrPlaces.size(); ip++ ) {
		G4cout << " PLACE COPYNO " << tgrPlaces[ip]->GetCopyNo() << G4endl;
	      }
	    }
	    if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator " << rtVol2G4Name << " Number of tgrPlaces " << tgrPlaces.size() << G4endl;
#endif
	    //--- Get unique one
	    if( tgrPlaces.size() != 1 ) {
	      G4Exception("RTVPlanSource::MoveAccelerator",
			  "",
			  FatalException,
			  ("Not only one placement found for volume " + g4PVNameLeaf
			   + "  Number of placements found= " + GmGenUtils::itoa(tgrPlaces.size())).c_str());
	    }
	    G4tgrPlace* place = tgrPlaces[tgrPlaces.size()-1];
	    //--- Now change placements in G4VPhysicalVolume's
	    //--- Move volume
	    //--- Check volume name
	    pv = PVs[iip*2-1-(ii<=posNeg.size())]; // FIRST IS X-2 (NEG), not X-1 (POS)
	    if( pv->GetName() != G4String(g4PVNameLeaf) ) {
	      G4Exception("RTVPlanSource::MoveAccelerator",
			  "",
			  FatalException,
			  ("Trying to move wrong volume: " + pv->GetName() +
			   " instead of " + g4PVNameLeaf).c_str());
	    }
	    //t	    geomMgr->OpenGeometry( pv );
	    pv->SetTranslation(place->GetPlacement());
	    //t	    geomMgr->CloseGeometry(true,false,pv);
#ifndef GAMOS_NO_VERBOSE
	    if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MoveAccelerator VOLUME MOVED " << pv->GetName() << " -> " << pv->GetTranslation() << G4endl;
#endif
	      
#ifndef GAMOS_NO_VERBOSE
	    if( GenerVerb(testVerb) ) {
	      std::vector<GmTouchable*> touchs2 = GmGeometryUtils::GetInstance()->GetTouchables(pv->GetName(), true);
   	      G4cout << " PRINTING TOUCHABLES " << pv->GetName() << " : " << touchs2.size() << G4endl; 
	      for( unsigned int jj = 0; jj < touchs2.size(); jj++ ) {
	        G4cout << " TOUCHABLE " << *(touchs2[jj]) << G4endl;
	      }
	    }
#endif
	  }
	}
      }

    } else {
      G4Exception("RTVplanSource::MoveAccelerator",
		  "",
		  FatalException,
		  ("VOLUME " + rtVol2G4Name + " NOT FOUND IN CURRENT BEAM: THIS SHOULD NOT HAPPEN, PLEASE CONTACT GAMOS AUTHORS").c_str());
    }
      
  }

  // Notify the VisManager as well
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) pVVisManager->GeometryHasChanged();

  //  G4UImanager * UI = G4UImanager::GetUIpointer(); //GDEB
  // UI->ApplyCommand("/control/execute visVRML2FILE.in"); //GDEB
  
  return;
  
}

//------------------------------------------------------------------------
void RTVPlanSource::MovePhantom(const ScanSpotData& spdata )
{

  //-- Get center of phantomContainer
  GmReadPhantomGeometry* phGeom = static_cast<GmReadPhantomGeometry*>(const_cast<G4VUserDetectorConstruction*>(theG4RunManager->GetUserDetectorConstruction()));
  if( !phGeom ) {
    G4Exception("RTVPlanSource::MovePhantom",
		"",
		FatalException,
		"Geometry constructor has to be of type GmReadPhantomGeometry");
  }
  G4ThreeVector pcCenter((phGeom->GetMaxX()+phGeom->GetMinX())/2.,
			 (phGeom->GetMaxY()+phGeom->GetMinY())/2.,
			 (phGeom->GetMaxZ()+phGeom->GetMinZ())/2.);

  std::vector<G4VPhysicalVolume*> PVs = theGeomUtils->GetPhysicalVolumes("phantomContainer",1);
  G4VPhysicalVolume* pv = PVs[0];
  G4ThreeVector phantomTranslation = theOrigPhantomTranslation;
  G4RotationMatrix* phantomRotMat = new G4RotationMatrix(theOrigPhantomRotMat);
    // check InitPhantomPos is 0
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << "RTVPlanSource::MovePhantom phantomTranslation " <<phantomTranslation  << G4endl;
#endif

  G4ThreeVector isocenter( spdata.IsocenterPosition.x(), spdata.IsocenterPosition.y(), spdata.IsocenterPosition.z());
  G4ThreeVector isocLocalPoint = isocenter - pcCenter;

  isocenter *= *phantomRotMat;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MovePhantom isocenter after origRot " << isocenter << " rotMat " << *phantomRotMat << G4endl;
#endif
  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MovePhantom isocenter local " << isocLocalPoint << " isocenter " << isocenter << " phantomCenter " << phantomTranslation << G4endl;
#endif
  isocLocalPoint *= CLHEP::inverseOf(*(phantomRotMat)); //displace to rotate around isocenter
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MovePhantom isocenter local after phantom rotation " << isocLocalPoint << G4endl;
#endif
  //-- isocenter placed at (0,0,-spdata.SourceAxisDistance)
  phantomTranslation += G4ThreeVector(0.,0.,-spdata.SourceAxisDistance) - phantomTranslation;  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << "RTVPlanSource::MovePhantom phantomTranslation at (0,0,-spdata.SourceAxisDistance) " <<phantomTranslation  << G4endl;
#endif

  //--- Rotate GantryAngle, around isocenter  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MovePhantom rotmat " << *phantomRotMat << G4endl;
#endif
  //  phantomRotMat->rotateY(-spdata.RotAngleY);
  //G4RotationMatrix rotMatGA;
  // rotMatGA.rotateY(-spdata.RotAngleY);
  // *phantomRotMat *= rotMatGA;
  //  phantomRotMat->rotateY(-10.*CLHEP::deg);
  //  phantomRotMat->rotateY(-spdata.RotAngleY);
  G4RotationMatrix rotMatGA;
  rotMatGA.rotateY(spdata.RotAngleY);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << "RTVPlanSource::MovePhantom unit rotMat after rotation " << rotMatGA << G4endl;
#endif
  *phantomRotMat *= rotMatGA;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MovePhantom rotmat after rotation " << spdata.RotAngleY/CLHEP::deg << " " << *phantomRotMat << G4endl;
#endif
  //-  isocLocalPoint.rotateY(spdata.RotAngleY); //displace to rotate around isocenter
  isocLocalPoint *= CLHEP::inverseOf(rotMatGA); //displace to rotate around isocenter
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "RTVPlanSource::MovePhantom isocenter local after rotation " << isocLocalPoint << G4endl;
#endif
  phantomTranslation -= isocLocalPoint;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << "RTVPlanSource::MovePhantom phantomTranslation after rotation " <<phantomTranslation  << G4endl;
#endif

  pv->SetTranslation(phantomTranslation);
  pv->SetRotation(phantomRotMat);

  // Notify the VisManager as well
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) pVVisManager->GeometryHasChanged();

  /*  GmGeometryUtils::GetInstance()->DumpG4PVLVTree( G4cout, 23 );
  std::vector<GmTouchable*> touchs2 = GmGeometryUtils::GetInstance()->GetTouchables( pv->GetName(), true );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) {
    G4cout << " PRINTING TOUCHABLES " << pv->GetName() << " : " << touchs2.size() << G4endl; 
    for( unsigned int jj = 0; jj < touchs2.size(); jj++ ) {
      G4cout << " TOUCHABLE " << *(touchs2[jj]) << G4endl;
    }
  }
#endif
  
  //  G4UImanager * UI = G4UImanager::GetUIpointer(); //GDEB
  //  UI->ApplyCommand("/control/execute visVRML2FILE.in"); //GDEB
 
  */
}

//-----------------------------------------------------------------------
RTVPlanBeam* RTVPlanSource::GetBeam( G4int ii )
{
  if( ii < 0 || ii >= G4int(theBeams.size()) ) {
    G4Exception("RTVPlanSource::GetBeam",
		"",
		FatalErrorInArgument,
		G4String("Asking for beam " + GmGenUtils::itoa(ii) + " while there are only " + GmGenUtils::itoa(theBeams.size())).c_str());
  }

  return theBeams[ii];

}

