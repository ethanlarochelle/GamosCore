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
#ifndef GmPSPrinterSqdose_hh
#define GmPSPrinterSqdose_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"
#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"
class G4PhantomParameterisation;
class GmSqdoseHeader;

// class description:
//
// This class fill a dose file with sum of weights and sum of weights square, to be able to mere dose files

class GmPSPrinterSqdose : public GmVPSPrinter, GmIObinMgr
{
public: // with description
  GmPSPrinterSqdose(G4String);
  virtual ~GmPSPrinterSqdose(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );
  virtual void SetParameters( const std::vector<G4String>& param );

private:
  GmSqdoseHeader* BuildSqdoseHeader();
  void WriteSqdose( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );

private:
  G4double theUnit;
  G4String theUnitName;
  G4PhantomParameterisation* thePhantomParam;

};

#endif
