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
#ifndef GmPSPrinterHistos_hh
#define GmPSPrinterHistos_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPSPrinter.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"

class GmPSPrinterHistos : 
  public GmVPSPrinter,
  public GmVHistoBuilder
{
public: // with description
  GmPSPrinterHistos(G4String);
  virtual ~GmPSPrinterHistos(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer );
  virtual void SetParameters( const std::vector<G4String>& param );

private:

  G4bool b2D;
  G4String theNameX;
  G4int theNBinsX;
  G4double theMinX;
  G4double theMaxX;
  G4double theOffsetX;
  G4String theNameY;
  G4int theNBinsY;
  G4double theMinY;
  G4double theMaxY;
  G4double theOffsetY;
  G4int theNShift;
  G4double theUnit;
  G4bool bHistoEachRow;
};

#endif
