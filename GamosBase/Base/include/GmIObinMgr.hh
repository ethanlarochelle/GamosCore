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
#ifndef GmIObinMgr_h
#define GmIObinMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmIObinMgr    
Author:      P. Arce
---------------------------------------------------------------------------*/ 
/*! Base class for class to manage reading and writing of binary files */ 

#include "GmIOMgr.hh"
#include <fstream>

//------------------------------------------------------------------------
class GmIObinMgr : public GmIOMgr
{ 
 public:
  GmIObinMgr();

  ~GmIObinMgr();

  virtual void Init();

  virtual void CloseFileIn();
  virtual void CloseFileOut();

  virtual void OpenFileIn( const G4String& fname = "" );
  virtual void OpenFileOut( const G4String& fname = "" );
  virtual void OpenFileOutBin( const G4String& fname = "" );

  FILE * GetFileIn() const { return theFileIn; }
  FILE * GetFileOut() const { return theFileOut; }

  void WriteHeaderCheck(std::ofstream* fOut);

protected:
  FILE *theFileIn;
  FILE *theFileOut;
  std::ofstream *theFileOutBin;
};

#endif
