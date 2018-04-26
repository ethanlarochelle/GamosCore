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
#include "GmModuleMgr.hh"
#include "GmGeomVerbosity.hh"
#include "GmVModule.hh"

GmModuleMgr* GmModuleMgr::theInstance = 0;

//-----------------------------------------------------------------------
GmModuleMgr* GmModuleMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmModuleMgr;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
GmModuleMgr::GmModuleMgr() 
{
}
 

//-----------------------------------------------------------------------
GmModuleMgr::~GmModuleMgr() 
{
}
 
//------------------------------------------------------------------------
GmVModule* GmModuleMgr::GetModule( G4String name, G4bool bMustExist )
{
  GmVModule* module = 0;
  
  std::map<G4String, GmVModule*>::const_iterator item = theModules.find(name);
  if( item == theModules.end() ) {
    G4ExceptionSeverity es;
    if( bMustExist ) {
      es = FatalException;
    } else {
      es = JustWarning;
    }
    G4Exception("GmModuleMgr::GetModule",
		"",
		es,
		("GAMOS Module does not exist " + name).c_str());    
  } else {
    module = (*item).second;		 
  }

  return module;
}

