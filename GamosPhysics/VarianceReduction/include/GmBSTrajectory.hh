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
#ifndef GmBSTrajectory_H
#define GmBSTrajectory_H

#include "GamosCore/GamosAnalysis/include/GmVTrajectory.hh"
#include "G4ThreeVector.hh"

class GmBSTrajectory : public GmVTrajectory
{
public:
  GmBSTrajectory();
  GmBSTrajectory(const G4Track* aTrack);
  virtual ~GmBSTrajectory();

  G4double GetPosX() const { return fInitialPosition.x(); }
  G4double GetPosY() const { return fInitialPosition.y(); }
  G4double GetPosZ() const { return fInitialPosition.z(); }
  G4double GetDirectionTheta() const { return fInitialMomentum.theta(); }
  G4double GetDirectionPhi() const { return fInitialMomentum.phi(); }
  G4double GetEnergy() const { return fEnergy; }

  void Clear();

  // Get/Set functions 

private:

  G4ThreeVector fInitialPosition;
  G4double fEnergy;

};

#endif
