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
#ifndef GmSolid_h
#define GmSolid_h

// Class that identifies a volume item ( = G4VTouchable with a format independent of G4 
// and some extra information)
// Pedro Arce
// 20.02.2001: Creation

#include <vector>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4Point3D.hh"
class G4VPhysicalVolume;
class G4VSolid;
class G4tgrSolid;

class GmSolid { 

 public:
  GmSolid(){};
  GmSolid( const G4VSolid*);
  GmSolid( const G4tgrSolid*);
  //  GmSolid( G4TouchableHistory* g4touch );
  ~GmSolid();

// get methods
 public:
  friend std::ostream& operator<<(std::ostream&, const GmSolid&);

  const G4String& GetName() const {
    return theName; }
  G4Point3D GetRandomPoint() const;

private: 
  //! The name 
  G4String theName; 
  G4String theType; 
  std::vector<G4double> theParams;

}; 

#endif
