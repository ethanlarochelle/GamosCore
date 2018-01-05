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
#ifndef GAMOS_NO_ROOT
#include "TH1F.h"

#include "TCanvas.h"

#include "GmHistoWriterGraphics.hh"

//----------------------------------------------------------------------
GmHistoWriterGraphics::GmHistoWriterGraphics(const G4String& filename, const G4String& format) : theFileName(filename), theFormat(format)
{
  theCanvas = 0;

}

//----------------------------------------------------------------------
GmHistoWriterGraphics::~GmHistoWriterGraphics()
{
}

//----------------------------------------------------------------------
void GmHistoWriterGraphics::SaveHisto1( const TH1F* his )
{
  SaveHisto( his );
}


//----------------------------------------------------------------------
void GmHistoWriterGraphics::SaveHisto2( const TH2F* his )
{
  SaveHisto( his );
}


//----------------------------------------------------------------------
void GmHistoWriterGraphics::SaveHistoProf1( const TProfile* his )
{
  SaveHisto( his );
}


//----------------------------------------------------------------------
void GmHistoWriterGraphics::SaveHistoProf2( const TProfile2D* his )
{
  SaveHisto( his );
}

//----------------------------------------------------------------------
void GmHistoWriterGraphics::SaveHisto(  const TH1* his )
{
  if( !theCanvas ) {
    theCanvas = new TCanvas("gamosGif");
  }
  TH1* hisNC = const_cast<TH1*>(his);
  hisNC->Draw("");

  G4String histoName = theFileName + "." + his->GetName() + "." + theFormat;
  G4cout << "GmHistorWriterPDF::SaveHisto in file " << histoName << G4endl;
  theCanvas->Update();
  theCanvas->SaveAs(histoName.c_str(),"+");

}

#endif
