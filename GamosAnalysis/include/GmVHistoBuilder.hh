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
#ifndef GmVHistoBuilder_HH
#define GmVHistoBuilder_HH

#include "globals.hh"
#include <vector>
#include <set>
class GmVFilter;
class GmVClassifier;
class GmAnalysisMgr;

class GmVHistoBuilder 
{
public:
  GmVHistoBuilder();

  void SetHistoNameAndNumber(const G4String& fname, std::vector<GmVFilter*>& filters, GmVClassifier* classifier );
  void BuildHistoName(const G4String& name, std::vector<GmVFilter*>& filters, GmVClassifier* classifier );
  void BuildHistoFileName(const G4String& name, std::vector<GmVFilter*>& filters, GmVClassifier* classifier );
  void BuildHistoNumber();

  void PrintHisto( G4int hn );

protected:
  G4String theHistoName;
  G4String theHistoFileName;
  G4int theHistoNumber;
  static std::set<G4int> theHistoNumbers;

  GmAnalysisMgr* theAnaMgr;

  G4bool bPrintHistoStats;
};

#endif
