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
#ifndef __GmProdCutsForAllProcessesUA__
#define __GmProdCutsForAllProcessesUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"
#include <map>

class G4Run;
class G4Track;
class G4LogicalVolume;

class GmProdCutsForAllProcessesUA : public GmUserRunAction, public GmUserStackingAction
{
public:
  GmProdCutsForAllProcessesUA();
  ~GmProdCutsForAllProcessesUA(){};
  virtual void BeginOfRunAction(const G4Run* );

  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);

private:
  std::map<G4LogicalVolume*,G4double> theGammaCuts;
  std::map<G4LogicalVolume*,G4double> theElectronCuts;
  std::map<G4LogicalVolume*,G4double> thePositronCuts;


};

#endif
