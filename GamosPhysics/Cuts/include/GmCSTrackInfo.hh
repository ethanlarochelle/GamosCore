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
#ifndef GmCSTrackInfo_H
#define GmCSTrackInfo_H 1
#include "GmCutsEnergy2Range.hh"

class G4Region;
class G4VProcess;
class G4ParticleDefinition;
class G4Track;


class GmCSTrackInfo
{
  //  friend std::ostream& operator<<(std::ostream& out, const GmCSTrackInfo&);

  public:
  GmCSTrackInfo(){};
  GmCSTrackInfo(const G4Track* );
  virtual ~GmCSTrackInfo();

  public:
  virtual void Print(std::ostream& out = G4cout ) const;

  G4int GetTrackID() const { return theTrackID;}
  GmCSTrackInfo* GetParent() const { return theParent;}
  G4Region* GetRegion() const { return theRegion;}
  G4VProcess* GetProcess() const { return theProcess;}
  G4ParticleDefinition* GetParticle() const { return theParticle;}
  G4double GetRange() const { return theRange;}
  G4double GetEnergy() const { return theEnergy;}
  G4double GetSafety() const { return theSafety;}
  G4bool IsAccepted() const { return bAccepted;}

  void SetAccepted(G4bool acc ) { bAccepted = acc;}
  void SetParent( GmCSTrackInfo* par ){ theParent = par;}

  static void CreateEnergy2Range() {
    theEnergy2Range = new GmCutsEnergy2Range;
  }
  static GmCutsEnergy2Range* GetEnergy2Range() {
    return theEnergy2Range;
  }

protected:
  G4int theTrackID;
  GmCSTrackInfo* theParent;
  G4Region* theRegion;
  G4VProcess* theProcess;
  G4ParticleDefinition* theParticle;
  G4double theRange;
  G4double theEnergy;
  G4double theSafety;
  G4bool bAccepted;

private:
  static GmCutsEnergy2Range* theEnergy2Range;

};

#endif
