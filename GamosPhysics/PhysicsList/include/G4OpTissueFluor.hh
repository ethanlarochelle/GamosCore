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
//  2019-12-06 Added to GAMOS 6.1  
//
// mail:  adam.k.glaser@dartmouth.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef G4OpTissueFluor_h
#define G4OpTissueFluor_h 1

/////////////
// Includes
/////////////

#include "globals.hh"
#include "templates.hh"
#include "Randomize.hh"
#include "G4Poisson.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleMomentum.hh"
#include "G4Step.hh"
#include "G4VDiscreteProcess.hh"
#include "G4DynamicParticle.hh"
#include "G4Material.hh"
#include "G4OpticalPhoton.hh"
#include "G4PhysicsTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4PhysicsOrderedFreeVector.hh"
#include "G4VWLSTimeGeneratorProfile.hh"

// Class Description:
// Discrete Process -- Bulk absorption of Optical Photons.
// Class inherits publicly from G4VDiscreteProcess
// Class Description - End:

/////////////////////
// Class Definition
/////////////////////

class G4VWLSTimeGeneratorProfile;

class G4OpTissueFluor : public G4VDiscreteProcess 
{

public: // Without description

  ////////////////////////////////
  // Constructors and Destructor
  ////////////////////////////////

  G4OpTissueFluor(const G4String& processName = "OpTissueFluor",
                   G4ProcessType type = fOptical);

  ~G4OpTissueFluor();

  ////////////
  // Methods
  ////////////

public: // With description

  G4bool IsApplicable(const G4ParticleDefinition& aParticleType);
  // Returns true -> 'is applicable' only for an optical photon.

  G4double GetMeanFreePath(const G4Track& aTrack,
			   G4double ,
			   G4ForceCondition* );
  // Returns the absorption length for bulk absorption of optical
  // photons in media with a specified attenuation length.

  G4VParticleChange* PostStepDoIt(const G4Track& aTrack,
				  const G4Step&  aStep);
  // This is the method implementing bulk absorption of optical
  // photons.

  G4PhysicsTable* GetIntegralTable() const;
  // Returns the address of the WLS integral table.

  void DumpPhysicsTable() const;
  // Prints the WLS integral table.

  void UseTimeProfile(const G4String name);
  // Selects the time profile generator

private:
  
  void BuildThePhysicsTable();
  std::map<G4double,G4double> emissionprob;
  std::map<G4int, std::map<G4double,G4double> > emissionprobmap;
  // Is the WLS integral table;

protected:

  G4VWLSTimeGeneratorProfile* WLSTimeGeneratorProfile;
  G4PhysicsTable* theIntegralTable;

};

////////////////////
// Inline methods
////////////////////

inline
G4bool G4OpTissueFluor::IsApplicable(const G4ParticleDefinition& aParticleType)
{
   return ( &aParticleType == G4OpticalPhoton::OpticalPhoton() );
}

inline
G4PhysicsTable* G4OpTissueFluor::GetIntegralTable() const
{
  return theIntegralTable;
}

inline
void G4OpTissueFluor::DumpPhysicsTable() const
{
  G4int PhysicsTableSize = theIntegralTable->entries();
  G4PhysicsOrderedFreeVector *v;
 
  for (G4int i = 0 ; i < PhysicsTableSize ; i++ )
    {
      v = (G4PhysicsOrderedFreeVector*)(*theIntegralTable)[i];
      v->DumpValues();
    }
}

#endif /* G4OpTissueFluor_h */

