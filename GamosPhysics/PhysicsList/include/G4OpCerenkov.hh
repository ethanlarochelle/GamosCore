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
// Cerenkov Radiation Class Implementation
////////////////////////////////////////////////////////////////////////
//
// File:        G4OpCerenkov.hh
// Description: Discrete Process -- Generation of Cerenkov Photons
// Version:     2.1
// Created:     1996-02-21  
// Author:      Juliet Armstrong
// Updated:     2007-09-30 by Peter Gumplinger
//              > change inheritance to G4VDiscreteProcess
//              GetContinuousStepLimit -> GetMeanFreePath (StronglyForced)
//              AlongStepDoIt -> PostStepDoIt
//              2005-08-17 by Peter Gumplinger
//              > change variable name MeanNumPhotons -> MeanNumberOfPhotons
//              2005-07-28 by Peter Gumplinger
//              > add G4ProcessType to constructor
//              2001-09-17, migration of Materials to pure STL (mma) 
//              2000-11-12 by Peter Gumplinger
//              > add check on CerenkovAngleIntegrals->IsFilledVectorExist()
//              in method GetAverageNumberOfPhotons 
//              > and a test for MeanNumberOfPhotons <= 0.0 in DoIt
//              2000-09-18 by Peter Gumplinger
//              > change: aSecondaryPosition=x0+rand*aStep.GetDeltaPosition();
//                        aSecondaryTrack->SetTouchable(0);
//              1999-10-29 by Peter Gumplinger
//              > change: == into <= in GetContinuousStepLimit
//              1997-08-08 by Peter Gumplinger
//              > add protection against /0
//              > G4MaterialPropertiesTable; new physics/tracking scheme
// 				> 2013-4-10 Modified for tissue-optics plugin to allow for
//				discrete and interpolated production of Cerenkov photon
//				wavelengths.
//
// mail:        adam.k.glaser@dartmouth.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef G4OpCerenkov_h
#define G4OpCerenkov_h 1

/////////////
// Includes
/////////////

#include "globals.hh"
#include "templates.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleMomentum.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4OpticalPhoton.hh"
#include "G4DynamicParticle.hh"
#include "G4Material.hh" 
#include "G4PhysicsTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4PhysicsOrderedFreeVector.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

// Class Description:
// Discrete Process -- Generation of Cerenkov Photons.
// Class inherits publicly from G4VDiscreteProcess.
// Class Description - End:

/////////////////////
// Class Definition
/////////////////////

class G4OpCerenkov : public G4VProcess
{

private:

        //////////////
        // Operators
        //////////////

	// G4OpCerenkov& operator=(const G4OpCerenkov &right);

public: // Without description

	////////////////////////////////
	// Constructors and Destructor
	////////////////////////////////

	G4OpCerenkov(const G4String& processName = "Cerenkov", 
                            G4ProcessType type = fElectromagnetic);

	// G4OpCerenkov(const G4OpCerenkov &right);

	~G4OpCerenkov();	

        ////////////
        // Methods
        ////////////

public: // With description

        G4bool IsApplicable(const G4ParticleDefinition& aParticleType);
        // Returns true -> 'is applicable', for all charged particles
        // except short-lived particles.

        G4double GetMeanFreePath(const G4Track& aTrack,
                                 G4double ,
                                 G4ForceCondition* );
        // Returns the discrete step limit and sets the 'StronglyForced'
        // condition for the DoIt to be invoked at every step.

        G4double PostStepGetPhysicalInteractionLength(const G4Track& aTrack,
                                                      G4double ,
                                                      G4ForceCondition* );
        // Returns the discrete step limit and sets the 'StronglyForced'
        // condition for the DoIt to be invoked at every step.

	G4VParticleChange* PostStepDoIt(const G4Track& aTrack, 
					const G4Step&  aStep);
        // This is the method implementing the Cerenkov process.

        //  no operation in  AtRestDoIt and  AlongStepDoIt
        virtual G4double AlongStepGetPhysicalInteractionLength(
                               const G4Track&,
                               G4double  ,
                               G4double  ,
                               G4double& ,
                               G4GPILSelection*
                              ) { return -1.0; };

        virtual G4double AtRestGetPhysicalInteractionLength(
                               const G4Track& ,
                               G4ForceCondition*
                              ) { return -1.0; };

        //  no operation in  AtRestDoIt and  AlongStepDoIt
        virtual G4VParticleChange* AtRestDoIt(
                               const G4Track& ,
                               const G4Step&
                              ) {return 0;};

        virtual G4VParticleChange* AlongStepDoIt(
                               const G4Track& ,
                               const G4Step&
                              ) {return 0;};

	void SetTrackSecondariesFirst(const G4bool state);
        // If set, the primary particle tracking is interrupted and any 
        // produced Cerenkov photons are tracked next. When all have 
        // been tracked, the tracking of the primary resumes. 
	
        void SetMaxBetaChangePerStep(const G4double d);
        // Set the maximum allowed change in beta = v/c in % (perCent)
        // per step.

	void SetMaxNumPhotonsPerStep(const G4int NumPhotons);
        // Set the maximum number of Cerenkov photons allowed to be 
        // generated during a tracking step. This is an average ONLY; 
        // the actual number will vary around this average. If invoked, 
        // the maximum photon stack will roughly be of the size set.
        // If not called, the step is not limited by the number of 
        // photons generated.

        G4PhysicsTable* GetPhysicsTable() const;
        // Returns the address of the physics table.

        void DumpPhysicsTable() const;
        // Prints the physics table.

private:

        void BuildThePhysicsTable();

	/////////////////////
	// Helper Functions
	/////////////////////

	G4double GetAverageNumberOfPhotons(const G4double charge,
                                const G4double beta,
		    		const G4Material *aMaterial,
				G4MaterialPropertyVector* Rindex) const;

        ///////////////////////
        // Class Data Members
        ///////////////////////

protected:

        G4PhysicsTable* thePhysicsTable;
	
        //  A Physics Table can be either a cross-sections table or
        //  an energy table (or can be used for other specific
        //  purposes).

private:

	G4bool fTrackSecondariesFirst;
        G4double fMaxBetaChange;
        G4int  fMaxPhotons;
	std::map<G4double,G4double> enerprob;
	std::map<G4int, std::map<G4double,G4double> > enerprobmap;
};

////////////////////
// Inline methods
////////////////////

inline 
G4bool G4OpCerenkov::IsApplicable(const G4ParticleDefinition& aParticleType)
{
   if (aParticleType.GetParticleName() == "chargedgeantino") return false;
   if (aParticleType.IsShortLived()) return false;

   return (aParticleType.GetPDGCharge() != 0);
}

inline 
void G4OpCerenkov::SetTrackSecondariesFirst(const G4bool state) 
{ 
	fTrackSecondariesFirst = state;
}

inline
void G4OpCerenkov::SetMaxBetaChangePerStep(const G4double value)
{
        fMaxBetaChange = value*perCent;
}

inline
void G4OpCerenkov::SetMaxNumPhotonsPerStep(const G4int NumPhotons) 
{ 
	fMaxPhotons = NumPhotons;
}

inline
void G4OpCerenkov::DumpPhysicsTable() const
{
        G4int PhysicsTableSize = thePhysicsTable->entries();
        G4PhysicsOrderedFreeVector *v;

        for (G4int i = 0 ; i < PhysicsTableSize ; i++ )
        {
        	v = (G4PhysicsOrderedFreeVector*)(*thePhysicsTable)[i];
        	v->DumpValues();
        }
}

inline
G4PhysicsTable* G4OpCerenkov::GetPhysicsTable() const
{
  return thePhysicsTable;
}

#endif /* G4OpCerenkov_h */
