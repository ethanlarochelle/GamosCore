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
#ifndef GmVDataUserBinFile_hh
#define GmVDataUserBinFile_hh

#include "GmVDataUser.hh"
#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"

class GmVDataUserBinFile : public GmVDataUser,
			   public GmIObinMgr
{
public:
  GmVDataUserBinFile();
  ~GmVDataUserBinFile(){};

protected:
  void WriteHeaderBin();

  void WriteHeaderInfo();

  void WriteHeaderNEvents( G4bool bIsFirst = 0 );

  void WriteHeaderNCalls( G4bool bIsFirst = 0 );

  void WriteHeaderData();

  virtual void EndOfRun();
  //  virtual void EndOfEvent();
  
protected:
  G4int theNCallsWritten;
  G4double theNEvents;

  G4int bWriteHeaderData;
  G4int bWriteHeaderCheck;
  G4int bWriteHeaderNEvents;
  G4int bWriteHeaderNCalls;
};

#endif
