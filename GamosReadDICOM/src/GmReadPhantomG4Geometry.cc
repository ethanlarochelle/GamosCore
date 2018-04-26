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

#include "GmReadPhantomG4Geometry.hh"
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
GmReadPhantomG4Geometry::GmReadPhantomG4Geometry()
{
}

//---------------------------------------------------------------------------
GmReadPhantomG4Geometry::~GmReadPhantomG4Geometry()
{
}

//---------------------------------------------------------------------------
void GmReadPhantomG4Geometry::ReadPhantomData()
{
  G4String filename = GmParameterMgr::GetInstance()->GetStringValue("GmReadPhantomGeometry:Phantom:FileName", "test.g4dcm");

  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  filename = GmGenUtils::FileInPath( path, filename );

  GmFileIn fing = GmFileIn::GetInstance(filename);
  std::ifstream* fin = fing.GetIfstream();
  std::vector<G4String> wl;
  G4int nMaterials;
  if( !fing.GetWordsInLine(wl) ) return;
  nMaterials = GmGenUtils::GetInteger(wl[0]);
  G4String stemp;
  G4tgbMaterialMgr* matmgr = G4tgbMaterialMgr::GetInstance(); 
  G4int nmate;
  for( G4int ii = 0; ii < nMaterials; ii++ ){
    if( !fing.GetWordsInLine(wl) ) return;
    nmate = GmGenUtils::GetInteger(wl[0]);
    stemp = wl[1];
    G4cout << "GmReadPhantomG4Geometry::ReadPhantomData reading nmate " << ii << " = " << nmate << " mate " << stemp << G4endl;
    if( ii != nmate ) G4Exception("GmReadPhantomG4Geometry::ReadPhantomData",
		"Wrong argument",
		FatalErrorInArgument,
				  "Material number should be in increasing order: wrong material number ");
    G4Material* mate = matmgr->FindOrBuildG4Material(stemp);
    thePhantomMaterialsOriginal[ii] = mate;
  }

  *fin >> thePatientPosition;
  if( GmGenUtils::IsNumber( thePatientPosition ) ) {
    nVoxelX = G4int(GmGenUtils::GetValue(thePatientPosition));
    *fin >> nVoxelY >> nVoxelZ;
  } else {
    *fin >> nVoxelX >> nVoxelY >> nVoxelZ;
  }
  G4cout << "GmReadPhantomG4Geometry::ReadPhantomData nVoxel X/Y/Z " << nVoxelX << " " << nVoxelY << " " << nVoxelZ << G4endl;
  *fin >> offsetX >> dimX;
  dimX = (dimX - offsetX)/nVoxelX;
  *fin >> offsetY >> dimY;
  dimY = (dimY - offsetY)/nVoxelY;
  *fin >> offsetZ >> dimZ;
  dimZ = (dimZ - offsetZ)/nVoxelZ;
  G4cout << "GmReadPhantomG4Geometry::ReadPhantomData voxelDimX " << dimX << " offsetX " << offsetX << G4endl;
  G4cout << "GmReadPhantomG4Geometry::ReadPhantomData voxelDimY " << dimY << " offsetY " << offsetY << G4endl;
  G4cout << "GmReadPhantomG4Geometry::ReadPhantomData voxelDimZ " << dimZ << " offsetZ " << offsetZ << G4endl;

  theMateIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];
  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	*fin >> stemp; 
	G4int nnew = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	//	G4cout << ix << " " << iy << " " << iz << " filling theMateIDs " << nnew << " = " <<  atoi(stemp.c_str())-1 << " " << stemp << G4endl;
	G4int mateID = atoi(stemp.c_str());
	if( mateID < 0 || mateID >= nMaterials ) {
	  G4Exception("GmReadPhantomG4Geometry::ReadPhantomData",
		      "Wrong index in phantom file",
		      FatalException,
		      G4String("It should be between 0 and "
			       + GmGenUtils::itoa(nMaterials-1) 
			       + ", while it is " 
			       + GmGenUtils::itoa(mateID)).c_str());
	}
	theMateIDs[nnew] = mateID;
      }
    }
  }

  ReadVoxelDensities( *fin );

  ReadPV( fing );

  fin->close();
}
