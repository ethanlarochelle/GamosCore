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
#ifndef GmRecHitsHistosUA_HH
#define GmRecHitsHistosUA_HH

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "G4ThreeVector.hh"
#include <set>
#include "globals.hh"
class GmAnalysisMgr;

class GmRecHit;

class GmRecHitsHistosUA: public GmUserRunAction, public GmUserEventAction
{
public:
  GmRecHitsHistosUA();
  ~GmRecHitsHistosUA();

  virtual void BeginOfRunAction(const G4Run* );
  virtual void EndOfRunAction(const G4Run* );
  virtual void EndOfEventAction(const G4Event* );

  void BookHistos();
  void SumHistos();

private:
  G4int GetHitTypeInt(const GmRecHit* hit );
  G4ThreeVector GetEDepoWidth( GmRecHit* hit );

private:
  std::set<G4String> theSDTypes;
  GmAnalysisMgr* theAnaMgr;
};

#endif
