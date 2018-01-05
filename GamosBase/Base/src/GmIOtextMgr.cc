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
#include "GmIOtextMgr.hh"
#include "GmParameterMgr.hh"
#include "GmBaseVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include <iomanip>
#include <iostream>

//-----------------------------------------------------------------------
GmIOtextMgr::GmIOtextMgr()
{
  Init();
}

//-----------------------------------------------------------------------
GmIOtextMgr::~GmIOtextMgr()
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(infoVerb) ) G4cout << " ~GmIOtextMgr( theFileOut " << theFileOut << " " << theFileNameOut << G4endl;
#endif
  CloseFileIn();
  CloseFileOut();

}

//-----------------------------------------------------------------------
void GmIOtextMgr::Init()
{
  theFileIn = 0;
  theFileOut = 0;

  InitDebugging();
  
}

//-----------------------------------------------------------------------
void GmIOtextMgr::OpenFileIn( const G4String& fname )
{
  if( theFileIn != 0 ){
    G4Exception("GmIOtextMgr::OpenFileIn",
		"Wrong argument",
		FatalErrorInArgument,
		"File already opened");
  } 
  
  SetFileNameIn( fname );
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  theFileNameIn = GmGenUtils::FileInPath( path, theFileNameIn );

  theFileIn =  & GmFileIn::GetInstance(theFileNameIn);
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(infoVerb) ) G4cout << " GmIOtextMgr::OpenFileIn(): " << theFileNameIn << " = " << theFileIn << G4endl;
#endif
}

//-----------------------------------------------------------------------
void GmIOtextMgr::OpenFileOut( const G4String& fname )
{
  if( theFileOut != 0 ){
    G4Exception("GmIOtextMgr::OpenFileOut",
		"Wrong argument",
		FatalErrorInArgument,
		("File already opened: " + fname).c_str());
  }

  SetFileNameOut( fname );

  theFileOut = new std::ofstream(theFileNameOut);
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(infoVerb) ) G4cout << " GmIOtextMgr::OpenFileOut(): " << theFileNameOut << G4endl;
#endif
}

//-----------------------------------------------------------------------
void GmIOtextMgr::CloseFileIn()
{
  if( theFileIn ) {
    theFileIn->Close();
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(infoVerb) ) G4cout << " GmIOtextMgr::CloseFileIn() file closed " << theFileNameIn << G4endl;
#endif
    delete theFileIn;
    theFileIn = 0;
  }
}

//-----------------------------------------------------------------------
void GmIOtextMgr::CloseFileOut()
{
  if( theFileOut ) {
    theFileOut->close();
#ifndef GAMOS_NO_VERBOSE
        if( BaseVerb(infoVerb) ) G4cout << " GmIOtextMgr::CloseFileOut() file closed " << theFileNameOut << G4endl;
#endif
    delete theFileOut;
    theFileOut = 0;
  }
}
