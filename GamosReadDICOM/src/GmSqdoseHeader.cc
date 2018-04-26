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
#include "GmSqdoseHeader.hh"
#include "Gm3ddoseHeader.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-----------------------------------------------------------------------
G4bool GmSqdoseHeader::operator==(const GmSqdoseHeader& right) const
{
  G4bool equ = 1;
  if( theNoVoxelX != right.GetNoVoxelX() ||
    theNoVoxelY != right.GetNoVoxelY() ||
    theNoVoxelZ != right.GetNoVoxelZ() ||
    theVoxelLimitsX != right.GetVoxelLimitsX() ||
    theVoxelLimitsY != right.GetVoxelLimitsY() ||
    theVoxelLimitsZ != right.GetVoxelLimitsZ() ) {
    equ = 0;
  }
  return equ;

}

//-----------------------------------------------------------------------
G4bool GmSqdoseHeader::operator!=(const GmSqdoseHeader& right) const
{
  return !( *this == right );
}

//-----------------------------------------------------------------------
GmSqdoseHeader::GmSqdoseHeader(const Gm3ddoseHeader& dh3d)
{
  //  G4cout << " reading header file " << G4endl;
  theNoEvent = dh3d.GetNumberOfEvents();
  G4cout << " N events " << theNoEvent << G4endl;

  theNoVoxelX = dh3d.GetNoVoxelX();
  theNoVoxelY = dh3d.GetNoVoxelY();
  theNoVoxelZ = dh3d.GetNoVoxelZ();

  theVoxelLimitsX = dh3d.GetVoxelLimitsX();
  theVoxelLimitsY = dh3d.GetVoxelLimitsY();
  theVoxelLimitsZ = dh3d.GetVoxelLimitsZ();
  for( size_t ii = 0; ii < theNoVoxelX+1; ii++ ){
    theVoxelLimitsX[ii] *= CLHEP::cm;
  }
  for( size_t ii = 0; ii < theNoVoxelY+1; ii++ ){
    theVoxelLimitsY[ii] *= CLHEP::cm;
  }
  for( size_t ii = 0; ii < theNoVoxelZ+1; ii++ ){
    theVoxelLimitsZ[ii] *= CLHEP::cm;
  } 
  G4cout << "GmSqdoseHeader(3ddose)  VoxelLimits " <<theVoxelLimitsX[1]-theVoxelLimitsX[0] << " " << theVoxelLimitsY[1]-theVoxelLimitsY[0] << " " << theVoxelLimitsZ[1]-theVoxelLimitsZ[0] << G4endl;
  G4cout << "GmSqdoseHeader(3ddose)  VoxelLimits " << theVoxelLimitsZ[0] << " " << theVoxelLimitsZ[1] << " " << theVoxelLimitsZ[2] << G4endl;

  // check first Z voxel
  if( theVoxelLimitsZ[1]-theVoxelLimitsZ[0] != theVoxelLimitsZ[2]-theVoxelLimitsZ[1] ) {
    G4cerr << "!! GmSqdoseHeader(const Gm3ddoseHeader) First Z voxel is different " << theVoxelLimitsZ[0] << " - " << theVoxelLimitsZ[1] << " " << theVoxelLimitsZ[2] << G4endl;
    theVoxelLimitsZ[0] = theVoxelLimitsZ[1] - (theVoxelLimitsZ[2]-theVoxelLimitsZ[1]);
    G4cout << "GmSqdoseHeader(3ddose) NEW VoxelLimits " << theVoxelLimitsZ[0] << " " << theVoxelLimitsZ[1] << " " << theVoxelLimitsZ[2] << G4endl;
  }

}
//-----------------------------------------------------------------------
void GmSqdoseHeader::Read( FILE* fin )
{
  if( fread(&theNoEvent, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading number of events");
  }

  //G4cout << " GmSqdoseHeader::Read NEvent " << theNoEvent << G4endl;

  if( fread(&theNoVoxelX, sizeof(size_t),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading number of voxels X");
  }
  if( fread(&theNoVoxelY, sizeof(size_t),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading number of voxels Y");
  }
  if( fread(&theNoVoxelZ, sizeof(size_t),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading number of voxels Z");
  }
  G4cout << "GmSqdoseHeader::Read NVoxels " << theNoVoxelX << " " << theNoVoxelY << " " << theNoVoxelZ << G4endl;
  
  float vlim;
  for( size_t ii = 0; ii < theNoVoxelX+1; ii++ ){
    if( fread(&vlim, sizeof(float),  1, fin) != 1) {
      G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		  "Problem reading number of voxel limits X");
    }
    theVoxelLimitsX.push_back( vlim );
    //    G4cout << ii << " READ theVoxelLimitsX " << vlim << G4endl; //GDEB
  }
  for( size_t ii = 0; ii < theNoVoxelY+1; ii++ ){
    if( fread(&vlim, sizeof(float),  1, fin) != 1) {
      G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		  "Problem reading number of voxel limits Y");
    }
    theVoxelLimitsY.push_back( vlim );
    //    G4cout << ii << " READ theVoxelLimitsY " << vlim << G4endl; //GDEB
  }
  for( size_t ii = 0; ii < theNoVoxelZ+1; ii++ ) {
    if( fread(&vlim, sizeof(float),  1, fin) != 1) {
      G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		  "Problem reading number of voxel limits Z");
    }
    theVoxelLimitsZ.push_back( vlim );
    //    G4cout << ii << " READ theVoxelLimitsZ " << vlim << G4endl; //GDEB
  }

  G4cout << "GmSqdoseHeader::Read VoxelLimits " <<theVoxelLimitsX[1]-theVoxelLimitsX[0] << " " << theVoxelLimitsY[1]-theVoxelLimitsY[0] << " " << theVoxelLimitsZ[1]-theVoxelLimitsZ[0] << G4endl;

  float rotxx, rotxy, rotxz, rotyx, rotyy, rotyz, rotzx, rotzy, rotzz;
  if( fread(&rotxx, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Rea",
		"Error",
		FatalException,
		"Problem reading rotation matrix xx");
  }
  if( fread(&rotxy, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix xy");
  }
  if( fread(&rotxz, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix xz");
  }
  if( fread(&rotyx, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix yx");
  }
  if( fread(&rotyy, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix yy");
  }
  if( fread(&rotyz, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix yz");
  }
  if( fread(&rotzx, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix zx");
  }
  if( fread(&rotzy, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix zy");
  }
  if( fread(&rotzz, sizeof(float),  1, fin) != 1) {
    G4Exception("GmSqdoseHeader::Read",
		"Error",
		FatalException,
		"Problem reading rotation matrix zz");
  }

  CLHEP::HepRep3x3 rottemp(rotxx,rotyx,rotzx,  // matrix representation
                           rotxy,rotyy,rotzy,  // (inverted)
                           rotxz,rotyz,rotzz);
  
  theRotationMatrix = G4RotationMatrix( rottemp );
  //  G4cout << " GmSqdoseHeader::Read N VoxelsLimits " << theVoxelLimitsX.size() << " " << theVoxelLimitsY.size() << " " << theVoxelLimitsZ.size() << G4endl;


}



//-----------------------------------------------------------------------
void GmSqdoseHeader::Print( FILE* fout )
{
  /*  if(fwrite((int *)theNoEvents,
                sizeof(int),1,fout)!=1)
        printf("\n Error writing temp2. \n");
  */

  if(fwrite(&theNoEvent,sizeof(float),1,fout)!=1)
    printf("\n Error writing nEvent. \n");

  //  G4cout << " GmSqdoseHeader::Print theNoEvent " << theNoEvent << G4endl;
  if(fwrite(&theNoVoxelX,
	    sizeof(size_t),1,fout)!=1)
    printf("\n Error writing nVoxelX. \n");
  
  if(fwrite(&theNoVoxelY,
	    sizeof(size_t),1,fout)!=1)
    printf("\n Error writing nVoxelY. \n");
  
  if(fwrite(&theNoVoxelZ,
	    sizeof(size_t),1,fout)!=1)
    printf("\n Error writing nVoxelZ. \n");

  G4cout << " HEADER NEvent " << theNoEvent << " Nvoxels " << theNoVoxelX << " " << theNoVoxelY<< " " << theNoVoxelZ << G4endl;
  float* vlimx = new float[theNoVoxelX+1];
  for( size_t ii = 0; ii < theNoVoxelX+1; ii++ ){
    vlimx[ii] = theVoxelLimitsX[ii];
    // G4cout << ii << " limx " << vlimx[ii] << " = " << theVoxelLimitsX[ii] << G4endl; //GDEB
  }
  if(fwrite(vlimx, sizeof(float),theNoVoxelX+1,fout)!=theNoVoxelX+1)
    printf("\n Error writing VoxelLimitsX. \n");
  
  float* vlimy = new float[theNoVoxelY+1];
  for( size_t ii = 0; ii < theNoVoxelY+1; ii++ ){
    vlimy[ii] = theVoxelLimitsY[ii];
    //       G4cout << ii << " limy " << vlimy[ii] << " = " << theVoxelLimitsY[ii] << G4endl; //GDEB
  }
  if(fwrite(vlimy, sizeof(float),theNoVoxelY+1,fout)!=theNoVoxelY+1)
    printf("\n Error writing VoxelLimitsY. \n");
  
  float* vlimz = new float[theNoVoxelZ+1];
  for( size_t ii = 0; ii < theNoVoxelZ+1; ii++ ){
    vlimz[ii] = theVoxelLimitsZ[ii];
    //    G4cout << ii << " limz " << vlimz[ii] << " = " << theVoxelLimitsZ[ii] << G4endl; //GDEB
  }
  if(fwrite(vlimz, sizeof(float),theNoVoxelZ+1,fout)!=theNoVoxelZ+1)
    printf("\n Error writing VoxelLimitsZ. \n");

  float rotxx = theRotationMatrix.xx();
  if(fwrite(&rotxx, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotxx. \n");
  float rotxy = theRotationMatrix.xy();
  if(fwrite(&rotxy, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotxy. \n");
  float rotxz = theRotationMatrix.xz();
  if(fwrite(&rotxz, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotxz. \n");
  float rotyx = theRotationMatrix.yx();
  if(fwrite(&rotyx, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotyx. \n");
  float rotyy = theRotationMatrix.yy();
  if(fwrite(&rotyy, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotyy. \n");
  float rotyz = theRotationMatrix.yz();
  if(fwrite(&rotyz, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotyz. \n");
  float rotzx = theRotationMatrix.zx();
  if(fwrite(&rotzx, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotzx. \n");
  float rotzy = theRotationMatrix.zy();
  if(fwrite(&rotzy, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotzy. \n");
  float rotzz = theRotationMatrix.zz();
  if(fwrite(&rotzz, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotzz. \n");

  delete vlimx;
  delete vlimy;
  delete vlimz;
}
