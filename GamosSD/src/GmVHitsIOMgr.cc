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
#include "GmVHitsIOMgr.hh"
#include "GmHitsEventMgr.hh"
#include "GmSDVerbosity.hh"
#include "GmEventTimeExtractorFromHit.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserAction.hh"

#include "G4RunManager.hh"
#include "G4EventManager.hh"

//-----------------------------------------------------------------------
GmVHitsIOMgr::GmVHitsIOMgr()
{ 
  theHitsEventMgr = GmHitsEventMgr::GetInstance(); // store a copy for optimisation
  theEventTimeExtractor = 0;

  theLastHit = 0;
  bLastHitRead = FALSE;
  bFirstHitRead = FALSE;

}

//-----------------------------------------------------------------------
void GmVHitsIOMgr::WriteAllHits()
{

  gamosSDHitMap hits = theHitsEventMgr->GetHitLists();
 
  GetFirstEventWithHit( hits );

  gamosSDHitMap::const_iterator itehm;
  hitVector::const_iterator iteh;
  //store hits to be written, so that they are later written by event ID, not by SD type
  std::multimap<G4int,GmHit*> hits2write;
  for( itehm = hits.begin(); itehm != hits.end(); itehm++ ){
    GmHitList* hitlist = (*itehm).second;
    const hitVector* hitVector2 = hitlist->GetHitsCompatibleInTime();
    for( iteh = hitVector2->begin(); iteh != hitVector2->end(); iteh++ ){
      GmHit* hit = *iteh;
      WriteHit( hit );
    } 
  }

}

//-----------------------------------------------------------------------
void GmVHitsIOMgr::WriteHitsInCurrentEvent()
{

}


//-----------------------------------------------------------------------
void GmVHitsIOMgr::WriteHitsInEvent( G4int )
{
}


//-----------------------------------------------------------------------
G4int GmVHitsIOMgr::GetFirstEventWithHit( gamosSDHitMap hits )
{
  G4int evtID1 = INT_MAX;
  gamosSDHitMap::const_iterator ite;
  for( ite = hits.begin(); ite != hits.end(); ite++ ){
    if( (*ite).second->size() != 0 ) {
      G4int evtID = (*((*ite).second->begin()))->GetEventID();
      if( evtID < evtID1 ) evtID1 = evtID;
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmVHitsIOMgr::GetFirstEventWithHit " << evtID1 << G4endl;
#endif

  return evtID1;

}


//-----------------------------------------------------------------------
GmVHitsIOMgr::~GmVHitsIOMgr()
{
}

