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
#ifndef GmCone_HH
#define GmCone_HH
#include "G4ThreeVector.hh"
#include <vector>
#include <iostream>
class GmLine;


#include "G4RotationMatrix.hh"

class GmCone 
{

public:  // with description

  // Constructor & destructor.
  GmCone(){};
  GmCone(const G4ThreeVector& pnt, const G4ThreeVector& dir, const G4double angle );
  ~GmCone(){};

  friend std::ostream& operator<<(std::ostream&, const GmCone&);

  G4double GetDist( const G4ThreeVector& ) const;

  std::vector<G4ThreeVector> Intersect( const GmLine& line ) const;

  // Get methods
  G4ThreeVector GetPoint() const{ return thePoint; }
  G4ThreeVector GetDir() const{ return theDir; }
  G4double GetAngle() const{ return theAngle; }
  
protected:
  
  G4ThreeVector thePoint;
  G4ThreeVector theDir;
  G4double theAngle;


  G4RotationMatrix* BuildG4RotMatrixFrom9( const G4ThreeVector& colx, const G4ThreeVector& coly, const G4ThreeVector& colz ) const;
 
};

#endif
