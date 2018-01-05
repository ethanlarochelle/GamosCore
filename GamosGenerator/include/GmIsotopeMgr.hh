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
#ifndef GmIsotopeMgr_HH
#define GmIsotopeMgr_HH

/*---------------------------------------------------------------------------   
ClassName:   GmIsotopeMgr    
Author:      P. Arce
Changes:     24/02/06: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 


#include <vector>
#include "globals.hh"
class GmIsotope;
class GmIsotopeSource;
class GmFileIn;

//------------------------------------------------------------------------
class GmIsotopeMgr 
{ 
 private:
  GmIsotopeMgr(){ };

 public:
  ~GmIsotopeMgr();

  //! Get the only instance 
  static GmIsotopeMgr* GetInstance();

  void ReadIsotopes( const G4String& filename );
  void ReadIsotopes( GmFileIn& fin );

  GmIsotopeSource* AddActiveIsotopeSource(  const G4String& sourceName, const G4String& isoName, const G4double energy );
  GmIsotope* GetIsotope( const G4String& isoName );

  std::vector<GmIsotope*> GetIsotopes() const { return theIsotopes; }
  //-  std::vector<GmIsotopeSource*> GetActiveIsotopes() const { return theActiveIsotopes; }
  //-  void SetIsotopeActivity(G4double);

private:
  static GmIsotopeMgr* theInstance;

  std::vector<GmIsotope*> theIsotopes;

  //-  std::vector<GmIsotopeSource*> theActiveIsotopes;
  
};

#endif
