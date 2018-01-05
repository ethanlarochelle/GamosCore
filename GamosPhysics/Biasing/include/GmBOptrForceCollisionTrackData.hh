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
// --------------------------------------------------------------------
// GEANT 4 class header file 
//
// Class Description:
//    Extends G4Track properties with information needed for the
//    force collision biasing operator.
//    The G4BOptrForceCollision class is made friend of this one in
//    order to keep unexposed to public most of the data members, as
//    they are used to control the logic.
//
// ------------------ GmBOptrForceCollisionTrackData ------------------
//
// Author: M.Verderi (LLR), October 2015
//
// --------------------------------------------------------------------

#ifndef GmBOptrForceCollisionTrackData_hh
#define GmBOptrForceCollisionTrackData_hh

class GmBOptrForceCollision;
#include "G4VAuxiliaryTrackInformation.hh"

enum ForceCollisionState { FCS_free, FCS_toBeCloned, FCS_toBeForced, FCS_toBeFreeFlight };

class GmBOptrForceCollisionTrackData : public G4VAuxiliaryTrackInformation {

friend class GmBOptrForceCollision;
  
public:
  GmBOptrForceCollisionTrackData( const GmBOptrForceCollision* );
  ~GmBOptrForceCollisionTrackData();
  
  // -- from base class:
  void Print() const;

  // -- Get methods:
  G4bool                             IsFreeFromBiasing() const
  { return ( fForceCollisionState == FCS_free);}
  // -- no set methods are provided : sets are made under exclusive control of GmBOptrForceCollision objects through friendness.
  
private:
  const GmBOptrForceCollision* fForceCollisionOperator;
  ForceCollisionState             fForceCollisionState;

  void Reset()
  {
    fForceCollisionOperator = nullptr;
    fForceCollisionState    = FCS_free;
  }
  
};

#endif
