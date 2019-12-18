//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
////////////////////////////////////////////////////////////////////////
// Optical Photon Fluorescence Class Implementation
////////////////////////////////////////////////////////////////////////
//
// File G4OpTissueFluor.hh
// Description: Discrete Process -- Fluorescence of Optical Photons
// Created: 2013-02-22
// Author: Adam Glaser
// Based on work from John Paul Archambault, and Peter Gumplinger
//
// This subroutine will perform optical fluorescence.
//
// 2019-12-06 Added to GAMOS 6.1  
//
// mail:  adam.k.glaser@dartmouth.edu
//
////////////////////////////////////////////////////////////////////////

#include "G4OpTissueFluor.hh"
#include "G4ios.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpProcessSubType.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4WLSTimeGeneratorProfileDelta.hh"
#include "G4WLSTimeGeneratorProfileExponential.hh"
#include "CLHEP/Random/RandFlat.h"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"

G4OpTissueFluor::G4OpTissueFluor(const G4String& processName, G4ProcessType type)
  : G4VDiscreteProcess(processName, type)
{
  SetProcessSubType(fOpWLS);

  theIntegralTable = 0;
 
  if (verboseLevel>0) {
    G4cout << GetProcessName() << " is created " << G4endl;
  }

  WLSTimeGeneratorProfile = 
       new G4WLSTimeGeneratorProfileDelta("WLSTimeGeneratorProfileDelta");

  BuildThePhysicsTable();

}

G4OpTissueFluor::~G4OpTissueFluor()
{
  if (theIntegralTable != 0) {
    theIntegralTable->clearAndDestroy();
    delete theIntegralTable;
  }
  delete WLSTimeGeneratorProfile;
}

////////////
// Methods
////////////

G4VParticleChange*
G4OpTissueFluor::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
  
  const G4Material* aMaterial = aTrack.GetMaterial();
  
  G4int materialIndex = aMaterial->GetIndex();

  G4MaterialPropertiesTable* aMaterialPropertiesTable =
    aMaterial->GetMaterialPropertiesTable();
  if (!aMaterialPropertiesTable)
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);

  G4double quantum_yield=1.0;

  if (aMaterialPropertiesTable->ConstPropertyExists("FLUOR_QUANTUMYIELD")){	
	quantum_yield   = aMaterialPropertiesTable->GetConstProperty("FLUOR_QUANTUMYIELD");
  }
  else{
	G4Exception("G4OpTissueFluor", "No quantum yield specified.",
                FatalException,
                "Please add using constant material property FLUOR_QUANTUMYIELD.");
  }


  G4String name = GmParameterMgr::GetInstance()->GetStringValue("G4OpTissueFluor:TimeProfile","");
  G4String calcType = GmParameterMgr::GetInstance()->GetStringValue("G4OpTissueFluor:Method","");
  
  	if( calcType == "fixed" ) {
	} 
	else if( calcType == "interpolate" ) {
	} 
	else {
	    G4Exception("G4OpTissueFluor:Method",
			"Error in type of calculation", 
			  FatalErrorInArgument,
			  G4String("Please specify with /gamos/setParam G4OpTissueFluor:Method TYPE. Only options supported: fixed and interpolate, you have selected " + calcType).c_str());
	}

  if (name == "delta")
    {
      delete WLSTimeGeneratorProfile;
      WLSTimeGeneratorProfile = 
             new G4WLSTimeGeneratorProfileDelta("delta");
    }
  else if (name == "exponential")
    {
      delete WLSTimeGeneratorProfile;
      WLSTimeGeneratorProfile =
             new G4WLSTimeGeneratorProfileExponential("exponential");
    }
  else
    {
      G4Exception("G4OpTissueFluor", "No Time Profile Specified",
                  FatalException,
                  "Please choose delta or exponential by using the command, /gamos/setParam G4OpTissueFluor:TimeProfile TYPE");
    }

  G4String Qsplit = GmParameterMgr::GetInstance()->GetStringValue("G4OpTissueFluor:QSplit","OFF");
  G4double theWeight=aTrack.GetWeight();  

if (Qsplit == "OFF") {

  G4double quantum_rand = 1.*G4UniformRand();

  if (quantum_rand >= quantum_yield) {

	  aParticleChange.Initialize(aTrack);

	  aParticleChange.ProposeTrackStatus(fStopAndKill);

	  if (verboseLevel>0) {
	    G4cout << "\n** Photon absorbed! **" << G4endl;
	  }
      return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }
  
  else {
  
  aParticleChange.Initialize(aTrack);
  
  aParticleChange.ProposeTrackStatus(fStopAndKill);

  if (verboseLevel>0) {
    G4cout << "\n** Photon absorbed! **" << G4endl;
  }
  
  G4StepPoint* pPostStepPoint = aStep.GetPostStepPoint();
    
  if (!aMaterialPropertiesTable)
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);

  const G4MaterialPropertyVector* WLS_Intensity = 
    aMaterialPropertiesTable->GetProperty("FLUOR_EMISSION"); 

  if (!WLS_Intensity)
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  
  G4int numPhotons = GmParameterMgr::GetInstance()->GetNumericValue("G4OpTissueFluor:NSplit",1);

  aParticleChange.SetNumberOfSecondaries(numPhotons);

  // Retrieve the WLS Integral for this material
  // new G4PhysicsOrderedFreeVector allocated to hold CII's

  G4double WLSTime = 0.*ns;

  if (aMaterialPropertiesTable->ConstPropertyExists("FLUOR_LIFETIME")){	
  WLSTime   = aMaterialPropertiesTable->
    GetConstProperty("FLUOR_LIFETIME");
  }
  else {
	G4Exception("G4OpTissueFluor", "No lifetime specified.",
                FatalException,
                "Please add using constant material property FLUOR_LIFETIME.");
  }

  G4double primaryEnergy = aTrack.GetDynamicParticle()->GetKineticEnergy();
    
  for (G4int i=0 ; i<numPhotons ; i++) {

    // Determine photon energy
	G4PhysicsOrderedFreeVector* WLSIntegral = NULL;
	G4double sampledEnergy = 0.0;
	WLSIntegral = (G4PhysicsOrderedFreeVector*)((*theIntegralTable)(materialIndex));
	emissionprob=emissionprobmap[materialIndex];
	G4double CIImax = WLSIntegral->GetMaxValue();
	//for (G4int j=0; j<=100; j++) {
		// Determine photon energy
		if (calcType == "interpolate"){
			G4double CIIvalue = G4UniformRand()*CIImax;
	        sampledEnergy = 
	                      WLSIntegral->GetEnergy(CIIvalue);
		}
		else if (calcType == "fixed"){
			G4double pv = CLHEP::RandFlat::shoot();
			std::map<G4double,G4double>::iterator ite = emissionprob.upper_bound( pv );
			sampledEnergy = (*ite).second;
		}
		else {
			G4Exception("G4OpTissueFluor", "Invalid fluorescence method.",
                FatalException,
                "Please set parameter for G4OpTissueFluor:Method [interpolate/fixed]");
		}
		if (verboseLevel>2) {
			G4cout << "G4OpTissueFluor: primaryEnergy = " << primaryEnergy << G4endl;
		    G4cout << "G4OpTissueFluor: sampledEnergy = " << sampledEnergy << G4endl;
		    G4cout << "G4OpTissueFluor: CIImax = " << CIImax << G4endl;
		}
		//if (sampledEnergy <= primaryEnergy) break;

	//}
	
	// Generate random photon direction

	G4double Theta = pi*G4UniformRand();
	G4double Phi = 2*pi*G4UniformRand();

	// Create photon momentum direction vector

	G4ParticleMomentum photonMomentum(std::sin(Theta)*std::cos(Phi), std::sin(Theta)*std::sin(Phi), std::cos(Theta));

	// Determine polarization of new photon

	G4double sx = std::cos(Theta)*std::cos(Phi);
	G4double sy = std::cos(Theta)*std::sin(Phi);
	G4double sz = -std::sin(Theta);

	G4ThreeVector photonPolarization(sx, sy, sz);

	G4ThreeVector perp = photonMomentum.cross(photonPolarization);

	G4double phi = 2*pi*G4UniformRand();
	G4double sinp = std::sin(phi);
	G4double cosp = std::cos(phi);

	photonPolarization = cosp * photonPolarization + sinp * perp;

	photonPolarization = photonPolarization.unit();
    
    // Generate a new photon:
    
    G4DynamicParticle* aWLSPhoton =
      new G4DynamicParticle(G4OpticalPhoton::OpticalPhoton(),
			    photonMomentum);
    aWLSPhoton->SetPolarization
      (photonPolarization.x(),
       photonPolarization.y(),
       photonPolarization.z());
    
    aWLSPhoton->SetKineticEnergy(sampledEnergy);
    
    // Must give position of WLS optical photon

    G4double TimeDelay = WLSTimeGeneratorProfile->GenerateTime(WLSTime);
    G4double aSecondaryTime = (pPostStepPoint->GetGlobalTime()) + TimeDelay;
	
    G4ThreeVector aSecondaryPosition = pPostStepPoint->GetPosition();

    G4Track* aSecondaryTrack = 
      new G4Track(aWLSPhoton,aSecondaryTime,aSecondaryPosition);
   
    aSecondaryTrack->SetTouchableHandle(aTrack.GetTouchableHandle()); 
    
    aSecondaryTrack->SetParentID(aTrack.GetTrackID());

    aSecondaryTrack->SetWeight(theWeight/numPhotons);

    aParticleChange.AddSecondary(aSecondaryTrack);

  }
  }
}
else if (Qsplit == "ON"){
	  aParticleChange.Initialize(aTrack);

	  aParticleChange.ProposeTrackStatus(fStopAndKill);

	  if (verboseLevel>0) {
	    G4cout << "\n** Photon absorbed! **" << G4endl;
	  }

	  G4StepPoint* pPostStepPoint = aStep.GetPostStepPoint();

	  if (!aMaterialPropertiesTable)
	    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);

	  const G4MaterialPropertyVector* WLS_Intensity = 
	    aMaterialPropertiesTable->GetProperty("FLUOR_EMISSION"); 

	  if (!WLS_Intensity)
	    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);

	  G4int numPhotons = GmParameterMgr::GetInstance()->GetNumericValue("G4OpTissueFluor:NSplit",1);

	  aParticleChange.SetNumberOfSecondaries(numPhotons);

	  // Retrieve the WLS Integral for this material
	  // new G4PhysicsOrderedFreeVector allocated to hold CII's

	  G4double WLSTime = 0.*ns;

	  if (aMaterialPropertiesTable->ConstPropertyExists("FLUOR_LIFETIME")){	
	  WLSTime   = aMaterialPropertiesTable->
	    GetConstProperty("FLUOR_LIFETIME");
	  }
	  else {
		G4Exception("G4OpTissueFluor", "No lifetime specified.",
	                FatalException,
	                "Please add using constant material property FLUOR_LIFETIME.");
	  }

	  G4double primaryEnergy = aTrack.GetDynamicParticle()->GetKineticEnergy();


	  for (G4int i=0 ; i<numPhotons ; i++) {

	    // Determine photon energy
		G4PhysicsOrderedFreeVector* WLSIntegral = NULL;
		G4double sampledEnergy = 0.0;
		WLSIntegral = (G4PhysicsOrderedFreeVector*)((*theIntegralTable)(materialIndex));
		emissionprob=emissionprobmap[materialIndex];
		G4double CIImax = WLSIntegral->GetMaxValue();
		//for (G4int j=0; j<=100; j++) {
			// Determine photon energy
			if (calcType == "interpolate"){
				G4double CIIvalue = G4UniformRand()*CIImax;
		        sampledEnergy = 
		                      WLSIntegral->GetEnergy(CIIvalue);
			}
			else if (calcType == "fixed"){
				G4double pv = CLHEP::RandFlat::shoot();
				std::map<G4double,G4double>::iterator ite = emissionprob.upper_bound( pv );
				sampledEnergy = (*ite).second;
			}
			else {
				G4Exception("G4OpTissueFluor", "Invalid fluorescence method.",
                FatalException,
                "Please set parameter for G4OpTissueFluor:Method [interpolate/fixed]");
			}
			if (verboseLevel>2) {
				G4cout << "G4OpTissueFluor: primaryEnergy = " << primaryEnergy << G4endl;
			    G4cout << "G4OpTissueFluor: sampledEnergy = " << sampledEnergy << G4endl;
			    G4cout << "G4OpTissueFluor: CIImax = " << CIImax << G4endl;
			}
			//if (sampledEnergy <= primaryEnergy) break;
		//}


		// Generate random photon direction

		G4double Theta = pi*G4UniformRand();
		G4double Phi = 2*pi*G4UniformRand();

		// Create photon momentum direction vector

		G4ParticleMomentum photonMomentum(std::sin(Theta)*std::cos(Phi), std::sin(Theta)*std::sin(Phi), std::cos(Theta));

		// Determine polarization of new photon

		G4double sx = std::cos(Theta)*std::cos(Phi);
		G4double sy = std::cos(Theta)*std::sin(Phi);
		G4double sz = -std::sin(Theta);

		G4ThreeVector photonPolarization(sx, sy, sz);

		G4ThreeVector perp = photonMomentum.cross(photonPolarization);

		G4double phi = 2*pi*G4UniformRand();
		G4double sinp = std::sin(phi);
		G4double cosp = std::cos(phi);

		photonPolarization = cosp * photonPolarization + sinp * perp;

		photonPolarization = photonPolarization.unit();

	    // Generate a new photon:

	    G4DynamicParticle* aWLSPhoton =
	      new G4DynamicParticle(G4OpticalPhoton::OpticalPhoton(),
				    photonMomentum);
	    aWLSPhoton->SetPolarization
	      (photonPolarization.x(),
	       photonPolarization.y(),
	       photonPolarization.z());

	    aWLSPhoton->SetKineticEnergy(sampledEnergy);
	    
		// Generate new G4Track object:

	    // Must give position of WLS optical photon

	    G4double TimeDelay = WLSTimeGeneratorProfile->GenerateTime(WLSTime);
	    G4double aSecondaryTime = (pPostStepPoint->GetGlobalTime()) + TimeDelay;

	    G4ThreeVector aSecondaryPosition = pPostStepPoint->GetPosition();

	    G4Track* aSecondaryTrack = 
	      new G4Track(aWLSPhoton,aSecondaryTime,aSecondaryPosition);

	    aSecondaryTrack->SetTouchableHandle(aTrack.GetTouchableHandle()); 

	    aSecondaryTrack->SetParentID(aTrack.GetTrackID());

	    aSecondaryTrack->SetWeight(quantum_yield*theWeight/numPhotons);

	    aParticleChange.AddSecondary(aSecondaryTrack);
	
      }
}
else{
	G4Exception("G4OpTissueFluor", "Invalid QSplit option defined.",
                  FatalException,
                  "Please choose ON or OFF by using the command, /gamos/setParam G4OpTissueFluor:QSplit TYPE");
}
  if (verboseLevel>0) {
   G4cout << "\n Exiting from G4OpTissueFluor::DoIt -- NumberOfSecondaries = " 
   << aParticleChange.GetNumberOfSecondaries() << G4endl;  
 }

 return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}
void G4OpTissueFluor::BuildThePhysicsTable()
{

  if (theIntegralTable) return;
  
  const G4MaterialTable* theMaterialTable = 
    G4Material::GetMaterialTable();
  G4int numOfMaterials = G4Material::GetNumberOfMaterials();
  
  // create new physics table
  
  if(!theIntegralTable)theIntegralTable = new G4PhysicsTable(numOfMaterials);
  
  // loop for materials
  
  for (G4int i=0 ; i < numOfMaterials; i++)
    {
      G4PhysicsOrderedFreeVector* aPhysicsOrderedFreeVector =
	new G4PhysicsOrderedFreeVector();
      
      // Retrieve vector of WLS wavelength intensity for
      // the material from the material's optical properties table.
      
      G4Material* aMaterial = (*theMaterialTable)[i];

      G4MaterialPropertiesTable* aMaterialPropertiesTable =
	aMaterial->GetMaterialPropertiesTable();

      if (aMaterialPropertiesTable) {

	G4MaterialPropertyVector* theWLSVector = 
	  aMaterialPropertiesTable->GetProperty("FLUOR_EMISSION");

	if (theWLSVector) {
	  
	  // Retrieve the first intensity point in vector
	  // of (photon energy, intensity) pairs
	  
	  G4double currentIN = (*theWLSVector)[0];
	  
	  if (currentIN >= 0.0) {

	    // Create first (photon energy) 
	   
	    G4double currentPM = theWLSVector->Energy(0);
	    
	    G4double currentCII = 0.0;
	    
	    aPhysicsOrderedFreeVector->
	      InsertValues(currentPM , currentCII);
	    
	    // Set previous values to current ones prior to loop
	    
	    G4double prevPM  = currentPM;
	    G4double prevCII = currentCII;
	    G4double prevIN  = currentIN;
	    
	    // loop over all (photon energy, intensity)
	    // pairs stored for this material

        for (size_t j = 1; j < theWLSVector->GetVectorLength(); j++)	    
	    {
			currentPM = theWLSVector->Energy(j);
			currentIN = (*theWLSVector)[j];

	  		currentCII = 0.5 * (prevIN + currentIN) / pow((prevPM+currentPM)/2.0,2.0);
		
			currentCII = prevCII +
			  (currentPM - prevPM) * currentCII;
		
			aPhysicsOrderedFreeVector->
			  InsertValues(currentPM, currentCII);
		
			prevPM  = currentPM;
			prevCII = currentCII;
			prevIN  = currentIN;
	    }
	
	   G4double energy = 0.0;
	   std::map<G4double,G4double> temprob;
		
	   for (size_t j = 0; j < theWLSVector->GetVectorLength(); j++)
	   {
	   		energy = theWLSVector->Energy(j);
			temprob[energy]=(*theWLSVector)[j];
	   }

		//---- Calculate sum of probabilities to normalize 
		std::map<G4double,G4double>::iterator ite;
		G4double tp = 0.;
	    for(ite = temprob.begin(); ite != temprob.end(); ite++){
	    	tp += (*ite).second;
	    }
	    G4double maxProbInv = 1./tp;

	    //--- Get the inverse of probabilities - energies distribution
	    tp = 0.;
	    for(ite = temprob.begin(); ite != temprob.end(); ite++){
		    G4double prob = (*ite).second; 
		      if( prob == 0. && std::distance( temprob.begin(), ite ) == G4int(temprob.size())-1 ) {
		      	prob = 1.E-10; // for last bin limit the probability is set to 0. by convention
		      }
		    tp += prob * maxProbInv;
		    emissionprob[tp] = (*ite).first;
   		}
	
	  }
	}
    }
	// The WLS integral for a given material
	// will be inserted in the table according to the
	// position of the material in the material table.
	emissionprobmap[i]=emissionprob;
	theIntegralTable->insertAt(i,aPhysicsOrderedFreeVector);
    }
}

G4double G4OpTissueFluor::GetMeanFreePath(const G4Track& aTrack,
 				         G4double ,
				         G4ForceCondition* )
{
  const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();
  const G4Material* aMaterial = aTrack.GetMaterial();

  G4double thePhotonEnergy = aParticle->GetTotalEnergy();

  G4MaterialPropertiesTable* aMaterialPropertyTable;
  G4MaterialPropertyVector* AttenuationLengthVector;
	
  G4double AttenuationLength = DBL_MAX;

  aMaterialPropertyTable = aMaterial->GetMaterialPropertiesTable();

  if ( aMaterialPropertyTable ) {
    AttenuationLengthVector = aMaterialPropertyTable->
      GetProperty("FLUOR_ABSCOEF");
    if ( AttenuationLengthVector ){
      AttenuationLength = AttenuationLengthVector->
	Value(thePhotonEnergy);
    }
    else {
    }
  }
  else {
  }
  
  return AttenuationLength;
}
