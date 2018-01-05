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
#ifndef GmGenerator_HH
#define GmGenerator_HH

class G4Event;
class GmParticleSource;
class GmIsotopeMgr;

#include "globals.hh"
#include <vector>
#include <iostream>
#include "G4VUserPrimaryGeneratorAction.hh"

class GmGenerator:public G4VUserPrimaryGeneratorAction
{
public:
  GmGenerator();
  ~GmGenerator();
  void GeneratePrimaries(G4Event* evt);

  void AddIsotopeSource( const G4String& newValues );
  void AddSingleParticleSource( const G4String& newValues );
  void AddDoubleBackToBackParticleSource( const G4String& newValues );

  GmParticleSource* FindSource( const G4String& name, G4bool mustExist = TRUE );
  void SetDistribution( const G4String& distType, std::vector<G4String>& wl );

  void ListSources( std::ostream& out = G4cout );

  void DeleteSources();

  void ReInitialiseDistributions();

  G4double GetLastEventTime() const {
   return theLastEventTime; 
  }

  void AddBiasDistribution( const G4String& sourceName, const G4String& varName, const G4String& biasDistName );
  void BiasTime(GmParticleSource* source, G4double& time);

  std::vector<GmParticleSource*> GetSources() const {
    return theSources; }

protected:
  std::vector<GmParticleSource*> theSources;
  GmIsotopeMgr* theIsotopeMgr;
  G4double theLastEventTime;

  G4bool bBiasDistributions;
};

#endif
