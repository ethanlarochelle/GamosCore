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

#include "GmReadPhantomEGSGeometry.hh"
#include "GmReadDICOMVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4PhantomParameterisation.hh"
#include "G4GeometryTolerance.hh"
#include "G4tgbVolumeMgr.hh"
#include "G4tgbMaterialMgr.hh"
#include "G4tgrMessenger.hh"
#include "G4tgrMessenger.hh"

//---------------------------------------------------------------------------
GmReadPhantomEGSGeometry::GmReadPhantomEGSGeometry()
{
}

//---------------------------------------------------------------------------
GmReadPhantomEGSGeometry::~GmReadPhantomEGSGeometry()
{
}


//---------------------------------------------------------------------------
void GmReadPhantomEGSGeometry::ReadPhantomData()
{
  G4String filename = GmParameterMgr::GetInstance()->GetStringValue("GmReadPhantomGeometry:Phantom:FileName", "test.egsphant");

  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  filename = GmGenUtils::FileInPath( path, filename );

  GmFileIn fing = GmFileIn::GetInstance(filename);
  std::ifstream* fin = fing.GetIfstream();

  std::vector<G4String> wl;
  G4int nMaterials;
  if( !fing.GetWordsInLine(wl) ) return;
  nMaterials = GmGenUtils::GetInteger(wl[0]);
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(infoVerb) ) G4cout << "GmReadPhantomEGSGeometry::ReadPhantomData nMaterials " << nMaterials << G4endl;
#endif
  G4String stemp;
  G4tgbMaterialMgr* matmgr = G4tgbMaterialMgr::GetInstance(); 
  for( G4int ii = 0; ii < nMaterials; ii++ ){
    if( !fing.GetWordsInLine(wl) ) return;
    stemp = wl[0];
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(infoVerb) ) G4cout << "GmReadPhantomEGSGeometry::ReadPhantomData material " << stemp << G4endl;
#endif
    G4Material* mate = matmgr->FindOrBuildG4Material(stemp);
    thePhantomMaterialsOriginal[ii] = mate;
  }

  for( G4int ii = 0; ii < nMaterials; ii++ ) *fin >> stemp;  // not used 

  *fin >> nVoxelX >> nVoxelY >> nVoxelZ;
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(infoVerb) ) G4cout << "GmReadPhantomEGSGeometry::ReadPhantomData nVoxelX " << nVoxelX << " nVoxelY " << nVoxelY << " nVoxelZ " << nVoxelZ << G4endl;
#endif
  std::pair<G4double,G4double> posdim = ReadVoxelDim( nVoxelX, *fin );
  offsetX = posdim.first;
  dimX = posdim.second;
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(infoVerb) ) G4cout << "GmReadPhantomEGSGeometry::ReadPhantomData dimX " << dimX << " offsetX " << offsetX << G4endl;
#endif
  posdim = ReadVoxelDim( nVoxelY, *fin );
  offsetY = posdim.first;
  dimY = posdim.second;
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(infoVerb) ) G4cout << "GmReadPhantomEGSGeometry::ReadPhantomData dimY " << dimY << " offsetY " << offsetY << G4endl;
#endif
  posdim = ReadVoxelDim( nVoxelZ, *fin );
  offsetZ = posdim.first;
  dimZ = posdim.second;
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(infoVerb) ) G4cout << "GmReadPhantomEGSGeometry::ReadPhantomData dimZ " << dimZ << " offsetZ " << offsetZ << G4endl;
#endif

  //  mateIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];
  theMateIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];
  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) { // check if loop in X first???
      if( !fing.GetWordsInLine(wl) ) return;
      stemp = wl[0];
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
      //      G4cout << " stemp " << stemp << G4endl;
	G4int nnew = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	G4String cid = stemp.substr(ix,1).c_str();
		//if( nnew % 10000 == 0 ) G4cout << stemp << " " << ix << " " << iy << " " << iz << " filling mateIDs " << nnew << " = " << atoi(cid.c_str())-1 << G4endl;
		//if( nnew > nVoxelX*nVoxelY*nVoxelZ-10000 ) G4cout << ix << " " << iy << " " << iz << " filling mateIDs " << nnew << " = " << atoi(cid)-1 << G4endl;
	G4int mateID = atoi(cid)-1;
	if( mateID < 0 || mateID >= nMaterials ) {
	  G4Exception("GmReadPhantomEGSGeometry::ReadPhantomData",
		      "Wrong index in phantom file",
		      FatalException,
		      G4String("eIt should be between 0 and "
			       + GmGenUtils::itoa(nMaterials-1) 
			       + ", while it is " 
			       + GmGenUtils::itoa(mateID)).c_str());
	}
	theMateIDs[nnew] = mateID;
	//	mateIDs[nnew] = 1-1;
      }
    }
  }

  ReadVoxelDensities( *fin );

  ReadPS( fing );

  fin->close();

}


//---------------------------------------------------------------------------
std::pair<G4double,G4double> GmReadPhantomEGSGeometry::ReadVoxelDim( G4int nVoxel, std::ifstream& fin ) 
{

  G4double dtemp = 0.;
  G4double dtempread = 0.;
  G4double dtempold = 0;
  G4double dim = 0.;
  G4double offset = 0.;
  G4double posLast;
  for( G4int ii = 0; ii < nVoxel+1; ii++ ){
    fin >> dtempread;
    dtempread *=CLHEP::cm;
    //    G4cout << " ReadVoxelDim " << ii << " = " << dtempread << G4endl;
    if( ii == 0 ){
      offset = dtempread;
    } else {
      dtemp = dtempread - dtempold;
      if( ii != 1 && fabs(dtemp - dim) > 1.E-4*CLHEP::mm ){ 
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(infoVerb) ) G4cout <<"GmReadPhantomEGSGeometry::ReadVoxelDim: voxel " << ii << " dimension " << dtemp << " old dim " << dim << " dtep-dim " << dtemp-dim << " dtempold " << dtempold << G4endl;
#endif
	G4Exception("GmReadPhantomEGSGeometry::ReadVoxelDim",
		    "Wrong argument",
		    FatalErrorInArgument, 
		    "Dimension is not the same as previous voxel. This is not supported");
      }
      dim = dtemp;
      //      G4cout << ii << " dim " << dim << G4endl;
    }
    dtempold = dtempread;
  }
  
  posLast = dtempread;
  dim = (posLast-offset)/nVoxel;
  //  G4cout << " return dim " << std::setprecision(12) << dim << G4endl;
  
  return std::pair<G4double,G4double>(offset,dim);
  
}
