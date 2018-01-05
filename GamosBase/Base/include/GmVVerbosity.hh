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
#ifndef GmVerbosity_hh
#define  GmVerbosity_hh
#include "globals.hh"
#include <map>

class GmVVerbosity 
{
public:
  GmVVerbosity();
  virtual ~GmVVerbosity(){};

  virtual void SetFilterLevel( int fl ) = 0;
  virtual int GetFilterLevel() const = 0;

  static void AddManager( G4String name, GmVVerbosity* mgr ){
    theManagers[name] = mgr;}
  static GmVVerbosity* FindManager( G4String name, G4bool bMustExist = FALSE );
  static void SetVerbosityLevel( const G4String& verbName, const G4int verbLevel, G4bool bMustExist = FALSE );

private:
#ifdef WIN32
  #if defined GmBaseBase_ALLOC_EXPORT 
    G4DLLEXPORT static std::map<G4String,GmVVerbosity*> theManagers;
  #else 
    G4DLLIMPORT static std::map<G4String,GmVVerbosity*> theManagers;
  #endif
#else
  static std::map<G4String,GmVVerbosity*> theManagers;
#endif
};


#endif
