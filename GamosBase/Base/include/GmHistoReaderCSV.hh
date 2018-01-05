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
#ifndef GmHistoReaderCSV_h
#define GmHistoReaderCSV_h 1

#include <fstream>

#include "globals.hh"
#ifdef GAMOS_NO_ROOT
#include "GmHisto1.hh"
#include "GmHisto2.hh"
#include "GmHistoProfile1.hh"
#include "GmHistoProfile2.hh"
#else
#include "GmAnalysisMgr.hh" // typedef's to ROOT histos
#endif

#include "GamosCore/GamosUtils/include/GmFileIn.hh"

class GmHistoReaderCSV
{
public:

  GmHistoReaderCSV(const G4String& filename);
  ~GmHistoReaderCSV();

  void ReadFile();

  void FillHisto1D( std::vector<G4String>& wl );
  void FillHisto2D( std::vector<G4String>& wl );
  void FillHistoProfile1D( std::vector<G4String>& wl );
  void FillHistoProfile2D( std::vector<G4String>& wl );

  GmHisto1* GetHisto1( const G4String& histoName );
  GmHisto2* GetHisto2( const G4String& histoName );
  GmHistoProfile1* GetHistoProfile1( const G4String& histoName );
  GmHistoProfile2* GetHistoProfile2( const G4String& histoName );

  std::map<G4String,GmHisto1*> GetHistos1() const {
    return theHistos1;
  }
  std::map<G4String,GmHisto2*> GetHistos2() const {
    return theHistos2;
  }
  std::map<G4String,GmHistoProfile1*> GetHistosProfile1() const {
    return theHistosProfile1;
  }
  std::map<G4String,GmHistoProfile2*> GetHistosProfile2() const {
    return theHistosProfile2;
  }

private:
  void FillHisto1D( std::vector<G4String>& wl, GmHisto1* his );
  void FillHisto2D( std::vector<G4String>& wl, GmHisto2* his );

private:
  G4String theFileName;
  GmFileIn theFile;

  std::map<G4String,GmHisto1*> theHistos1;
  std::map<G4String,GmHisto2*> theHistos2;
  std::map<G4String,GmHistoProfile1*> theHistosProfile1;
  std::map<G4String,GmHistoProfile2*> theHistosProfile2;

  static G4bool bHistoCSVErrors;

};

#endif
