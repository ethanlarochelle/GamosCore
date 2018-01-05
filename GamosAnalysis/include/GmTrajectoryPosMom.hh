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
#ifndef GmTrajectoryPosMom_H
#define GmTrajectoryPosMom_H

#include <vector>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GmTrajectory.hh"
class GmTrajPointPosMom;

class GmTrajectoryPosMom : public GmTrajectory
{
public:
  GmTrajectoryPosMom();
  GmTrajectoryPosMom(const G4Track* aTrack);
  virtual ~GmTrajectoryPosMom();
 
  void Clear();

  void AddPoint( GmTrajPointPosMom* point ){
    thePoints.push_back( point ); }
 
  // Get/Set functions 
  inline G4int GetTrackID() const
  { return fTrackID; }
  inline G4int GetParentID() const
  { return fParentID; }
  inline G4String GetParticleName() const
  { return fParticleName; }
  inline G4double GetCharge() const
  { return fPDGCharge; }
  inline G4int GetPDGEncoding() const
  { return fPDGEncoding; }
  inline G4ThreeVector GetInitialMomentum() const
  { return fInitialMomentum; }
  
  const std::vector<GmTrajPointPosMom*> GetPoints() const{
    return thePoints;}
  const GmTrajPointPosMom* GetPoint(G4int ipos ){
    return thePoints[ipos];}
  G4int GetNumberOfPoints(){
    return thePoints.size();}

private:
  G4int                     fTrackID;
  G4int                     fParentID;
  G4int                     fPDGEncoding;
  G4double                  fPDGCharge;
  G4String                  fParticleName;
  G4ThreeVector             fInitialMomentum;

  std::vector<GmTrajPointPosMom*> thePoints;


};

#endif
