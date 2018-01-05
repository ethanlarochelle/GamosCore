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
#ifndef GmStepMgr_h
#define GmStepMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmStepMgr    
Author:      P. Arce
Changes:     12/01: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Manager of copies of G4Step, for filtering in the future  */ 
//----------------------------------------------- 

#include <vector>
#include <map>
#include <set>
#include "globals.hh"
#include "G4TrackVector.hh"
class GmFutureFilter;
class G4Step;
class G4StepPoint;
class G4Track;
class G4TouchableHistory;
typedef std::multimap<G4int,G4Step*> mmis;

//------------------------------------------------------------------------
class GmStepMgr 
{ 
private:
  GmStepMgr();

public:
  static GmStepMgr* GetInstance();
  ~GmStepMgr(){};

  void SaveStep( const G4Step* aStep );
  G4Track* SaveTrack( G4Track* aTrack );
  std::vector<G4Step*> GetSteps( GmFutureFilter* filter, const G4Step* currentStep );
  G4Step* GetAncestorStep( const G4Step* currentStep );

  void ClearObjects();

private:
  static GmStepMgr* theInstance;

protected:
  std::multimap<G4int,G4Step*> theNewSteps; // store track ID for easier ancestor list building
  std::vector<G4StepPoint*> theNewStepPoints;
  std::map<G4int,G4Track*> theNewTracks;
  std::vector<G4TouchableHistory*> theNewTouchables;
  std::map<G4int,G4Step*> theCreatorStep;

  G4TrackVector theTracksSaved;
  G4int theNTracksSavedInEvent;
};

#endif
