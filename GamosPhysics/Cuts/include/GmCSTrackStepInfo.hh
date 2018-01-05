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
#ifndef GmCSTrackStepInfo_H
#define GmCSTrackStepInfo_H 1
#include "GmCSTrackInfo.hh"
#include "G4ThreeVector.hh"


class GmCSTrackStepInfo : public GmCSTrackInfo
{
  //  friend std::ostream& operator<<(std::ostream& out, const GmCSTrackStepInfo&);

  public:
    GmCSTrackStepInfo(const G4Track* );
    virtual ~GmCSTrackStepInfo();

  public:
  G4ThreeVector GetPosition() const {
    return  thePosition; }
  void SetPosition( G4ThreeVector pos ) {
    thePosition = pos; }
  G4double GetTrackLength() const {
    return  theTrackLength; }
  void SetTrackLength( G4double tl ){
    theTrackLength = tl; }

  virtual void Print(std::ostream& out  = G4cout) const;

private:
  G4ThreeVector thePosition;
  G4double theTrackLength;
};

#endif
