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
#include "GmReadPhantomSVMgr.hh"
#include "GmReadDICOMVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include <fstream>

GmReadPhantomSVMgr* GmReadPhantomSVMgr::theInstance = 0;

//---------------------------------------------------------------------------
GmReadPhantomSVMgr* GmReadPhantomSVMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmReadPhantomSVMgr();
  }
  return theInstance;
}

//---------------------------------------------------------------------------
void GmReadPhantomSVMgr::ReadSVEGS( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ )
{
  //  std::vector<G4String> theSVNames;
  //  theSVNames = GmParameterMgr::GetInstance()->GetVStringValue("GmReadPhantomEGSwithStGeometry:SVNames",theSVNames);

  G4int theSVNOD = GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomEGSwithStGeometry:SVNumberOfDigits",2);

  theSVIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) {
    G4cout << " GmReadPhantomSVMgr::ReadSVEGS created theSVIDs of size " << nVoxelX*nVoxelY*nVoxelZ << G4endl;
  }
#endif

  std::vector<G4String> wl;
  G4String stemp;
  G4int id;
  std::ifstream* finf = fin.GetIfstream();
  for( G4int ii = 0; ii < nVoxelY*nVoxelZ; ii++ ){
    *finf >> stemp;
    for( size_t jj = 0; jj < stemp.length(); jj += theSVNOD ){
      id = GmGenUtils::GetInt(stemp.substr(jj,theSVNOD));
      G4int copyNo = jj/theSVNOD + ii*nVoxelX;
      //      G4cout << " GmReadPhantomSVMgr copyNo " << copyNo << " jj " << jj << " ii " << ii << G4endl;
#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) ) {
	if( id != -1 ) G4cout << "GmReadPhantomSVMgr::ReadSVEGS  ReadPhantomSV " << copyNo << " = " << id << G4endl;
    }
#endif
      theSVIDs[copyNo] = id;
    }
  }
  /*  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	fin >> id; 
	G4int nnew = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	//	G4cout << ix << " " << iy << " " << iz << " filling mateIDs " << nnew << " = " << atoi(cid)-1 << G4endl;
	theSVIDs[nnew] = id;
      }
    }
    } */

  G4int tvnumber;
  G4String tvname;
  for(;;){
    if( !fin.GetWordsInLine(wl) ) return;
    tvnumber = GmGenUtils::GetInteger(wl[0]);
    tvname = wl[1];
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) G4cout << "GmReadPhantomSVMgr::ReadSVEGS:  Reading tvnumber " << tvnumber << " tvname " << tvname << G4endl;
#endif
    if( fin.eof() ) break;
    theSVs[tvnumber] = tvname;
    /*    G4bool bfound = FALSE;
    for( size_t ii = 0; ii < theSVNames.size(); ii++ ){  
      if( tvname == theSVNames[ii] ){
	theSVNumbers.insert( tvnumber );
	bfound = TRUE;
      } 
    }
    if( !bfound ) {
      G4Exception("GmReadPhantomSVMgr::ReadSV",
		  "",
		  FatalErrorInArgument,
		  (" volume name not found " + tvname).c_str());
		  } */
  }

}

//---------------------------------------------------------------------------
void GmReadPhantomSVMgr::ReadSVG4( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ )
{
  //  std::vector<G4String> theSVNames;
  //  theSVNames = GmParameterMgr::GetInstance()->GetVStringValue("GmReadPhantomEGSwithStGeometry:SVNames",theSVNames);

  G4int nVoxels = nVoxelX*nVoxelY*nVoxelZ;
  theSVIDs = new size_t[nVoxels];
#ifndef GAMOS_NO_VERBOSE
 if( ReadDICOMVerb(debugVerb) ) {
   G4cout << " GmReadPhantomSVMgr::ReadSVG4 created theSVIDs of size " << nVoxelX*nVoxelY*nVoxelZ << G4endl;
 }
#endif
  G4String stemp;
  G4int id;
  std::vector<G4String> wl;
  std::ifstream* finf = fin.GetIfstream();
  for( G4int ii = 0; ii < nVoxels; ii++ ){
    *finf >> id;
    if( ii% 100000 == 1 ) G4cout<<"GmReadPhantomSVMgr::ReadSVG4  ReadPhantomSVn " << ii << " = " << id << G4endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) {
      if( id != 0 ) G4cout << this <<"GmReadPhantomSVMgr::ReadSVG4  ReadPhantomSV " << ii << " = " << id << G4endl;
}
#endif
    theSVIDs[ii] = id;
  }
  /*  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	fin >> id; 
	G4int nnew = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	//	G4cout << ix << " " << iy << " " << iz << " filling mateIDs " << nnew << " = " << atoi(cid)-1 << G4endl;
	theSVIDs[nnew] = id;
      }
    }
    } */

  G4int tvnumber;
  G4String tvname;
  if( fin.eof() ) {
      G4Exception("GmReadPhantomSVMgr::ReadSV",
		  "",
		  FatalErrorInArgument,
		  "End of file before list of structure names");
  }
  for(;;){
    if( !fin.GetWordsInLine(wl) ) return;
    tvnumber = GmGenUtils::GetInteger(wl[0]);
    tvname = wl[1];;
    if( fin.eof() ) break;
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) {
      G4cout << "GmReadPhantomSVMgr::ReadSVG4:  Reading tvnumber " << tvnumber << " tvname " << tvname << G4endl;
    }
#endif
    theSVs[tvnumber] = tvname;
    /*    G4bool bfound = FALSE;
    for( size_t ii = 0; ii < theSVNames.size(); ii++ ){  
      if( tvname == theSVNames[ii] ){
	theSVNumbers.insert( tvnumber );
	bfound = TRUE;
      } 
    }
    if( !bfound ) {
      G4Exception("GmReadPhantomSVMgr::ReadSV",
		  "",
		  FatalErrorInArgument,
		  (" volume name not found " + tvname).c_str());
		  } */
  }

}

//---------------------------------------------------------------------------
G4int GmReadPhantomSVMgr::GetSVID( size_t voxelID )
{
  /*  //  size_t vid = *(theSVIDs+voxelID*sizeof(size_t));
  size_t vid = theSVIDs[voxelID];
  std::map<G4int,G4String>::const_iterator ite = theSVs.find(vid);
  if( ite != theSV.end() ){
    return vid;
  }else {
    return -1;
  }
*/

  return theSVIDs[voxelID];

}

//---------------------------------------------------------------------------     
G4int GmReadPhantomSVMgr::GetSVIDFromPhysVolName( G4String pvName )
{
  std::map<G4int,G4String>::const_iterator ite;
  for( ite = theSVs.begin(); ite != theSVs.end(); ite++ ) {
    if( (*ite).second == pvName ) {
      return (*ite).first;      
    }
  }

  for( ite = theSVs.begin(); ite != theSVs.end(); ite++ ) {
    G4cerr << " GmReadPhantomSVMgr::GetSVIDFromPhysVolName SV " << (*ite).first << " : " << (*ite).second << G4endl;
  }
  G4Exception("GmReadPhantomSVMgr::GetSVIDFromPhysVolName",
	      "",
	      FatalException,
	      ("SV name not found " + pvName).c_str());

  return -1;
}
