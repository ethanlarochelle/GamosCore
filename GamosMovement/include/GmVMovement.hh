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
///////////////////////////////////////////////////////////////////////////////
// File: GmVMovement.h
// Date: 23/02/2007 P.Arce
// Description: Base class to define a movement
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmVMovement_h
#define GmVMovement_h 1

#include "globals.hh"
#include <vector>
class G4VPhysicalVolume;
#include "G4ThreeVector.hh"

enum GmMovementType{ AfterTime, AfterNEvents };

class GmVMovement 
{
public:
  GmVMovement(){};
  GmVMovement(std::vector<G4String>& words, GmMovementType mt );
  ~GmVMovement();
  
  virtual G4bool IsNewMovement( G4double );
  virtual G4bool Move() = 0;

  void InitialiseLastValue();

  GmMovementType GetType() const{
    return theType; }
  G4double GetLastValue() const {
    return theLastValue;}
  
protected:
  std::vector<G4VPhysicalVolume*> theVolumes;
  GmMovementType theType; 
  G4double theValue;
  G4ThreeVector theAxis;
  G4double theInterval;
  G4double theOffset;
  G4int theNIntervals;
  G4int theNMoved;

  G4double theLastValue; // last value of time or nevents when a movement was made
};

#endif
