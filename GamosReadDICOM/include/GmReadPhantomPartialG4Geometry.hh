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
#ifndef GmReadPhantomPartialG4Geometry_h
#define GmReadPhantomPartialG4Geometry_h 1

#include "globals.hh"
#include "GmReadPhantomGeometry.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include <map>
#include <set>

class G4PartialPhantomParameterisation;
class G4LogicalVolume;


class GmReadPhantomPartialG4Geometry : public GmReadPhantomGeometry
{
public:

  GmReadPhantomPartialG4Geometry();
  ~GmReadPhantomPartialG4Geometry();

  virtual void ReadPS( GmFileIn& ){};

private:
  virtual void ReadPhantomData();
  virtual void ConstructPhantom(G4LogicalVolume* parentLV);

  void ReadVoxelDensitiesPartial( std::ifstream& fin, std::map< G4int, std::map< G4int, G4int > > ifxmin, std::map< G4int, std::map< G4int, G4int > > ifxmax );

  std::pair<G4double,G4double> ReadVoxelDim( G4int nVoxel, std::ifstream& fin ); 
  void ReadVoxelDensitiesPartial( std::ifstream& fin );

  G4PartialPhantomParameterisation* thePartialPhantomParam;

  std::multimap<G4int,G4int> theFilledIDs;
  std::map< G4int, std::map< G4int, G4int > > theFilledMins;
  std::map< G4int, std::map< G4int, G4int > > theFilledMaxs;
  G4int theNVoxels;
};

#endif
