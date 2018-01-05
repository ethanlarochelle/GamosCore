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
#ifndef GmGetParticleMgr_hh
#define GmGetParticleMgr_hh

#include "globals.hh"
#include <vector>
class G4ParticleDefinition;
class G4Ions;
class G4VProcess;

class GmGetParticleMgr
{
private:
  GmGetParticleMgr();

public:
  ~GmGetParticleMgr();

  static GmGetParticleMgr* GetInstance();

  std::vector<G4ParticleDefinition*> GetG4ParticleList( const G4String& particleName, G4bool bMustExist = 1 );
  G4ParticleDefinition* GetG4Particle(const G4String& particleName, G4bool bMustExist = 1);
  G4ParticleDefinition* GetG4Particle(const G4VProcess* proc, G4bool bMustExist = 1 );
  G4Ions* CreateIon( const G4String& newValues );
  G4Ions* CreateIon( G4int atomicNumber,
		     G4int atomicMass,
		     G4double energy,
		     G4int excitNumber);

  std::vector<G4VProcess*> GetG4ProcessList(const G4String& particleName, const G4String& processName, G4bool bMustExist = 1 );
  std::vector<G4VProcess*> GetG4ProcessList(const G4String& processName, G4bool bMustExist = 1 );
  G4String GetG4ProcessName(const G4String& particleName, G4String processName );

private:
  static GmGetParticleMgr* theInstance;
};

#endif
