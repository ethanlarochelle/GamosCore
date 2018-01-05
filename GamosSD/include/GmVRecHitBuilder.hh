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
#ifndef GmVRecHitBuilderizer_HH
#define GmVRecHitBuilderizer_HH

#include <vector>
#include "globals.hh"

class GmRecHit;
class GmVNumericDistribution;
class GmHitsEventMgr;

enum MEBehaviour{MEAcceptAll, MEDeleteSmall, MEAcceptIf1Big, MEDeleteIf1Small};

class GmVRecHitBuilder
{

public:
  
  GmVRecHitBuilder();
  virtual ~GmVRecHitBuilder(){};

  void SetSDType(const G4String& sdtype);
  void SetName( const G4String& name ){
    theName = name; }
  void SetParams();

  virtual void CleanRecHits() = 0; //cannot be done here, because there is no info about GmRecHit

  virtual void DeleteHit( GmRecHit* rhit ) = 0; //cannot be done here, because there is no info about GmRecHit

  virtual void CheckRecHitsMinEnergy();
  virtual void CheckEnergyEfficiency();

  virtual void SmearRecHitsEnergy();
  virtual void SmearRecHitsTime();

protected:
  std::vector<GmRecHit*> theRecHits;

  G4String theName;
  G4String theSDType;

  MEBehaviour theMEBehaviour;
  G4double theMinHitEnergy;
  G4double theMinRecHitEnergy;
  GmHitsEventMgr* theHitsEventMgr;
  GmVNumericDistribution* theEfficiencyDistribution;

  G4double theEnergyResol;
  G4double theEnergyResolFluct;
  G4double theEnergyResolInstr;
  G4double theTimeResol;

};

#endif
