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
#include "globals.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "GmReadPETDicom.hh"
#include "G4PhantomParameterisation.hh"
#include "G4GeometryTolerance.hh"

#include "G4tgbVolumeMgr.hh"
#include "G4tgbMaterialMgr.hh"
#include "G4tgrMessenger.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4tgrMessenger.hh"

//---------------------------------------------------------------------------
GmReadPETDicom::GmReadPETDicom()
{
}

//---------------------------------------------------------------------------
GmReadPETDicom::~GmReadPETDicom()
{
}

//---------------------------------------------------------------------------
void GmReadPETDicom::ReadData( std::string fileName )
{

  if (fileName == "" ) {
    fileName = GmParameterMgr::GetInstance()->GetStringValue("GmReadPETDicom:FileName", "test.g4dcm");

    G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
    fileName = GmGenUtils::FileInPath( path, fileName );
  }
  
  GmFileIn fing = GmFileIn::GetInstance(fileName);
  std::ifstream* fin = fing.GetIfstream();
  std::vector<G4String> wl;

  *fin >> nVoxelX >> nVoxelY >> nVoxelZ;
  G4cout << "GmReadPETDicom::ReadData nVoxel X/Y/Z " << nVoxelX << " " << nVoxelY << " " << nVoxelZ << G4endl;
  *fin >> offsetX >> dimX;
  dimX = (dimX - offsetX)/nVoxelX;
  *fin >> offsetY >> dimY;
  dimY = (dimY - offsetY)/nVoxelY;
  *fin >> offsetZ >> dimZ;
  dimZ = (dimZ - offsetZ)/nVoxelZ;
  G4cout << "GmReadPETDicom::ReadData voxelDimX " << dimX << " offsetX " << offsetX << G4endl;
  G4cout << "GmReadPETDicom::ReadData voxelDimY " << dimY << " offsetY " << offsetY << G4endl;
  G4cout << "GmReadPETDicom::ReadData voxelDimZ " << dimZ << " offsetZ " << offsetZ << G4endl;

  G4String stemp;
  theActivities = new size_t[nVoxelX*nVoxelY*nVoxelZ];
  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	*fin >> stemp; 
	G4int nnew = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	G4int pixID = atoi(stemp.c_str());
	theActivities[nnew] = pixID;
	//	if( nnew < 1000 ) G4cout << this << " " <<  ix << " " << iy << " " << iz << " filling activities " << nnew << " : " << theActivities[nnew] << G4endl; //GDEB
      }
    }
  }

  fin->close();
}
