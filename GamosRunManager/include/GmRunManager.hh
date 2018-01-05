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
#ifndef GmRunManager_H
#define GmRunManager_H

#include "G4RunManager.hh"
class GmSDMessenger;
class GmAnalysisMessenger;
class GmRunMessenger;

class GmRunManager
{
public:
  GmRunManager();
  virtual ~GmRunManager();

  static GmRunManager* GetRunManager();

  void Initialise();
  virtual void CreateG4RunManager();

  void SelectGeometry( const G4String& );
  void SelectPhysicsList( const G4String& );
  void SelectGenerator( const G4String& );
  void SelectUserAction( const G4String& );
  void SelectVerbosity( const G4String& );
  void SetRandomSeeds( G4int index, G4int nTries );
  void RestoreRandomSeeds( G4int index, G4int nTries );

  G4RunManager* GetG4RunManager() const {
    return theG4RunManager;
  }


protected:
  void InitialiseFactories();

protected:
  static GmRunManager* theRunManager;
  G4RunManager* theG4RunManager;

  G4String theGeometryName;
  G4String thePhysicsListName;
  G4String theGeneratorName;

  GmSDMessenger* theGmSDMessenger;
  GmAnalysisMessenger* theGmAnalysisMessenger;
  GmRunMessenger* theGmRunMessenger;

  G4double theNSeedsInFile;
  G4bool bRestoreSeedsUsed;

};


#endif
