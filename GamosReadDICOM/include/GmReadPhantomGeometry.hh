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
#ifndef GmReadPhantomGeometry_h
#define GmReadPhantomGeometry_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class G4Material;
class G4LogicalVolume;
class G4PhysicalVolume;
class G4Box;
class G4PhantomParameterisation;
#include <map>

struct matInfo 
{
  G4double sumdens;
  G4int nvoxels;
  G4int id;
};

class GmReadPhantomGeometry : public G4VUserDetectorConstruction
{
public:

  GmReadPhantomGeometry();
  ~GmReadPhantomGeometry();

  G4VPhysicalVolume* Construct();

  std::vector<G4Material*> GetPhantomMaterials() const {
    return thePhantomMaterials;
  }
  void SetPhantomMaterials( std::vector<G4Material*> pm ) {
    thePhantomMaterials = pm;
  }

  G4int GetNVoxelX() {
    return nVoxelX;
  }
  G4int GetNVoxelY() {
    return nVoxelY;
  }
    G4int GetNVoxelZ() {
    return nVoxelZ;
  }

  G4float GetMinX() const {
    return offsetX;
  }
  G4float GetMaxX() const {
    return offsetX+dimX*nVoxelX;
  }
  G4float GetMinY() const {
    return offsetY;
  }
  G4float GetMaxY() const {
    return offsetY+dimY*nVoxelY;
  }
  G4float GetMinZ() const {
    return offsetZ;
  }
  G4float GetMaxZ() const {
    return offsetZ+dimZ*nVoxelZ;
  }
  size_t* GetMateIDs() const {
    return theMateIDs;
  }
  float* GetMateDensities() const {
    return theMateDensities;
  }
  G4float GetVoxelHalfX() const {
    return dimX/2.;
  }
  G4float GetVoxelHalfY() const {
    return dimY/2.;
  }
  G4float GetVoxelHalfZ() const {
    return dimZ/2.;
  }

  G4int GetNVoxels() {
    return nVoxelX * nVoxelY * nVoxelZ;
  }
  
protected:
  virtual void ReadPhantomData() = 0;

  virtual void ConstructPhantom(G4LogicalVolume* parentLV);
  
  std::pair<G4double,G4double> ReadVoxelDim( G4int nVoxel, std::ifstream& fin ); 
  G4Material* BuildMaterialChangingDensity( G4Material* origMate, float density, G4String mateName );

  virtual void ReadVoxelDensities( std::ifstream& fin );

protected:
  G4LogicalVolume* world_logic;
  G4LogicalVolume* cont_logic;
  G4VPhysicalVolume* parameterisedPhysVolume; 

  G4int nVoxelX, nVoxelY, nVoxelZ;
  G4float dimX, dimY, dimZ;
  G4float offsetX, offsetY, offsetZ;

  std::vector<G4Material*> thePhantomMaterials; 
  G4Material* theMatePhantom;
  size_t* theMateIDs;
  float* theMateDensities;

  G4ThreeVector theInitialDisp;
  G4double theInitialRotAngleX;
  G4double theInitialRotAngleY;
  G4double theInitialRotAngleZ;

  G4PhantomParameterisation* thePhantomParam;

  G4bool bRecalculateMaterialDensities;

  std::map<G4int,G4Material*> thePhantomMaterialsOriginal; // map numberOfMaterial to G4Material. They are the list of materials as built from .geom file

};

#endif
