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
#ifndef GmVGeomCondition_h
#define GmVGeomCondition_h 1

#include "globals.hh"
class G4VTouchable;
class G4Track;
class G4Step;
class G4StepPoint;

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Base class to manage geometry conditions for GmVolumeConditionFilter
// 
///////////////////////////////////////////////////////////////////////////////

class GmVGeomCondition 
{
public: // with description
  GmVGeomCondition(){};
  
  virtual ~GmVGeomCondition(){};
  
public: // with description
  virtual const G4VTouchable* GetTouchableFromTrack(const G4Track*) = 0;
  virtual const G4VTouchable* GetTouchableFromStep(const G4Step*) = 0;
  const G4VTouchable* GetTouchableFromStepPoint(const G4StepPoint*);
  
};

#endif
