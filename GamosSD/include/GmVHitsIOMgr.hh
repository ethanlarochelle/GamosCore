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
#ifndef GmVHitsIOMgr_h
#define GmVHitsIOMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmVHitsIOMgr    
Author:      P. Arce
Changes:     10/07: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Class to manage the reading and writing of hits  */ 
#include <map>

#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
#include "GmHitList.hh"
class GmHitsEventMgr;
class GmEventTimeExtractorFromHit;
class GmHit;

//------------------------------------------------------------------------
class GmVHitsIOMgr
{ 
public:
  GmVHitsIOMgr();

  ~GmVHitsIOMgr();

  void WriteAllHits();
  void WriteHitsInCurrentEvent();
  void WriteHitsInEvent( G4int evtID );
  G4int GetFirstEventWithHit( gamosSDHitMap hits );

  virtual void WriteHit(GmHit* hit) = 0;
  virtual void ReadHits(){};

protected:
  typedef std::multimap<G4int,unsigned long long> mmil;

  GmHitsEventMgr* theHitsEventMgr;
  GmEventTimeExtractorFromHit* theEventTimeExtractor;

  GmHit* theLastHit;
  G4bool bLastHitRead;
  G4bool bFirstHitRead;

  std::map<G4String,G4double> theEnergyResols;
  std::map<G4String,G4double> theTimeMinResols;
};

#endif
