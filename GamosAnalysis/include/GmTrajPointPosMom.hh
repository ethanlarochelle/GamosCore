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
#ifndef GmTrajPointPosMom_H
#define GmTrajPointPosMom_H

#include <vector>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GmVTrajPoint.hh"
class G4StepPoint;

class GmTrajPointPosMom : public GmVTrajPoint
{
public:
  GmTrajPointPosMom(){};
  GmTrajPointPosMom( const G4StepPoint* );
  virtual ~GmTrajPointPosMom();

  G4ThreeVector GetPosition() const { return thePos;}
  G4ThreeVector GetMomentum() const { return theMom; }
  G4ThreeVector GetMomentumDirection() const;
  G4double GetKineticEnergy() const { return  theKineticEnergy;}
  G4double GetTime() const { return theTime;}

private:
  G4ThreeVector thePos;
  G4ThreeVector theMom;
  G4double theKineticEnergy;
  G4double theTime;

};

std::ostream & operator<<(std::ostream & os, const GmTrajPointPosMom &);

#endif
