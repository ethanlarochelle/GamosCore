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
#include "GmVVerbosity.hh"
#include "Reflex/PluginService.h"

/** Constructor   
 */
std::map<G4String,GmVVerbosity*>  GmVVerbosity::theManagers;

GmVVerbosity::GmVVerbosity(){
}


GmVVerbosity* GmVVerbosity::FindManager(  G4String name, G4bool bMustExist )
{
  static std::map<G4String,GmVVerbosity*>::const_iterator ite = theManagers.find(name);
  if( ite == theManagers.end() ){
    if( bMustExist ) {
      for( ite = theManagers.begin(); ite != theManagers.end(); ite++ ){
#ifndef GAMOS_NO_VERBOSE
	//      G4cerr << "VerbosityMgr: " << (*ite).first << G4endl;
#endif
      }
      G4Exception(" GmVVerbosity::FindManager ",   
		  " Manager name not found",
		  FatalErrorInArgument,
		  name);
    } else {
      return 0;
    }
  }

  return (*ite).second;
}

void GmVVerbosity::SetVerbosityLevel( const G4String& verbName, const G4int verbLevel, G4bool bMustExist )
{

  GmVVerbosity* verbMgr = GmVVerbosity::FindManager( verbName, bMustExist );
  if( !verbMgr ) {
    verbMgr = Reflex::PluginService::Create<GmVVerbosity*>(verbName);
    GmVVerbosity::AddManager( verbName, verbMgr );
  }
  if( verbMgr == 0 ) {
    G4Exception("GmRunManager::SelectVerbosity",
		"verbosity does not exists ",
		FatalErrorInArgument,
		verbName);
  }
   
  verbMgr->SetFilterLevel( verbLevel );
  
}

