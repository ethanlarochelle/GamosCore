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
// Optical Photon Modified Rayleigh Scattering Class Implementation
////////////////////////////////////////////////////////////////////////
//
// File G4OpTissueRayleighMHG.hh
// Description: Discrete Process -- Modifed Rayleigh Scattering of Optical Photons
// Created: 2013-02-22
// Author: Adam Glaser
// Based on work from Julient Armstrong, Xin Qian, and Peter Gumplinger
//
// This subroutine will perform optical scattering based upon
// a modified rayleigh phase function.  The phase function is proportional
// to cos(theta)^2.
//
// 2019-12-06 Added to GAMOS 6.1  
//
// mail:  adam.k.glaser@dartmouth.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef G4OpTissueRayleighMHG_h
#define G4OpTissueRayleighMHG_h 1

/////////////
// Includes
/////////////

#include "globals.hh"
#include "templates.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleMomentum.hh"
#include "G4Step.hh"
#include "G4VDiscreteProcess.hh"
#include "G4DynamicParticle.hh"
#include "G4Material.hh"
#include "G4OpticalPhoton.hh"
#include "G4PhysicsTable.hh"
#include "G4PhysicsOrderedFreeVector.hh"

// Class Description:
// Discrete Process -- Rayleigh scattering of optical photons.
// Class inherits publicly from G4VDiscreteProcess.
// Class Description - End:

/////////////////////
// Class Definition
/////////////////////

class G4OpTissueRayleighMHG : public G4VDiscreteProcess 
{

private:
 
        //////////////
        // Operators
        //////////////

        // G4OpTissueRayleighMHG& operator=(const G4OpTissueRayleighMHG &right);

public: // Without description

        ////////////////////////////////
        // Constructors and Destructor
        ////////////////////////////////
 
        G4OpTissueRayleighMHG(const G4String& processName = "OpTissueRayleighMHG",
                              G4ProcessType type = fOptical);

        // G4OpTissueRayleighMHG(const G4OpTissueRayleighMHG &right);

	~G4OpTissueRayleighMHG();

        ////////////
        // Methods
        ////////////

public: // With description

        G4bool IsApplicable(const G4ParticleDefinition& aParticleType);
        // Returns true -> 'is applicable' only for an optical photon.

        G4double GetMeanFreePath(const G4Track& aTrack,
				 G4double ,
                                 G4ForceCondition* );
        // Returns the mean free path for Rayleigh scattering in water.
        // --- Not yet implemented for other materials! ---

        G4VParticleChange* PostStepDoIt(const G4Track& aTrack,
                                       const G4Step&  aStep);
        // This is the method implementing Rayleigh scattering.

        G4PhysicsTable* GetPhysicsTable() const;
        // Returns the address of the physics table.

        void DumpPhysicsTable() const;
        // Prints the physics table.

private:

        void BuildThePhysicsTable();

        /////////////////////
        // Helper Functions
        /////////////////////

	G4PhysicsOrderedFreeVector* RayleighAttenuationLengthGenerator(
					G4MaterialPropertiesTable *aMPT);

        ///////////////////////
        // Class Data Members
        ///////////////////////

protected:

        G4PhysicsTable* thePhysicsTable;
        //  A Physics Table can be either a cross-sections table or
        //  an energy table (or can be used for other specific
        //  purposes).

private:

        G4bool DefaultWater;

};

////////////////////
// Inline methods
////////////////////

inline
G4bool G4OpTissueRayleighMHG::IsApplicable(const G4ParticleDefinition& aParticleType)
{
  return ( &aParticleType == G4OpticalPhoton::OpticalPhoton() );
}

inline
void G4OpTissueRayleighMHG::DumpPhysicsTable() const

{
        G4int PhysicsTableSize = thePhysicsTable->entries();
        G4PhysicsOrderedFreeVector *v;

        for (G4int i = 0 ; i < PhysicsTableSize ; i++ )
        {
                v = (G4PhysicsOrderedFreeVector*)(*thePhysicsTable)[i];
                v->DumpValues();
        }
}

inline G4PhysicsTable* G4OpTissueRayleighMHG::GetPhysicsTable() const
{
  return thePhysicsTable;
}


#endif /* G4OpTissueRayleighMHG_h */
