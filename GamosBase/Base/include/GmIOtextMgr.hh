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
#ifndef GmIOtextMgr_h
#define GmIOtextMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmIOtextMgr    
Author:      P. Arce
---------------------------------------------------------------------------*/ 
/*! Base class for class to manage reading and writing of text files */ 

#include "GmIOMgr.hh"
#include <fstream>
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

//------------------------------------------------------------------------
class GmIOtextMgr : public GmIOMgr
{ 
 public:
  GmIOtextMgr();

  ~GmIOtextMgr();

  virtual void Init();

  virtual void CloseFileIn();
  virtual void CloseFileOut();

  virtual void OpenFileIn( const G4String& fname = "" );
  virtual void OpenFileOut( const G4String& fname = "" );

  GmFileIn * GetFileIn() const { return theFileIn; }
  std::ofstream * GetFileOut() const { return theFileOut; }

protected:
  GmFileIn *theFileIn;
  std::ofstream *theFileOut;
};

#endif
