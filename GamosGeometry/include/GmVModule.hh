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
#ifndef GmVModule_h
#define GmVModule_h

// Base class for geometry modules 
// Pedro Arce
// 31.08.2009: Creation

#include <vector>
#include <map>
#include <sstream>
#include "G4tgrLineProcessor.hh"

class GmVModule { 

 public:
  GmVModule( const std::vector<G4String>& par );
  ~GmVModule();

 public:
  virtual void BuildObjects() = 0;

  friend std::ostream& operator<<(std::ostream&, const GmVModule&);

  const G4String& GetName() const {
    return theName; 
  }
  std::vector<G4String> GetParams() const {
    return theParams;
  }
  G4String GetParams( G4int ii );

  void SetLineProcessor(G4tgrLineProcessor* lp ){
    theLineProcessor = lp;
  }

protected:
  void BuildObject( std::ostringstream& fout );

  G4String PrintW(G4String name );
  G4String PrintWVal(G4String name );

protected:
  G4String theName; 
  std::vector<G4String> theParams;
  std::map<G4String,G4String> theWords;

  G4tgrLineProcessor* theLineProcessor;
}; 

#endif
