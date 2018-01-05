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
#include "GmUIterminal.hh"
#include "GmUIMessenger.hh"


//-------------------------------------------------------------------------
GmUIterminal::GmUIterminal(G4VUIshell* aShell, G4bool qsig) 
  : G4UIterminal( aShell, qsig )
{
  fcout = 0;
  fcerr = 0;
  new GmUIMessenger(this);
  bWriteLogFiles = true;
}


//-------------------------------------------------------------------------
GmUIterminal::~GmUIterminal() 
{
  if(fcout) delete fcout;
  if(fcerr) delete fcerr;
}


//-------------------------------------------------------------------------
G4int GmUIterminal::ReceiveG4cout(const G4String& coutString)
{
  if( fcout && bWriteLogFiles ) {
    *fcout <<  coutString << std::flush; 
  }
  std::cout <<  coutString << std::flush;
  return 0;
}


//-------------------------------------------------------------------------
G4int GmUIterminal::ReceiveG4cerr(const G4String& cerrString)
{
  if( fcerr && bWriteLogFiles ) {
    *fcerr <<  cerrString << std::flush; 
  }
  std::cerr <<  cerrString << std::flush;
  return 0;
}                                                                       


//-------------------------------------------------------------------------
void GmUIterminal::SetCoutFile( const G4String& fname )
{
  //--- Check if cout and cerr go to the same file
  if( fcerr && theCerrFileName == fname ) {
    fcout = fcerr;
  } else {
    theCoutFileName = fname;
    fcout = new std::ofstream(fname);
  }
}


//-------------------------------------------------------------------------
void GmUIterminal::SetCerrFile( const G4String& fname )
{
  //--- Check if cout and cerr go to the same file
  if( fcout && theCoutFileName == fname ) {
    fcerr = fcout;
  } else {
    theCerrFileName = fname;
    fcerr = new std::ofstream(fname);
  }
}


//-------------------------------------------------------------------------
void GmUIterminal::SetWriteLog( G4bool bWrite )
{
  bWriteLogFiles = bWrite;
}
