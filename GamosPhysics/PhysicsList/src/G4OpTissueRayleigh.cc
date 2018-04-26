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
// Optical Photon Rayleigh Scattering Class Implementation
////////////////////////////////////////////////////////////////////////
//
// File G4OpTissueRayleigh.hh
// Description: Discrete Process -- Rayleigh Scattering of Optical Photons
// Created: 2013-02-22
// Author: Adam Glaser
// Based on work from Julient Armstrong, Xin Qian, and Peter Gumplinger
//
// This subroutine will perform optical scattering based upon
// a modified rayleigh phase function.  The phase function is uniformly
// distributed for cos(theta).
//
// mail:  adam.k.glaser@dartmouth.edu
//
////////////////////////////////////////////////////////////////////////

#include "G4OpTissueRayleigh.hh"
#include "G4ios.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpProcessSubType.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

G4OpTissueRayleigh::G4OpTissueRayleigh(const G4String& processName, G4ProcessType type)
           : G4VDiscreteProcess(processName, type)
{
        SetProcessSubType(fOpRayleigh);

        thePhysicsTable = 0;

        DefaultWater = false;

        if (verboseLevel>0) {
           G4cout << GetProcessName() << " is created " << G4endl;
        }

        BuildThePhysicsTable();
}

G4OpTissueRayleigh::~G4OpTissueRayleigh()
{
        if (thePhysicsTable!= 0) {
           thePhysicsTable->clearAndDestroy();
           delete thePhysicsTable;
        }
}

////////////
// Methods
////////////

G4VParticleChange*
G4OpTissueRayleigh::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{

		const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();
	    const G4Material* aMaterial = aTrack.GetMaterial();

	    G4double thePhotonEnergy = aParticle->GetTotalEnergy();

	    G4double AttenuationLength = DBL_MAX;

	    G4MaterialPropertiesTable* aMaterialPropertyTable =
	      aMaterial->GetMaterialPropertiesTable();

	    if (aMaterialPropertyTable) {
	       G4MaterialPropertyVector* AttenuationLengthVector =
	                             aMaterialPropertyTable->GetProperty("RAYLEIGH_SCATCOEF");
	       if (AttenuationLengthVector) {
	          AttenuationLength = AttenuationLengthVector ->
	                                Value(thePhotonEnergy);
	       } else {
	       }
	    } else {
	    }
	
		aParticleChange.Initialize(aTrack);
		
		if (verboseLevel>0) {
	               G4cout << "Scattering Photon!" << G4endl;
	               G4cout << "Old Momentum Direction: "
	                      << aParticle->GetMomentumDirection() << G4endl;
	               G4cout << "Old Polarization: "
	                      << aParticle->GetPolarization() << G4endl;
	    }
	
      G4double cosTheta;
      G4ThreeVector OldMomentumDirection, NewMomentumDirection;
      G4ThreeVector OldPolarization, NewPolarization;

      do {
         // Try to simulate the scattered photon momentum direction
         // w.r.t. the initial photon momentum direction

	  G4double r = G4UniformRand();
	  G4double CosTheta = 2*r-1.; // uniformly distributed for cos(theta)

	  G4double Theta = std::acos(CosTheta);
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

	   // simulate according to the distribution cos^2(theta)
	      cosTheta = NewPolarization.dot(OldPolarization);
	   } while (std::pow(cosTheta,2) < G4UniformRand());

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

void G4OpTissueRayleigh::BuildThePhysicsTable()
{
//      Builds a table of scattering lengths for each material

        if (thePhysicsTable) return;

        const G4MaterialTable* theMaterialTable=
                               G4Material::GetMaterialTable();
        G4int numOfMaterials = G4Material::GetNumberOfMaterials();
		
        // create a new physics table

        thePhysicsTable = new G4PhysicsTable(numOfMaterials);

        // loop for materials

        for (G4int i=0 ; i < numOfMaterials; i++)
        {
            G4PhysicsOrderedFreeVector* ScatteringLengths = NULL;
			
            G4MaterialPropertiesTable *aMaterialPropertiesTable =
                         (*theMaterialTable)[i]->GetMaterialPropertiesTable();
                                                                                
            if(aMaterialPropertiesTable){

              G4MaterialPropertyVector* AttenuationLengthVector =
                            aMaterialPropertiesTable->GetProperty("RAYLEIGH_SCATCOEF");

              if(!AttenuationLengthVector){

                if ((*theMaterialTable)[i]->GetName() == "Water")
                {
		   // Call utility routine to Generate
		   // Rayleigh Scattering Lengths

                   DefaultWater = true;

                   ScatteringLengths =
		   RayleighAttenuationLengthGenerator(aMaterialPropertiesTable);
                }
              }
	    }

	    thePhysicsTable->insertAt(i,ScatteringLengths);
        } 
}

//--------------------------------------------------------------------------//

G4double G4OpTissueRayleigh::GetMeanFreePath(const G4Track& aTrack,
                                     G4double ,
                                     G4ForceCondition* )
{
        const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();
        const G4Material* aMaterial = aTrack.GetMaterial();

        G4double thePhotonEnergy = aParticle->GetTotalEnergy();

        G4double AttenuationLength = DBL_MAX;

        if (aMaterial->GetName() == "Water" && DefaultWater){

           G4bool isOutRange;

           AttenuationLength =
                (*thePhysicsTable)(aMaterial->GetIndex())->
                           GetValue(thePhotonEnergy, isOutRange);
        }
        else {

           G4MaterialPropertiesTable* aMaterialPropertyTable =
                           aMaterial->GetMaterialPropertiesTable();

           if(aMaterialPropertyTable){
             G4MaterialPropertyVector* AttenuationLengthVector =
                   aMaterialPropertyTable->GetProperty("RAYLEIGH_SCATCOEF");
             if(AttenuationLengthVector){
               AttenuationLength = AttenuationLengthVector ->
                                    Value(thePhotonEnergy);
             }
             else{
             }
           }
           else{
           }
        }

        return AttenuationLength;
}

//--------------------------------------------------------------------------//

G4PhysicsOrderedFreeVector* 
G4OpTissueRayleigh::RayleighAttenuationLengthGenerator(G4MaterialPropertiesTable *aMPT) 
{
        // Physical Constants

        // isothermal compressibility of water
        G4double betat = 7.658e-23*m3/MeV;

        // K Boltzman
        G4double kboltz = 8.61739e-11*MeV/kelvin;

        // Temperature of water is 10 degrees celsius
        // conversion to kelvin:
        // TCelsius = TKelvin - 273.15 => 273.15 + 10 = 283.15
        G4double temp = 283.15*kelvin;

        // Retrieve vectors for refraction index
        // and photon energy from the material properties table

        G4MaterialPropertyVector* Rindex = aMPT->GetProperty("RINDEX");

        G4double refsq;
        G4double e;
        G4double xlambda;
        G4double c1, c2, c3, c4;
        G4double Dist;
        G4double refraction_index;

        G4PhysicsOrderedFreeVector *RayleighScatteringLengths = 
				new G4PhysicsOrderedFreeVector();

        if (Rindex ) {

           for (size_t i = 0; i < Rindex->GetVectorLength(); i++) {

                e = Rindex->Energy(i);

                refraction_index = (*Rindex)[i];

                refsq = refraction_index*refraction_index;
                xlambda = h_Planck*c_light/e;

	        if (verboseLevel>0) {
        	        G4cout << Rindex->Energy(i) << " MeV\t";
                	G4cout << xlambda << " mm\t";
		}

                c1 = 1 / (6.0 * pi);
                c2 = std::pow((2.0 * pi / xlambda), 4);
                c3 = std::pow( ( (refsq - 1.0) * (refsq + 2.0) / 3.0 ), 2);
                c4 = betat * temp * kboltz;

                Dist = 1.0 / (c1*c2*c3*c4);

	        if (verboseLevel>0) {
	                G4cout << Dist << " mm" << G4endl;
		}
                RayleighScatteringLengths->
			InsertValues(Rindex->Energy(i), Dist);
           }

        }

	return RayleighScatteringLengths;
}
