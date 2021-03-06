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
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4ios.hh"

#include "GmSaveHistosAfterNEvents.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

//----------------------------------------------------------------
GmSaveHistosAfterNEvents::GmSaveHistosAfterNEvents()
{
}

//----------------------------------------------------------------
void GmSaveHistosAfterNEvents::BeginOfRunAction( const G4Run* )
{

  theFirstEvent = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":FirstEvent", 1));
  theEachNEvent = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":EachNEvent", 1000));

}

//----------------------------------------------------------------
void GmSaveHistosAfterNEvents::EndOfEventAction( const G4Event* event )
{

  G4int evtid = event->GetEventID();
  if( evtid >= theFirstEvent && evtid%theEachNEvent == 0 ) {
    std::map<G4String,GmAnalysisMgr*> anaMgrs = GmAnalysisMgr::GetInstances();
    std::map<G4String,GmAnalysisMgr*>::const_iterator itea;
    std::set<G4String> fileFormats = GmAnalysisMgr::GetFileFormats();
    std::set<G4String>::const_iterator ites;
    for( itea = anaMgrs.begin(); itea != anaMgrs.end(); itea++ ){
      GmAnalysisMgr* anaMgr = (*itea).second;
      for( ites = fileFormats.begin(); ites != fileFormats.end(); ites++) {
	anaMgr->Save( anaMgr->GetFileName(), *ites );
      }
    }
  }
}
