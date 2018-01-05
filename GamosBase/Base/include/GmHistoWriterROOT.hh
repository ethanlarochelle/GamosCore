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
#ifndef GmHistoWriterROOT_h
#define GmHistoWriterROOT_h 1

#ifndef GAMOS_NO_ROOT
#include "GmHistoWriter.hh"
class TFile;
//class GmHisto1;

#include "globals.hh"

class GmHistoWriterROOT : public GmHistoWriter
{
public:

  GmHistoWriterROOT(const G4String& filename);
  ~GmHistoWriterROOT();
  virtual void SaveHisto1( const TH1F* his );
  virtual void SaveHisto2( const TH2F* his );
  virtual void SaveHistoProf1( const TProfile* his );
  virtual void SaveHistoProf2( const TProfile2D* his );
  //  virtual void SaveHisto1( const GmHisto1* gmhis );

 private:
  void SaveHisto(  const TH1* his );
  //  void SaveHisto( const GmHisto1* gmhis );

  private:
  G4String theFileName;
  TFile* theFile;
};

#endif

#endif
