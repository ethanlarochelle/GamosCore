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
// Optical Photon User-Defined Scattering Class Implementation
////////////////////////////////////////////////////////////////////////
//
// File G4OpTissueScatter.hh
// Description: Discrete Process -- User Defined Scattering of Optical Photons
// Created: 2013-02-22
// Author: Adam Glaser
// Based on work from Xin Qian, and Peter Gumplinger
//
// This subroutine will perform optical scattering based upon
// a user defined 2D phase function.
//
// 2019-12-06 Added to GAMOS 6.1  
//
// mail:  adam.k.glaser@dartmouth.edu
//
////////////////////////////////////////////////////////////////////////

#include "G4OpTissueScatter.hh"
#include "G4ios.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpProcessSubType.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include <functional>
#include <iostream>

G4OpTissueScatter::G4OpTissueScatter(const G4String& processName, G4ProcessType type)
           : G4VDiscreteProcess(processName, type)
{
  SetProcessSubType(fOpRayleigh);

  if (verboseLevel>0) {
    G4cout << GetProcessName() << " is created " << G4endl;
  }
  
  ReadData();
}

G4OpTissueScatter::~G4OpTissueScatter()
{
}

////////////
// Methods
////////////

G4VParticleChange*
G4OpTissueScatter::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
		
		const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();
	    const G4Material* aMaterial = aTrack.GetMaterial();

	    G4double thePhotonEnergy = aParticle->GetTotalEnergy();

	    G4double AttenuationLength = DBL_MAX;

	    G4MaterialPropertiesTable* aMaterialPropertyTable =
	      aMaterial->GetMaterialPropertiesTable();

	    if (aMaterialPropertyTable) {
	       G4MaterialPropertyVector* AttenuationLengthVector =
	                             aMaterialPropertyTable->GetProperty("USER_SCATCOEF");
	       if (AttenuationLengthVector) {
	          AttenuationLength = AttenuationLengthVector ->
	                                Value(thePhotonEnergy);
	       } else {
	       }
	    } else {
	    }
		
	   if (verboseLevel>0) {
				G4cout << "Scattering Photon!" << G4endl;
				G4cout << "Old Momentum Direction: "
					   << aParticle->GetMomentumDirection() << G4endl;
				G4cout << "Old Polarization: "
					   << aParticle->GetPolarization() << G4endl;
		}

		G4ThreeVector OldMomentumDirection, NewMomentumDirection;
		G4ThreeVector OldPolarization, NewPolarization;
  
		G4double cosTheta = 0.;
		cosTheta = GenerateValue(aMaterial,thePhotonEnergy);

		   G4double Theta = std::acos(cosTheta);
		   G4double Phi = G4UniformRand()*2*pi;

		   NewMomentumDirection.set
					   (std::sin(Theta)*std::cos(Phi), std::sin(Theta)*std::sin(Phi), std::cos(Theta));

		   // Rotate the new momentum direction into global reference system
		   OldMomentumDirection = aParticle->GetMomentumDirection();
		   NewMomentumDirection.rotateUz(OldMomentumDirection);
		   NewMomentumDirection = NewMomentumDirection.unit();

		   // calculate the new polarization direction
		   // The new polarization needs to be in the same plane as the new
		   // momentum direction and the old polarization direction
		   OldPolarization = aParticle->GetPolarization();
		   G4double constant = -1./NewMomentumDirection.dot(OldPolarization);

		   NewPolarization = NewMomentumDirection + constant*OldPolarization;
		   NewPolarization = NewPolarization.unit();

		   // There is a corner case, where the Newmomentum direction
		   // is the same as oldpolariztion direction:
		   // random generate the azimuthal angle w.r.t. Newmomentum direction
		   if (NewPolarization.mag() == 0.) {
			  G4double rand = G4UniformRand()*2*pi;
			  NewPolarization.set(std::cos(rand),std::sin(rand),0.);
			  NewPolarization.rotateUz(NewMomentumDirection);
		   } else {
			  // There are two directions which are perpendicular
			  // to the new momentum direction
			  if (G4UniformRand() < 0.5) NewPolarization = -NewPolarization;
		   }

		aParticleChange.ProposePolarization(NewPolarization);
		aParticleChange.ProposeMomentumDirection(NewMomentumDirection);

		if (verboseLevel>0) {
				G4cout << "New Polarization: " 
					 << NewPolarization << G4endl;
				G4cout << "Polarization Change: "
					 << *(aParticleChange.GetPolarization()) << G4endl;  
				G4cout << "New Momentum Direction: " 
					 << NewMomentumDirection << G4endl;
				G4cout << "Momentum Change: "
					 << *(aParticleChange.GetMomentumDirection()) << G4endl; 
		}
		
        return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

//--------------------------------------------------------------------------//

G4double G4OpTissueScatter::GetMeanFreePath(const G4Track& aTrack,
                                     G4double ,
                                     G4ForceCondition* )
{
        const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();
        const G4Material* aMaterial = aTrack.GetMaterial();

        G4double thePhotonEnergy = aParticle->GetTotalEnergy();

        G4double AttenuationLength = DBL_MAX;

        G4MaterialPropertiesTable* aMaterialPropertyTable =
                        aMaterial->GetMaterialPropertiesTable();

        if(aMaterialPropertyTable){
          G4MaterialPropertyVector* AttenuationLengthVector =
                aMaterialPropertyTable->GetProperty("USER_SCATCOEF");
          if(AttenuationLengthVector){
            AttenuationLength = AttenuationLengthVector ->
                                 Value(thePhotonEnergy);
          }
          else{
          }
        }
        else{
        }

        return AttenuationLength;
}

//--------------------------------------------------------------------------//

G4double G4OpTissueScatter::GenerateValue(const G4Material* aMaterial, G4double waveLength) {
  
G4String theFileNamePrefix="phase_function";

wlMap = theData[aMaterial];
std::vector<G4double> wL;
wL = wLValues[aMaterial];

G4PhysicsOrderedFreeVector* costProb1;
G4PhysicsOrderedFreeVector* costProb2;
			
std::vector<G4double>::iterator ite, ind;

if( wL.front() - 1239.84187/waveLength/1000000. > 1.E-10 ){
		G4Exception((theFileNamePrefix+"_G4OpTissueScatter::GenerateValue").c_str(),
     	"Error in photon wavelength.",
     	FatalErrorInArgument,
       "Photon wavelength is below range in provided data file.");
}
if ( wL.back() - 1239.84187/waveLength/1000000. < -1.E-10 ){
	G4Exception((theFileNamePrefix+"G4OpTissueScatter::GenerateValue").c_str(),
     	"Error in photon wavelength.",
     	FatalErrorInArgument,
     	"Photon wavelength is above range in provided data file.");
}

ind = upper_bound (wL.begin(), wL.end(), 1239.84187/waveLength/1000000.);

G4int indLow = (ind - wL.begin());
G4int indUp = (ind - wL.begin());
indLow = indLow-1;

G4bool isOutRange;
G4double r = G4UniformRand();
G4double cosTheta = 0.;

if( std::abs(wL.front() - 1239.84187/waveLength/1000000.) < 1.E-10 ){
 	costProb1 = wlMap[0];
	cosTheta = costProb1->GetValue(r, isOutRange);
}
else if ( std::abs(wL.back() - 1239.84187/waveLength/1000000.) < 1.E-10 ){
	costProb1 = wlMap[wL.size()-1];
	cosTheta = costProb1->GetValue(r, isOutRange);
}
else {
	costProb1 = wlMap[indLow];
	costProb2 = wlMap[indUp];

	G4double cosTheta1 = costProb1->GetValue(r, isOutRange);
	G4double cosTheta2 = costProb2->GetValue(r, isOutRange);

	G4double wl1 = waveLengths[indLow];
	G4double wl2 = waveLengths[indUp];
	wl1=1239.84187/wl1/1000000.;
	wl2=1239.84187/wl2/1000000.;
	waveLength=1239.84187/waveLength/1000000.;
	
	// G4cout << "Data" << G4endl;
	// G4cout << wl1 << G4endl;
	// G4cout << wl2 << G4endl;
	// G4cout << waveLength << G4endl;
	// G4cout << cosTheta1 << G4endl;
	// G4cout << cosTheta2 << G4endl;
	// G4cout << cosTheta << G4endl;
	
    cosTheta = ((waveLength-wl1)*(cosTheta2-cosTheta1)/(wl2-wl1))+cosTheta1;
}

return cosTheta;

}

//--------------------------------------------------------------------------//

void G4OpTissueScatter::ReadData()
{
//  Builds a table of scattering lengths for each material
  const G4MaterialTable* theMaterialTable=
    G4Material::GetMaterialTable();
  G4int numOfMaterials = G4Material::GetNumberOfMaterials();
  G4String theFileNamePrefix="phase_function";

  // loop for materials
  for (G4int im=0 ; im < numOfMaterials; im++) {
	
	const G4Material* aMaterial = (*theMaterialTable)[im];
    G4MaterialPropertiesTable *aMaterialPropertiesTable =
      aMaterial->GetMaterialPropertiesTable();

    if(aMaterialPropertiesTable){

      G4MaterialPropertyVector* AttenuationLengthVector =
	  aMaterialPropertiesTable->GetProperty("USER_SCATCOEF");

      if(AttenuationLengthVector){
	
		G4String fileName = theFileNamePrefix+"."+aMaterial->GetName() + ".txt";
	    G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
	    fileName = GmGenUtils::FileInPath( path, fileName);
	    GmFileIn fin = GmFileIn::GetInstance(fileName);   

	    std::vector<G4double> values;
	    std::vector<G4String> wl;

		G4cout << "Reading " << fileName  << " ..." << G4endl; 
		
		unsigned int rows = 0;
				
	    for( ;; ){
	      if(! fin.GetWordsInLine( wl ) ) break;
	      if( wl[0] == ":COSTHETA" ) {
			if( wl.size() == 1) {
		  		G4Exception((theFileNamePrefix+"_G4OpTissueScatter::ReadData").c_str(),
			      	"Error in number of words in line",
			      	FatalErrorInArgument,
			      	"Line starting with ':COSTHETA' must contain the values");
			}
			G4double ctprev=-DBL_MAX;
			G4double ctnew=0.;
			G4double cmax=0.0;
		  	G4double cmin=0.0;
			for( unsigned int ii = 1; ii < wl.size(); ii++ ) {
				ctnew = GmGenUtils::GetValue(wl[ii]);
				if(ctprev >= ctnew){
	  				G4Exception((theFileNamePrefix+"_G4OpTissueScatter::ReadData").c_str(),
			      		"Error in cos(theta) definition.",
			      		FatalErrorInArgument,
			      		"cos(theta) values must be specified in ascending order.");
	  			}
	  			else{
			  		cosThetas.push_back(GmGenUtils::GetValue(wl[ii]));
				    if (GmGenUtils::GetValue(wl[ii])>cmax){
						cmax=GmGenUtils::GetValue(wl[ii]);
					}
					if (GmGenUtils::GetValue(wl[ii])<cmin){
						cmin=GmGenUtils::GetValue(wl[ii]);
					}
				}
				ctprev=ctnew;
			}

			if( cmax > 1. ){
				G4Exception((theFileNamePrefix+"_G4OpTissueScatter::ReadData").c_str(),
		      		"Error in cos(theta) definition.",
		      		FatalErrorInArgument,
		      		"cos(theta) values must be <= 1.0.");
			}
			if( cmin < -1.){
				G4Exception((theFileNamePrefix+"_G4OpTissueScatter::ReadData").c_str(),
		      		"Error in cos(theta) definition.",
		      		FatalErrorInArgument,
		      		"os(theta) values must be >= -1.0.");
			}

			G4cout << "cos(theta) entires read N = " << cosThetas.size() << G4endl;
	      	} 
			else if( wl[0] == ":WAVELENGTH" ) {
				if( wl.size() == 1) {
		  			G4Exception((theFileNamePrefix+"_G4OpTissueScatter::ReadData").c_str(),
			      		"Error in number of words in line",
			      		FatalErrorInArgument,
			      		"Line starting with ':WAVELENGTH' must contain the values");
				}
				G4double wlprev=DBL_MAX;
				G4double wlnew=0.;
				for( unsigned int ii = 1; ii < wl.size(); ii++ ) {
		  			wlnew = GmGenUtils::GetValue(wl[ii]);
		  			if(wlprev <= wlnew){
		  				G4Exception((theFileNamePrefix+"_G4OpTissueScatter::ReadData").c_str(),
				      		"Error in wavelengh definition.",
				      		FatalErrorInArgument,
				      		"Wavelengths must be specified in ascending order.");
		  			}
		  			else{
				  		waveLengths.push_back(GmGenUtils::GetValue(wl[ii]));
					}
					wlprev=wlnew;
				}
				G4cout << "Wavelength entires read N = " << waveLengths.size() << G4endl;
	      } 
		else {
			if( cosThetas.size() == 0 || waveLengths.size() == 0 ) {
		  		G4Exception((theFileNamePrefix+"_G4OpTissueScatter::ReadData").c_str(),
			      	"Error in number of words in line",
			      	FatalErrorInArgument,
			      	"No line starting with ':COSTHETA' or with ':WAVELENGTH' found before line with probability values");
			}
			for( unsigned int ii = 0; ii < wl.size(); ii++ ) {
		  		G4double val = GmGenUtils::GetValue(wl[ii]);
		  		values.push_back(val);
			}
			rows = rows+1;
	   }
	 }

	unsigned int cols = values.size()/rows;
	
	if( rows != waveLengths.size() || cols != cosThetas.size() ){
		G4Exception((theFileNamePrefix+"_G4OpTissueScatter::ReadData").c_str(),
	      "Error in number of words in line",
	      FatalErrorInArgument,
	    (G4String("Matrix is not transposed correctly or is the wrong size. The size of P[wavelength,cos(theta)] is ") + GmGenUtils::itoa(rows) + " x " + GmGenUtils::itoa(cols) + " but it should be " + GmGenUtils::itoa(waveLengths.size()) + " x " + GmGenUtils::itoa(cosThetas.size()) + ".").c_str());
	}
	
	G4cout << rows << G4endl;
	G4cout << cols << G4endl;
	
	 G4cout << fileName  << " successfully read." << G4endl; 

	  if( values.size() != cosThetas.size() * waveLengths.size() ) {
			G4Exception((theFileNamePrefix+"_G4OpTissueScatter::ReadData").c_str(),
		      "Error in number of words in line",
		      FatalErrorInArgument,
		    (G4String("Number of probability values = ") + GmGenUtils::itoa(values.size()) + " is not equal to number of costheta entries = " + GmGenUtils::itoa(cosThetas.size()) + " X number of wavelength = " + GmGenUtils::itoa(waveLengths.size()) + ". Please check that the probability entries are correct AND that the supplied .txt file ends with the tag :ENDFILE.").c_str());
	  }

		G4cout << "Calculating cumulative probability distributions..." << G4endl; 

	    //--- Get the inverse of probabilities - energies distribution
	    G4int nWaveLengths = waveLengths.size();
	    G4int nCosThetas = cosThetas.size();
	    for( int iw = 0; iw < nWaveLengths; iw++ ){
		  G4PhysicsOrderedFreeVector *costProb = 
						new G4PhysicsOrderedFreeVector();
	      G4double tp1 = 0.;
	      for( int ict = 0; ict < nCosThetas; ict++ ){
			G4int idata = iw*nCosThetas + ict;
			tp1 += values[idata]; // calculate sum of probabilities to normalize 
	      }
		  if( tp1 != 1. ) G4cerr << theFileNamePrefix << "_G4OpTissueScatter::ReadData. Probabilities do not sum up to 1.0, but to " << tp1 << ". They will be normalized to 1.0 " << G4endl;
	      G4double maxProbInv = 1./tp1;
	      G4double tp = 0.;
	      for( int ict = 0; ict < nCosThetas; ict++ ){
			G4int idata = iw*nCosThetas + ict;
			G4double prob = values[idata];
			if( prob == 0. && ict == nCosThetas - 1 ) {
		  		prob = 1.E-10; // for last bin limit the probability is set to 0. by convention
			}
			tp += prob * maxProbInv;
			costProb->InsertValues(tp,cosThetas[ict]);
	      }
	    //thePhysicsTable->insertAt(iw , costProb);
		wlMap[iw]=costProb;
	    }
	    theData[(*theMaterialTable)[im]] = wlMap;

		std::vector<G4double> wL;
		std::vector<G4double>::iterator ite;
		for(ite = waveLengths.begin(); ite != waveLengths.end(); ite++) {
			wL.push_back(1239.84187/(*ite)/1000000.);
		}
		wLValues[(*theMaterialTable)[im]] = wL;
		wL.clear();
	    G4cout << "Adding " << (*theMaterialTable)[im]->GetName() << " scattering phase function to physics table with N data entries = " << nWaveLengths*nCosThetas << G4endl;
	
      }
    }
  }

}

//--------------------------------------------------------------------------//