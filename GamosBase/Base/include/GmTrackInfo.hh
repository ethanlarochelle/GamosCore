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
// --------------------------------------------------------------
#ifndef GmTrackInfo_hh
#define GmTrackInfo_hh

#include "globals.hh"
#include "G4VUserTrackInformation.hh"
#include "G4ThreeVector.hh"
#include <map>
class G4ParticleDefinition;

class GmTrackInfo : public G4VUserTrackInformation
{
public:

  GmTrackInfo( const G4String& type = "GmTrackInfo" );
  
  virtual ~GmTrackInfo();

  void Print() const {};

  G4bool BoolValueExists( const G4String& key );
  G4bool IntValueExists( const G4String& key );
  G4bool DoubleValueExists( const G4String& key );
  G4bool StringValueExists( const G4String& key );
  G4bool ThreeVectorValueExists( const G4String& key );

  G4bool GetBoolValue( const G4String& key );
  G4int GetIntValue( const G4String& key );
  G4double GetDoubleValue( const G4String& key );
  G4String GetStringValue( const G4String& key );
  G4ThreeVector GetThreeVectorValue( const G4String& key );

  void SetBoolValue( const G4String& key, G4bool value );
  void SetIntValue( const G4String& key, G4int value );
  void SetDoubleValue( const G4String& key, G4double value );
  void SetStringValue( const G4String& key, G4String value );
  void SetThreeVectorValue( const G4String& key, G4ThreeVector value );
  
  // special treatment to weight
  void SetWeight( G4double wei ) {
    theWeight = wei; 
  }
  G4double GetWeight() const {
    return theWeight; 
  }

  // special treatment to particle 
  G4ParticleDefinition* GetParticle() const {
    return theParticle;  
  }
  void SetParticle( G4ParticleDefinition* part ) {
    theParticle = part;
  }

private:
  std::map<G4String,G4bool> theBoolValues;
  std::map<G4String,G4int> theIntValues;
  std::map<G4String,G4double> theDoubleValues;
  std::map<G4String,G4String> theStringValues;
  std::map<G4String,G4ThreeVector> theThreeVectorValues;

  G4double theWeight;
  G4ParticleDefinition* theParticle;

};

#endif
