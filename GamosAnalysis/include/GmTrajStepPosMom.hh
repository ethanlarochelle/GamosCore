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
#ifndef GmTrajStepPosMom_H
#define GmTrajStepPosMom_H

#include <vector>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GmVTrajStep.hh"
class G4Step;

class GmTrajStepPosMom : public GmVTrajStep
{
public:
  GmTrajStepPosMom( const G4Step* , TSPUse pointToUse = TSPUPrePost);
  virtual ~GmTrajStepPosMom();

  G4ThreeVector GetPositionPre() const { return thePositionPre;}
  G4ThreeVector GetMomentumPre() const { return theMomentumPre;}
  G4ThreeVector GetMomentumDirectionPre() const;
  G4ThreeVector GetPositionPost() const { return thePositionPost;}
  G4ThreeVector GetMomentumPost() const { return theMomentumPost;}
  G4ThreeVector GetMomentumDirectionPost() const;
  G4ThreeVector GetPosition() const;
  G4ThreeVector GetMomentum() const;
  G4ThreeVector GetMomentumDirection() const;

  G4ThreeVector GetDeltaPosition() const;
  G4ThreeVector GetDeltaMomentum() const;
  G4ThreeVector GetDeltaMomentumDirection() const;
  G4double GetDeltaAngle() const;

private:
  G4ThreeVector thePositionPre;
  G4ThreeVector theMomentumPre;
  G4ThreeVector thePositionPost;
  G4ThreeVector theMomentumPost;

};

#endif
