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
#ifndef GmPSPrinter3ddose_hh
#define GmPSPrinter3ddose_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"
#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
class G4PhantomParameterisation;

// class description:
//
// This class fill a dose file in 3ddose (DOSXYZnrc) format

class GmPSPrinter3ddose : public GmVPSPrinter, GmIOtextMgr
{
public: // with description
  GmPSPrinter3ddose(G4String);
  virtual ~GmPSPrinter3ddose(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:
  void Write3ddoseHeader();
  void Write3ddose( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:
  G4PhantomParameterisation* thePhantomParam;

};

#endif
