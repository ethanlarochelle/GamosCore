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
#ifndef GmCutsStudyMgr_h
#define GmCutsStudyMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmCutsStudyMgr    
Author:      P. Arce
Changes:     05/08: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Manager for cuts studies  */ 

#include "globals.hh"
class G4Region;
class GmCSTrackInfo;
class GmCSTrackStepInfo;
class G4Step;
#include "G4VFilter.hh"
#include "G4ThreeVector.hh"

#include <map>
#include <vector>

typedef std::multimap<G4Region*,GmCSTrackInfo*> mmrcs;
typedef G4VFilter<G4Step> GmVCSFilter;

//------------------------------------------------------------------------
class GmCutsStudyMgr 
{ 
private:
  GmCutsStudyMgr();
 public:
  ~GmCutsStudyMgr();
  static GmCutsStudyMgr* GetInstance();
  void AddTrackInfo( GmCSTrackInfo* );
  mmrcs GetTrackInfos() const { return theTrackInfos; }
  GmCSTrackInfo* GetTrackInfo( G4int id ); 
  GmCSTrackStepInfo* GetTrackStepInfo( G4int id, G4ThreeVector pos );
  std::vector<GmCSTrackStepInfo*> GetTrackStepInfosBefore( G4int id, G4double trklen );
  GmVCSFilter* GetFilter() const { 
    return theFilter; }
  void SetFilter( GmVCSFilter* filt ) {
    theFilter = filt; }

  void CleanTrackInfos();

private:
  static GmCutsStudyMgr* theInstance;
  std::multimap<G4Region*,GmCSTrackInfo*> theTrackInfos;
  GmVCSFilter* theFilter;
};

#endif
