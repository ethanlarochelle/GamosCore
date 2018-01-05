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
#ifndef GmUIterminal_h
#define GmUIterminal_h 1

#include "G4UIterminal.hh"
#include "globals.hh"
#include <fstream>

// class description:
//
//  This is a base class of all (G)UI session.
//  SessionStart() method should be called to start the session.
//

class GmUIterminal : public G4UIterminal
{
  // Base class of UI/GUI session
  
public:
  GmUIterminal(G4VUIshell* aShell=0, G4bool qsig=true);
  virtual ~GmUIterminal();
  
  virtual G4int ReceiveG4cout(const G4String& coutString);
  virtual G4int ReceiveG4cerr(const G4String& cerrString);
  // These two methods will be invoked by G4strstreambuf.
  
  void SetCoutFile( const G4String& fname );
  void SetCerrFile( const G4String& fname );
  void SetWriteLog( G4bool bWrite );

private:
  G4String theCoutFileName;
  G4String theCerrFileName;
  std::ofstream* fcout;
  std::ofstream* fcerr;
  G4bool bWriteLogFiles;

};

#endif

