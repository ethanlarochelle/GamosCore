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
#ifndef GmHistoWriterTXT_h
#define GmHistoWriterTXT_h 1

#include "GmHistoWriter.hh"
#include <fstream>

#include "globals.hh"
#ifdef GAMOS_NO_ROOT
class GmHisto1;
class GmHisto2;
class GmHistoProfile1;
class GmHistoProfile2;
#endif

class GmHistoWriterTXT : public GmHistoWriter
{
public:

  GmHistoWriterTXT(const G4String& filename);
  ~GmHistoWriterTXT();
#ifndef GAMOS_NO_ROOT
  virtual void SaveHisto1( const TH1F* his );
  virtual void SaveHisto2( const TH2F* his );
  virtual void SaveHistoProf1( const TProfile* his );
  virtual void SaveHistoProf2( const TProfile2D* his );
private:
  void SaveHisto1D(const G4String& hisType, const TH1* his );
  void SaveHisto2D(const G4String& hisType, const TH2* his );
#else
  virtual void SaveHisto1( const GmHisto1* his );
  virtual void SaveHisto2( const GmHisto2* his );
  virtual void SaveHistoProf1( const GmHistoProfile1* his );
  virtual void SaveHistoProf2( const GmHistoProfile2* his );
 private:
  void SaveHisto1D(const G4String& hisType, const GmHisto1* his );
  void SaveHisto1D(const G4String& hisType, const GmHistoProfile1* his );
  void SaveHisto2D(const G4String& hisType, const GmHisto2* his );
  void SaveHisto2D(const G4String& hisType, const GmHistoProfile2* his );
#endif

  private:
  std::ofstream theFile;

  static G4bool bHistoTXTErrors;

};

#endif
