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
#ifndef GmBuildPhantomStructuresUA__HH
#define GmBuildPhantomStructuresUA__HH

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include <vector>
#include <fstream>
#include "G4PhantomParameterisation.hh"
#include "G4ThreeVector.hh"
class G4VPhysicalVolume;
class G4PhantomParameterisation;
class GmAnalysisMgr;

struct PStPlaneData {
  std::vector< std::vector<G4double>* > thePointV; // 3 sets of points (x,y,z)
};
struct PStructureData {
  G4String theName;
  G4int theNumber;
  std::vector<PStPlaneData*>* thePlanes;
};

class GmBuildPhantomStructuresUA : public GmUserRunAction
{
public:
  GmBuildPhantomStructuresUA();
  ~GmBuildPhantomStructuresUA(){};
  virtual void BeginOfRunAction( const G4Run* );

private:
  void InitialiseDoseCounters();
  void Write3ddose();
  void FillDoseHistos();

private:
  GmAnalysisMgr* theAnaMgr;

  G4VPhysicalVolume* thePhantomStructure;

  std::ofstream* theDoseFile;

  G4PhantomParameterisation* theRegularParam;

  G4ThreeVector thePhantomMinusCorner;
};

#endif
