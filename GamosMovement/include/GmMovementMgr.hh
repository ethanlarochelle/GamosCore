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
#ifndef GmMovementMgr_h
#define GmMovementMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmMovementMgr    
Author:      P. Arce
Changes:      
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Class to manage the G4UserAction's and allow load on demand of any UserAction  */ 

#include <vector>
#include "globals.hh"
class GmVMovement;

//------------------------------------------------------------------------
class GmMovementMgr 
{ 
 private:
  GmMovementMgr();

 public:
  ~GmMovementMgr();

  //! Get the only instance 
  #ifdef WIN32
  #if defined GmMove_ALLOC_EXPORT 
    G4DLLEXPORT static GmMovementMgr* GetInstance();
  #else 
    G4DLLIMPORT static GmMovementMgr* GetInstance();
  #endif
#else
   static GmMovementMgr* GetInstance();
#endif

  std::vector<GmVMovement*> GetNewMovements( G4int eventID, G4double time );

  void AddMovement( GmVMovement* mv ) { 
    theMovements.push_back( mv ); }

  void InitialiseLastValue();

  void ReadMovementFile( G4String fileName );

  std::vector<GmVMovement*> GetMovements() const {
    return theMovements;
  }

  G4bool ApplyMovements() const { 
    return bApplyMovements; 
  }

private:
  static GmMovementMgr* theInstance;

  std::vector<GmVMovement*> theMovements;

  G4bool bApplyMovements; 
};

#endif
