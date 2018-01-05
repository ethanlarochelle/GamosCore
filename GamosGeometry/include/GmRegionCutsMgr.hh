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
#ifndef GmRegionCutsMgr_h
#define GmRegionCutsMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmRegionCutsMgr    
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

//------------------------------------------------------------------------
class GmRegionCutsMgr 
{ 
 private:
  GmRegionCutsMgr();

 public:
  ~GmRegionCutsMgr();

  //! Get the only instance 
  static GmRegionCutsMgr* GetInstance();

  void AddRegionData( const std::vector<G4String>& rd );

  void AddRegionCuts( const std::vector<G4String>& rc );
  
  std::vector<GmRegionData*> FindRegionData( const G4String& name);

  void BuildRegions();
  void BuildProductionCuts();

  std::map<G4Region*,G4int> GetRegionIndices() const {
    return theRegionIndices;
  }

  void BuildRegionIndices();
  G4int GetRegionIndex( G4Region* reg );
  G4Region* GetIndexRegion( G4int index );

private:
  static GmRegionCutsMgr* theInstance;

  std::vector<GmRegionData*> theRegionDatae;

  std::map<G4Region*,G4int> theRegionIndices;

};

#endif
