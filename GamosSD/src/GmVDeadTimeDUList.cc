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
#include "GamosCore/GamosSD/include/GmVDeadTimeDUList.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"
#include "globals.hh"


//----------------------------------------------------------------------
void GmVDeadTimeDUList::Clean(G4double time)
{
  GmVDeadTimeDUList::iterator ite;
  std::vector<GmVDeadTimeDUList::iterator> vite;
  for( ite = begin(); ite != end(); ite++ ){
    if( ((*ite).second).time < time ) {
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << "GmVDeadTimeDUList::Clean  deleting dead time " << ((*ite).second).time << " < " << time << " ID " << (*ite).first << G4endl;
#endif
      vite.push_back( ite );
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << "GmVDeadTimeDUList::Clean  not deleting dead time " << ((*ite).second).time << " > " << time << " ID " << (*ite).first << G4endl;
#endif
    }
  }

  std::vector<GmVDeadTimeDUList::iterator>::iterator itev;
  for( itev = vite.begin(); itev != vite.end(); itev++ ){
    erase(*itev);
  }
}
 
//----------------------------------------------------------------------
G4bool GmVDeadTimeDUList::FindDetUnit( GmHit* hit, G4double deadTime )
{
  unsigned long long du = hit->GetDetUnitID();
  G4double hitTime = hit->GetTime();
  
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmVDeadTimeDUList::FindDetUnit Number of DUs= " << size() << G4endl;
#endif
  du = du/theNShift;
  
  iterator ite = find(du);
  if( ite != end() ) {
    G4double deadHitTime = ((*ite).second).time; 
    if(deadHitTime < hitTime &&  deadHitTime+deadTime >= hitTime && hit->GetEventID() != ((*ite).second).eventID ) {
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(debugVerb) ) G4cout << "GmVDeadTimeDUList::FindDetUnit du dead " << du << " in time " << deadHitTime << " > " << hitTime << G4endl;
#endif
      return true;
      } else {
	G4double deadHitTime2 = ((*ite).second).time;
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(debugVerb) ) G4cout << "GmVDeadTimeDUList::FindDetUnit du not dead " << du << " out of time " << deadHitTime2 << " <= " << hitTime <<  " || " << deadHitTime2+deadTime << " > " << hitTime << " || eventID " << hit->GetEventID() << " = " << ((*ite).second).eventID << G4endl;
#endif
    }
  }
   
  return false;
}
