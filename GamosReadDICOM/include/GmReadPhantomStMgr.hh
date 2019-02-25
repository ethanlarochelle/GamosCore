#ifndef GmReadPhantomStMgr_hh
#define GmReadPhantomStMgr_hh

#include <map>
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

class GmReadPhantomStMgr 
{
  
  GmReadPhantomStMgr();
  
public:
  ~GmReadPhantomStMgr(){};
  
  static GmReadPhantomStMgr* GetInstance();
  
  void ReadStructG4( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ );
  void ReadStructEGS( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ );

  size_t GetStID( size_t id );
  std::vector<size_t> GetStIDList( size_t id ); // decompose id in Structure ID's 
  size_t* GetStIDs() const {
   return theStIDs;
  }
  std::map<G4int,G4String> GetStructs() const {
    return theStructs;
  }

  G4int GetStIDFromPhysVolName( G4String pvName );

  size_t GetROIShift() const {
    return theROIShift;
  }
  void SetROIShift( size_t mr ) {
    theROIShift = mr;
  }

private:
  static GmReadPhantomStMgr* theInstance;

  size_t* theStIDs;
  std::map<G4int,G4String> theStructs; // the structure voxels
  size_t theROIShift;
};

#endif
