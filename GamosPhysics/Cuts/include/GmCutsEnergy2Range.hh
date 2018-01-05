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
#ifndef GmCutsEnergy2Range__HH
#define GmCutsEnergy2Range__HH

class G4MaterialCutsCouple; 
class G4Material; 
class G4ParticleDefinition;
class G4RToEConvForGamma;
class G4RToEConvForElectron;
class G4RToEConvForPositron;
class G4VRangeToEnergyConverter;

#include "globals.hh"

class GmCutsEnergy2Range
{
public:
  GmCutsEnergy2Range();
  ~GmCutsEnergy2Range();

  G4double Convert( G4double ener, const G4ParticleDefinition* part, const G4MaterialCutsCouple* mate );
  G4double ConvertSlow( G4double ener, const G4ParticleDefinition* part, const G4MaterialCutsCouple* cutsCouple );
  G4double ConvertSlow( G4double ener, const G4ParticleDefinition* part, const G4Material* mate );
  G4double Convert( G4VRangeToEnergyConverter* R2E, G4double ener, const G4ParticleDefinition* part, const G4MaterialCutsCouple* cutsCouple);
  G4double Convert( G4VRangeToEnergyConverter* R2E, G4double ener, const G4ParticleDefinition* part, const G4Material* mate);

private:
  G4RToEConvForGamma * theR2EGamma;
  G4RToEConvForElectron * theR2EElectron;
  G4RToEConvForPositron * theR2EPositron;
};

#endif
