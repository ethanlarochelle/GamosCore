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

#ifndef GmReadPETDicom_h
#define GmReadPETDicom_h 1

#include "globals.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

class G4Material;
class G4LogicalVolume;
class G4PhysicalVolume;
class G4Box;


class GmReadPETDicom 
{
public:

  GmReadPETDicom();
  ~GmReadPETDicom();

public:
  virtual void ReadData( std::string fileName = "" );
  
  size_t* GetActivities() const {
    return theActivities;
  }
  size_t GetPixel(int ii) const {
    return theActivities[ii];
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
  
private:
  G4int nVoxelX, nVoxelY, nVoxelZ;
  G4float dimX, dimY, dimZ;
  G4float offsetX, offsetY, offsetZ;

  size_t* theActivities;
  
  //-  std::pair<G4double,G4double> ReadVoxelDim( G4int nVoxel, std::ifstream& fin ); 
};

#endif
