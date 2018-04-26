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
#ifndef GmReadPhantomSVMgr_hh
#define GmReadPhantomSVMgr_hh

#include <map>
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

class GmReadPhantomSVMgr 
{
  
  GmReadPhantomSVMgr(){};
  
public:
  ~GmReadPhantomSVMgr(){};
  
  static GmReadPhantomSVMgr* GetInstance();
  
  void ReadSVG4( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ );
  void ReadSVEGS( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ );

  G4int GetSVID( size_t id );
  size_t* GetSVIDs() const {
   return theSVIDs;
  }
  std::map<G4int,G4String> GetSVs() const {
    return theSVs;
  }

  G4int GetSVIDFromPhysVolName( G4String pvName );

private:
  static GmReadPhantomSVMgr* theInstance;

  size_t* theSVIDs;
  std::map<G4int,G4String> theSVs; // the structure voxels

};

#endif
