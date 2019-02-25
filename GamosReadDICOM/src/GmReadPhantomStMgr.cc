#include "GmReadPhantomStMgr.hh"
#include "GmReadDICOMVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include <fstream>

GmReadPhantomStMgr* GmReadPhantomStMgr::theInstance = 0;

GmReadPhantomStMgr::GmReadPhantomStMgr()
{
  theROIShift = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmPhantomStructure:ROIShift",100));
}

//---------------------------------------------------------------------------
GmReadPhantomStMgr* GmReadPhantomStMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmReadPhantomStMgr();
  }
  return theInstance;
}

//---------------------------------------------------------------------------
void GmReadPhantomStMgr::ReadStructEGS( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ )
{
  G4int theStNOD = GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomEGSwithStGeometry:StNumberOfDigits",2);

  theStIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) {
    G4cout << " GmReadPhantomStMgr::ReadStructEGS created theStIDs of size " << nVoxelX*nVoxelY*nVoxelZ << G4endl;
  }
#endif

  std::vector<G4String> wl;
  G4String stemp;
  size_t id;
  std::ifstream* finf = fin.GetIfstream();
  for( G4int ii = 0; ii < nVoxelY*nVoxelZ; ii++ ){
    *finf >> stemp;
    for( size_t jj = 0; jj < stemp.length(); jj += theStNOD ){
      id = GmGenUtils::GetInt(stemp.substr(jj,theStNOD));
      size_t copyNo = jj/theStNOD + ii*nVoxelX;
      //      G4cout << " GmReadPhantomStMgr copyNo " << copyNo << " jj " << jj << " ii " << ii << G4endl;
#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) ) {
	if( id != -1 ) G4cout << "GmReadPhantomStMgr::ReadStructEGS  ReadPhantomSt " << copyNo << " = " << id << " <- " << G4endl;
    }
#endif
      theStIDs[copyNo] = id;
    }
  }

  G4int tvnumber;
  G4String tvname;
  for(;;){
    if( !fin.GetWordsInLine(wl) ) return;
    tvnumber = GmGenUtils::GetInteger(wl[0]);
    tvname = wl[1];
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) G4cout << "GmReadPhantomStMgr::ReadStructEGS:  Reading tvnumber " << tvnumber << " tvname " << tvname << G4endl;
#endif
    if( fin.eof() ) break;
    theStructs[tvnumber] = tvname;
  }

}

//---------------------------------------------------------------------------
void GmReadPhantomStMgr::ReadStructG4( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ )
{
  size_t nVoxels = nVoxelX*nVoxelY*nVoxelZ;
  theStIDs = new size_t[nVoxels];
#ifndef GAMOS_NO_VERBOSE
 if( ReadDICOMVerb(debugVerb) ) {
   G4cout << " GmReadPhantomStMgr::ReadStructG4 created theStIDs of size " << nVoxels << G4endl;
 }
#endif
  G4String stemp;
  size_t id;
  std::vector<G4String> wl;
  std::ifstream* finf = fin.GetIfstream();
  for( size_t ii = 0; ii < nVoxels; ii++ ){
    *finf >> id;
#ifndef GAMOS_NO_VERBOSE
 if( ReadDICOMVerb(infoVerb) ) {
   if( ii% 1000000 == 1 ) G4cout<<"GmReadPhantomStMgr::ReadStructG4  ReadPhantomStn " << ii << " = " << id << G4endl; //GDEB
 }
 if( ReadDICOMVerb(debugVerb) ) {
   if( id != 0 ) G4cout <<"GmReadPhantomStMgr::ReadStructG4  ReadPhantomSt " << ii << " = " << id << G4endl;
 }
#endif
    theStIDs[ii] = id;
  }
  /*  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	fin >> id; 
	G4int nnew = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	//	G4cout << ix << " " << iy << " " << iz << " filling mateIDs " << nnew << " = " << atoi(cid)-1 << G4endl;
	theStIDs[nnew] = id;
      }
    }
    } */

  G4int tvnumber;
  G4String tvname;
  if( fin.eof() ) {
      G4Exception("GmReadPhantomStMgr::ReadStructG4",
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
      G4cout << "GmReadPhantomStMgr::ReadStructG4:  Reading tvnumber " << tvnumber << " tvname " << tvname << G4endl;
    }
#endif
    theStructs[tvnumber] = tvname;
  }

}

//---------------------------------------------------------------------------
size_t GmReadPhantomStMgr::GetStID( size_t voxelID )
{
  return theStIDs[voxelID];

}

//---------------------------------------------------------------------------
std::vector<size_t> GmReadPhantomStMgr::GetStIDList( size_t voxelID )
{
  std::vector<size_t> stIDList;
  G4double lognm = 1./log(theROIShift);
  size_t stid = theStIDs[voxelID];
  G4int nSTs;
  if( stid == 0 ) {
    nSTs = 1;
  } else {
    nSTs = log(stid)*lognm + 1;
  }
  //    if( stid != 0 ) G4cout << " PVSID " << ii << " : " << stid << " " << nSTs << " : " << log(stid) << " " << lognm << G4endl; //GDEB
  for( G4int ipv = 0; ipv < nSTs; ipv++ ) {
    size_t stid1 = size_t( stid / std::pow(theROIShift,ipv) ) % theROIShift;
    //      if( stid != 0 )  G4cout << " PVSID1 " << stid1 << " = " << theStructureID << G4endl; //GDEB
    stIDList.push_back(stid1);
  }

  return stIDList;

}

//---------------------------------------------------------------------------     
G4int GmReadPhantomStMgr::GetStIDFromPhysVolName( G4String pvName )
{
  std::map<G4int,G4String>::const_iterator ite;
  for( ite = theStructs.begin(); ite != theStructs.end(); ite++ ) {
    if( (*ite).second == pvName ) {
      return (*ite).first;      
    }
  }

  for( ite = theStructs.begin(); ite != theStructs.end(); ite++ ) {
    G4cerr << " GmReadPhantomStMgr::GetStIDFromPhysVolName Struct " << (*ite).first << " : " << (*ite).second << G4endl;
  }
  G4Exception("GmReadPhantomStMgr::GetStIDFromPhysVolName",
	      "",
	      FatalException,
	      ("Struct name not found " + pvName).c_str());

  return -1;
}
