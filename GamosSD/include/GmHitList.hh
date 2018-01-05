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
#ifndef GmHitList_HH
#define GmHitList_HH

#include "GamosCore/GamosSD/include/GmHit.hh"
#include <map>
#include <set>
class GmVDeadTimeDUList;
class G4RunManager;

typedef std::vector< GmHit* > hitVector;

enum MeasuringType{ bMTBackwards, bMTInterval, bMTTriggerGlobal, bMTTriggerIndependent };

class GmHitList : public std::vector<GmHit*>
{
public:
  GmHitList( G4String& sdtyp );
  ~GmHitList();

  void AddHit( GmHit* hit );
  const G4String GetSDType() const {
    return theSDType; }  

  void BuildHitsCompatibleInTime( G4double tim = -1. );
  void BuildHitsAll();
  void BuildHitsCompatibleInTimeBackwards( G4double tim = -1. );
  void BuildHitsCompatibleInTimeInterval( G4double tim );
  void BuildHitsCompatibleInTimeTriggerGlobal( G4double tim );
  void BuildHitsCompatibleInTimeTriggerIndependent( G4double tim );

  void AddHitToDeadTimeDetUnitList();

  void CleanHits( G4double tim = -1. );
  void CleanHitsBefore( G4double tim );
  void CleanHitsBefore( G4double tim, unsigned long long trigID );
  void CleanDeadTimeDetUnitList( G4double tim = 1.);

  void DeleteHit( iterator hit );

  const hitVector* GetHitsCompatibleInTime() const {
    return &theHitsCompatibleInTime; 
  }

  G4double GetTriggerTime( G4double hitTime, unsigned long long detUnitID );

  G4double GetMeasuringTime() const {
    return theMeasuringTime; 
  }

 
private:
  
  G4String theSDType;

  G4double theMeasuringTime;
  G4double theDeadTime;
  GmVDeadTimeDUList* theDeadTimeDetUnitList;

  hitVector theHitsCompatibleInTime;

  G4bool bParalizable;

  MeasuringType theMeasuringType;
  G4double theCurrentIntervalStartTime;
  G4double thePreviousEventIntervalTime;
  G4RunManager* theRunManager;

  // for trigger independent 
  std::map<unsigned long long, G4double> theCurrentIntervalStartTimes;
  std::map<unsigned long long, G4double> thePreviousEventIntervalTimes;
  G4int theNShift;
  G4int theNTrigAncestors;
  std::set<unsigned long long> theHitTrigIDs;
};

typedef std::map< G4String, GmHitList* > gamosSDHitMap;

#endif
