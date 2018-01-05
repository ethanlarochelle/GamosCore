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
#ifndef GmSDSeparateByTime_HH
#define GmSDSeparateByTime_HH

#include "GamosCore/GamosSD/include/GmVSD.hh"

class G4LogicalVolume;
class G4Step;
class GmDetUnitIDBuilderFromAncestors;
class GmHitsEventMgr;

class GmSDSeparateByTime: public GmVSD
{

public:
  GmSDSeparateByTime(G4String type);
  ~GmSDSeparateByTime();

  virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);

  virtual void EndOfEvent(G4HCofThisEvent*HCE);

  virtual unsigned long long GetDetUnitID( G4Step* aStep );
  virtual void CalculateAndSetPosition( GmHit* hit, G4Step* aStep );

  GmDetUnitIDBuilderFromAncestors* GetDetUnitIDBuilder() const { return theDetUnitIDBuilder; }

private:
  GmDetUnitIDBuilderFromAncestors* theDetUnitIDBuilder;

  GmHitsEventMgr* theHitsEventMg;

  G4double theMinimalMeasuringTime;

  G4bool bDiscardBigTimes;
  G4bool bBigTimeCalculated;
  G4double theBigTimeFactor;
  G4double theBiggestHitTime;

};
#endif
