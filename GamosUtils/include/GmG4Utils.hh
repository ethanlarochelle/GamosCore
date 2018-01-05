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
#ifndef GmG4Utils_HH
#define GmG4Utils_HH
//
#include "globals.hh"
#include <vector>
#include <set>
#include <map>
#include "G4ParticleDefinition.hh"
#include "G4TrackVector.hh"
#include "G4ThreeVector.hh"
class G4NavigationHistory;
class G4VProcess;
class G4Step;
class G4SteppingManager;
class G4Material;
class G4Ions;
class G4Nucleus;

class GmG4Utils {
public:
  static G4TrackVector GetSecondariesOfCurrentStep();
  static G4TrackVector GetSecondariesOfCurrentStep(G4SteppingManager* fpSteppingManager );

  static G4bool IsLightMeson( G4ParticleDefinition* partDef );
  static G4bool IsCharmMeson( G4ParticleDefinition* partDef );
  static G4bool IsBottomMeson( G4ParticleDefinition* partDef );
  static G4bool IsLightBaryon( G4ParticleDefinition* partDef );
  static G4bool IsStrangeBaryon( G4ParticleDefinition* partDef );
  static G4bool IsCharmBaryon( G4ParticleDefinition* partDef );
  static G4bool IsIon( G4ParticleDefinition* partDef );
  
  static G4ThreeVector GetLocalFromGlobalPos( const G4ThreeVector globalPos, const G4NavigationHistory* touch );
  static G4ThreeVector GetLocalFromGlobalDir( const G4ThreeVector globalDir, const G4NavigationHistory* navHis );

  static G4String GetInelasticName( const G4Step* aStep );
  static G4String GetInelasticName( std::vector<G4String> secos, const G4ParticleDefinition* primaryParticle, const G4Nucleus* targetNucleus = 0 );
  static G4String GetInelasticName( std::vector<G4ParticleDefinition*> secos, const G4ParticleDefinition* projectile, const G4Nucleus* targetNucleus = 0 );
  static G4String GetInelasticName( std::map<G4String,G4int> secosN, const G4ParticleDefinition* primaryParticle, const G4Nucleus* targetNucleus = 0 );
  static G4String GetNucleusNameFromSecos(  std::map<G4String,G4int> channelParticleList, const G4ParticleDefinition* primaryParticle );

  static std::map<G4int,G4String> theElements;
  static std::map<G4String,G4int> theElementsInv;
  static void BuildElementNames();
  static G4String GetElementNameFromZ(G4int Z);
  static G4int GetElementZFromName(G4String name);
  static G4bool CheckProcessExists( G4ProcessManager* pmanager, const G4String& procName, G4bool bWarning = false );
  static std::vector<G4Material*> GetG4MaterialList( const G4String& materialName, G4bool bMustExist );
  static G4String GetParticleShortName( G4String name );

};

#endif
