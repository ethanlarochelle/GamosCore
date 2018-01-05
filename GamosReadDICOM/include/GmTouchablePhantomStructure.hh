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
#ifndef GmTouchablePhantomStructure_h
#define GmTouchablePhantomStructure_h 1

class G4VPhysicalVolume;
class G4VTouchable;
#include "GamosCore/GamosBase/Filters/include/GmVTouchableFilterVolume.hh"
#include <vector>
#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters touchables by G4VPhysicalVolume
// 
///////////////////////////////////////////////////////////////////////////////
class GmRegularParamUtils;
class GmReadPhantomPSMgr;

class GmTouchablePhantomStructure : public GmVTouchableFilterVolume 
{

public: // with description
  GmTouchablePhantomStructure();
  
  virtual ~GmTouchablePhantomStructure();
  
public: // with description
  virtual G4bool AcceptTouchable( const G4VTouchable* );

  void show();

  virtual void SetParameters( std::vector<G4String>& params);
  G4bool CheckIndex( G4int idx );

protected:
  std::set<G4int> theIndices;

  GmRegularParamUtils* theRegularUtils;
  GmReadPhantomPSMgr* thePVMgr;
  G4int theIdxShift;

};

#endif
