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
#ifndef GmUserLimitsMgr_H
#define GmUserLimitsMgr_H

#include <vector>
#include <map>
#include "globals.hh"
class G4UserLimits;
class G4LogicalVolume;
class G4ParticleDefinition;
class G4RToEConvForGamma;
class G4RToEConvForElectron;
class G4RToEConvForPositron;

class GmUserLimitsMgr
{
private:
  GmUserLimitsMgr();

public:
  static GmUserLimitsMgr* GetInstance();

  ~GmUserLimitsMgr();

  void CreateUserLimitsAll( std::vector<G4String>& wl );
  void CreateUserLimitsMaxStep( std::vector<G4String>& wl );
  void CreateUserLimitsMaxTrkLen( std::vector<G4String>& wl );
  void CreateUserLimitsMaxTOF( std::vector<G4String>& wl );
  void CreateUserLimitsMinEKin( std::vector<G4String>& wl );
  void CreateUserLimitsMinEKinByRange( std::vector<G4String>& wl );
  void CreateUserLimitsMinRange( std::vector<G4String>& wl );
  void AddLVAndParticle( std::vector<G4String>& wl );

  G4UserLimits* GetUserLimits( G4LogicalVolume* lv, G4ParticleDefinition* particle );

  void PrintUserLimits(std::ostream& out );

private:
  void CreateUserLimits( std::vector<G4String>& wl, G4bool bStepLimiter, G4bool bSpecialCuts );
  void AssociateLVAndParticle( G4String& lvName, G4String& partName, G4UserLimits* ul, G4bool bCheckRepeat = TRUE );
  void AssociateProcesses2Particle( G4String& partName, G4bool bStepLimiter, G4bool bSpecialCuts );
  void AssociateProcesses2Particle( G4ParticleDefinition* particle, G4bool bStepLimiter, G4bool bSpecialCuts );
  void InitialiseR2E();

private:
  static GmUserLimitsMgr* theInstance;
  std::map<G4String,G4UserLimits*> theUserLimits;
  typedef std::map<G4LogicalVolume*, std::map<G4ParticleDefinition*,G4UserLimits*>* > mlvpul;
  mlvpul theUserLimitsMap;

  G4RToEConvForGamma*  theR2EGamma;
  G4RToEConvForElectron* theR2EElectron;
  G4RToEConvForPositron* theR2EPositron;
};

#endif
