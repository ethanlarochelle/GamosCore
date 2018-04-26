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
// a user defined phase function.
//
// mail:  adam.k.glaser@dartmouth.edu
//
////////////////////////////////////////////////////////////////////////
#ifndef G4OpTissueScatter_h
#define G4OpTissueScatter_h 1

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
#include <set>

/////////////////////
// Class Definition
/////////////////////

class G4OpTissueScatter : public G4VDiscreteProcess 
{

private:

public:
 
        G4OpTissueScatter(const G4String& processName = "OpTissueScatter",
                              G4ProcessType type = fOptical);

		~G4OpTissueScatter();

////////////
// Methods
////////////

public:

        G4bool IsApplicable(const G4ParticleDefinition& aParticleType);

        G4double GetMeanFreePath(const G4Track& aTrack,
				 G4double , G4ForceCondition* );

        G4VParticleChange* PostStepDoIt(const G4Track& aTrack,
                                       const G4Step&  aStep);
	
	    void ReadData();
	
	    G4double GenerateValue(const G4Material* aMaterial, G4double waveLength);

private:

protected:
		std::vector<G4Material*> theMaterials;

private:
  G4String theFileName;
  G4String theFileNamePrefix;
  std::vector<G4double> waveLengths;
  std::vector<G4double> cosThetas;
  std::map<const G4Material*, std::map<G4int, G4PhysicsOrderedFreeVector*> > theData;
  std::set<const G4Material*> bUseScatCoef;
  std::map<G4int, G4PhysicsOrderedFreeVector*> wlMap;
  std::map<const G4Material*, std::vector<G4double> > wLValues;
};

////////////////////
// Inline methods
////////////////////

inline
G4bool G4OpTissueScatter::IsApplicable(const G4ParticleDefinition& aParticleType)
{
  return ( &aParticleType == G4OpticalPhoton::OpticalPhoton() );
}

#endif /* G4OpTissueScatter_h */
