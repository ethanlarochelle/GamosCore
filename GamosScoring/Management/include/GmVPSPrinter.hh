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
#ifndef GmVPSPrinter_hh
#define GmVPSPrinter_hh 1

#include <vector>
#include "globals.hh"
#include "G4THitsMap.hh"
class GmVPrimitiveScorer;
class GmVPrimitiveScorerVector;
// class description:
//
// This is the base class of the GAMOS GmVPSPrinter.
// It construct the GmVPSPrinter object passing the corresponding arguments

class GmVPSPrinter 
{
public: // with description
  GmVPSPrinter(G4String);
  virtual ~GmVPSPrinter(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer ) = 0;
  virtual void DumpAllVector( std::vector<G4double>*, GmVPrimitiveScorerVector* ){};

  G4String GetName() const {
    return theName; }

  virtual void SetParameters( const std::vector<G4String>& );

  void SetUnit(GmVPrimitiveScorer* theScorer );
  
protected:

  G4String theName;

  G4bool bPrintSumV2;

  G4String theUnitName;
  G4double theUnit;
  G4double theNewUnit;
  G4double theUnitRatio;
  G4double theUnitRatio2;
};

#endif
