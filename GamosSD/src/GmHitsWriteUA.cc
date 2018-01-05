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
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
 
#include "GmHitsWriteUA.hh"
#include "GmHitsIObinMgr.hh"
#include "GmHitsIOtextMgr.hh"
#include "GmHitsEventMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//---------------------------------------------------------------------
GmHitsWriteUA::GmHitsWriteUA()
{
   bBinFile = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("SD:GmHitsWriteUA:BinFile",1));

   if( bBinFile ) {
     theHitsIOMgr = new GmHitsIObinMgr;
     GmHitsIObinMgr* theHitsIOMgrbin = static_cast<GmHitsIObinMgr*>(theHitsIOMgr);
     theHitsIOMgrbin->OpenFileOut();
   } else {
     theHitsIOMgr = new GmHitsIOtextMgr;
     GmHitsIOtextMgr* theHitsIOMgrtext = static_cast<GmHitsIOtextMgr*>(theHitsIOMgr);
     theHitsIOMgrtext->OpenFileOut();
   }
}

//---------------------------------------------------------------------
void GmHitsWriteUA::EndOfEventAction(const G4Event* evt )
{
  std::vector<GmHit*> hits = GmHitsEventMgr::GetInstance()->GetHitsInEvent( evt->GetEventID() );
  //  G4cout << " GmHitsWriteUA::BeginOfEventAction nhits " << hits.size() << G4endl;
  theHitsIOMgr->WriteAllHits();
}
