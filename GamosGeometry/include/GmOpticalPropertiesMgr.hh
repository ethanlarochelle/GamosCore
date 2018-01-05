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
#ifndef GmOpticalPropertiesMgr_h
#define GmOpticalPropertiesMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmOpticalPropertiesMgr    
Author:      P. Arce
Changes:      
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 

#include <vector>
#include <map>
#include "globals.hh"
class GmRegionData;
class G4Region;
class G4MaterialPropertiesTable;
class G4OpticalSurface;
class G4LogicalBorderSurface;
class G4LogicalSkinSurface;
class G4SurfaceProperty;

struct LogicalBorderSurfaceData {
  G4String PVname1;
  G4String PVname2;
  G4SurfaceProperty* surfaceProp;
};
struct LogicalSkinSurfaceData {
  G4String LVname;
  G4SurfaceProperty* surfaceProp;
};

typedef std::map<G4String, G4MaterialPropertiesTable*> msmpt;
typedef std::map< G4String, std::map<G4String,std::vector<G4double> >* > msmsvd;
typedef std::map< G4String, std::map<G4String,G4double>* > msmsd;
typedef std::map<G4String, G4OpticalSurface*> msos;
typedef std::map<G4String, LogicalBorderSurfaceData> mslbsd;
typedef std::map<G4String, LogicalSkinSurfaceData> mslssd;

//------------------------------------------------------------------------
class GmOpticalPropertiesMgr 
{ 
 private:
  GmOpticalPropertiesMgr();

 public:
  ~GmOpticalPropertiesMgr();

  //! Get the only instance 
  static GmOpticalPropertiesMgr* GetInstance();

  void CreateMaterialPropertiesTable( const std::vector<G4String> wl );
  void AddEnergiesToTable( std::vector<G4String> wl );
  void AddPropertyToTable( const std::vector<G4String> wl );
  void AddConstPropertyToTable( const std::vector<G4String> wl );
  void AttachTableToMaterial( const std::vector<G4String> wl );
  void AttachTableToOpsurface( const std::vector<G4String> wl );  
  void CreateOpticalSurface( const std::vector<G4String> wl );
  void CreateLogicalBorderSurface( const std::vector<G4String> wl );
  void CreateLogicalSkinSurface( const std::vector<G4String> wl );

  void BuildG4();
  
private:
  static GmOpticalPropertiesMgr* theInstance;

  msmpt theMatPropTables;
  msmsvd theMPTProperties;
  msmsd theMPTConstProperties;
  std::map<G4String, G4String> theTableToMaterials;
  msos theOpticalSurfaces;
  mslbsd theLogicalBorderSurfaceData;
  mslssd theLogicalSkinSurfaceData;
};

#endif
