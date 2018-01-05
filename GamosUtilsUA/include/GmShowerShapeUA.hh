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
#ifndef GmShowerShapeUA_hh
#define GmShowerShapeUA_hh

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"

#include <set>
#include <map>
#include <iostream>
#include "GmSSData.hh"

enum  SSIT{ SSITPre, SSITPost,  SSITLinear };
  
class GmShowerShapeUA : public GmUserRunAction, 
			public GmUserSteppingAction, 
			public GmUserEventAction, 
			public GmVHistoBuilder
{
public:
  GmShowerShapeUA();
  ~GmShowerShapeUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfEventAction(const G4Event* );
  virtual void InitialiseHistoNames();

  GmSSData* CheckWhichShower( const G4Step* aStep );

private:
  void BookHistos(G4int index);

  G4ThreeVector GetShowerDirection( GmSSData* ssdata);
  void DumpText( const G4Event* evt  );
  void DumpBin( const G4Event* evt  );
 
private:
  std::map<G4int, GmSSData*> theShowerData;

  TSPUse thePointToUse;
  SSIT theInterpolationType;
  G4bool bShowerDirection1stTrack;
  G4bool bUsePostInFirstTrack;
  G4bool bIncludeOtherStepsOfFirstTrack;

  std::set<G4double> theRadii;

  std::set<G4int> theHistos;

  G4String theTextFileName;
  std::ostream* theTextFileOut;

  G4String theBinFileName;
  FILE* theBinFileOut;

};

#endif
