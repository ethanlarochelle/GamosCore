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
#include "GmHistoWriterCSV.hh"
#include "GmHisto1.hh"
#include "GmHisto2.hh"
#include "GmHistoProfile1.hh"
#include "GmHistoProfile2.hh"

G4bool GmHistoWriterCSV::bHistoCSVErrors = TRUE;

//----------------------------------------------------------------------
GmHistoWriterCSV::GmHistoWriterCSV(const G4String& filename)
{
  theFile.open(filename.c_str());
  //  G4cout << "GmHistoWriterCSV: creating histo file " << filename << G4endl;
  //  theFileName = filename;  

}
 
//----------------------------------------------------------------------
GmHistoWriterCSV::~GmHistoWriterCSV()
{
  theFile.close();
}


 
//----------------------------------------------------------------------
#ifndef GAMOS_NO_ROOT
void GmHistoWriterCSV::SaveHisto1( const TH1F* his )
#else  
void GmHistoWriterCSV::SaveHisto1( const GmHisto1* his )
#endif
{
  SaveHisto1D("1D", his );
}


//----------------------------------------------------------------------
#ifndef GAMOS_NO_ROOT
void GmHistoWriterCSV::SaveHisto2( const TH2F* his )
#else 
void GmHistoWriterCSV::SaveHisto2( const GmHisto2* his )
#endif
{
  SaveHisto2D("2D", his);
}

#ifndef GAMOS_NO_ROOT
//----------------------------------------------------------------------
void GmHistoWriterCSV::SaveHistoProf1( const TProfile* his )
{
  SaveHisto1D("Prof1D", his );
}

//----------------------------------------------------------------------
void GmHistoWriterCSV::SaveHistoProf2( const TProfile2D* his )
{
  SaveHisto2D("Prof2D", his );
}
#else
//----------------------------------------------------------------------
void GmHistoWriterCSV::SaveHistoProf1( const GmHistoProfile1* his )
{
  SaveHisto1D("Prof1D", his );
}

//----------------------------------------------------------------------
void GmHistoWriterCSV::SaveHistoProf2( const GmHistoProfile2* his )
{
  SaveHisto2D("Prof2D", his );
}
#endif

#ifndef GAMOS_NO_ROOT
//----------------------------------------------------------------------
void GmHistoWriterCSV::SaveHisto1D(const G4String& hisType, const TH1* his )
{
  G4int nbins = his->GetXaxis()->GetNbins();
  theFile << '"' << hisType << '"' << "," << '"' << his->GetName() << '"' << "," << nbins << "," << his->GetXaxis()->GetXmin() << "," << his->GetXaxis()->GetXmax();
  for( G4int ii = 0; ii <= nbins+1; ii++ ){
    theFile << "," << his->GetBinContent(ii);
    if( bHistoCSVErrors ) {
      theFile << "," << his->GetBinError(ii);
    }
  }
  theFile << "," << his->GetEntries() << "," << his->GetMean();
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetMeanError();  
  }
  
  theFile << "," << his->GetRMS();  
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetRMSError() << G4endl;  
  } else {
    theFile << G4endl;  
  }    
}

#else
//----------------------------------------------------------------------
void GmHistoWriterCSV::SaveHisto1D(const G4String& hisType, const GmHisto1* his )
{
  G4int nbins = his->GetNbins();
  theFile << '"' << hisType << '"' << "," << '"' << his->GetName() << '"' << "," << nbins << "," << his->GetLowerEdge() << "," << his->GetUpperEdge();
  for( G4int ii = 0; ii <= nbins+1; ii++ ){
    theFile << "," << his->GetBinContent(ii);
    if( bHistoCSVErrors ) {
      theFile << "," << his->GetBinError(ii);
    }
  }
  theFile << "," << his->GetEntries() << "," << his->GetMean();
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetMeanError();
  }
  
  theFile << "," << his->GetRMS();  
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetRMSError() << G4endl;  
  } else {
    theFile << G4endl;  
  }

}

//?? 
//----------------------------------------------------------------------
void GmHistoWriterCSV::SaveHisto1D(const G4String& hisType, const GmHistoProfile1* his )
{
  G4int nbins = his->GetNbins();
  theFile << '"' << hisType << '"' << "," << '"' << his->GetName() << '"' << "," << nbins << "," << his->GetLowerEdge() << "," << his->GetUpperEdge();
  for( G4int ii = 0; ii <= nbins+1; ii++ ){
    theFile << "," << his->GetBinContent(ii);
    if( bHistoCSVErrors ) {
      theFile << "," << his->GetBinError(ii);
    }
  }
  theFile << "," << his->GetEntries() << "," << his->GetMean();
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetMeanError();
  }
  
  theFile << "," << his->GetRMS();  
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetRMSError() << G4endl;  
  } else {
    theFile << G4endl;  
  }

}
#endif

#ifndef GAMOS_NO_ROOT
//----------------------------------------------------------------------
void GmHistoWriterCSV::SaveHisto2D(const G4String& hisType,  const TH2* his )
{
  G4int nbinsx = his->GetXaxis()->GetNbins();
  G4int nbinsy = his->GetYaxis()->GetNbins();
  theFile << '"' << hisType << '"' << "," << '"' << his->GetName() << '"' << "," << nbinsx << "," << his->GetXaxis()->GetXmin() << "," << his->GetXaxis()->GetXmax() << "," << nbinsy << "," << his->GetYaxis()->GetXmin() << "," << his->GetYaxis()->GetXmax();
  for( G4int ii = 0; ii <= nbinsx+1; ii++ ){
    for( G4int jj = 0; jj <= nbinsy+1; jj++ ){
      theFile << "," << his->GetBinContent(ii,jj);
      if( bHistoCSVErrors ) {
	theFile << "," << his->GetBinError(ii,jj);
      }
    }
  }

  theFile << "," << his->GetEntries() << "," << his->GetMean(1);
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetMeanError(1);
  }
  
  theFile << "," << his->GetRMS(1);  
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetRMSError(1);  
  } 

  theFile << "," << his->GetMean(2);
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetMeanError(2);
  }
  
  theFile << "," << his->GetRMS(2);  
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetRMSError(2) << G4endl;  
  } else {
    theFile << G4endl; 
  }
  
}
#else
//----------------------------------------------------------------------
void GmHistoWriterCSV::SaveHisto2D(const G4String& hisType, const GmHisto2* his )
{
  G4int nbinsx = his->GetNbinsX();
  G4int nbinsy = his->GetNbinsY();
  theFile << '"' << hisType << '"' << "," << '"' << his->GetName() << '"' << "," << nbinsx << "," << his->GetLowerEdgeX() << "," << his->GetUpperEdgeX() << "," << nbinsy << "," << his->GetLowerEdgeY() << "," << his->GetUpperEdgeY();
  for( G4int ii = 0; ii <= nbinsx+1; ii++ ){
    for( G4int jj = 0; jj <= nbinsy+1; jj++ ){
      theFile << "," << his->GetBinContent(ii,jj);
      if( bHistoCSVErrors ) {
	theFile << "," << his->GetBinError(ii,jj);
      }
    }
  }

  theFile << "," << his->GetEntries() << "," << his->GetMean(1);
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetMeanError(1);
  }
  
  theFile << "," << his->GetRMS(1);
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetRMSError(1); 
  } 

  theFile << "," << his->GetMean(2);
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetMeanError(2);  
  }
  
  theFile << "," << his->GetRMS(2);  
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetRMSError(2) << G4endl;  
  } else {
    theFile << G4endl;  
  }

}

//----------------------------------------------------------------------
void GmHistoWriterCSV::SaveHisto2D(const G4String& hisType, const GmHistoProfile2* his )
{
  G4int nbinsx = his->GetNbinsX();
  G4int nbinsy = his->GetNbinsY();
  theFile << '"' << hisType << '"' << "," << '"' << his->GetName() << '"' << "," << nbinsx << "," << his->GetLowerEdgeX() << "," << his->GetUpperEdgeX() << "," << nbinsy << "," << his->GetLowerEdgeY() << "," << his->GetUpperEdgeY();
  for( G4int ii = 0; ii <= nbinsx+1; ii++ ){
    for( G4int jj = 0; jj <= nbinsy+1; jj++ ){
      theFile << "," << his->GetBinContent(ii,jj);
      if( bHistoCSVErrors ) {
	theFile << "," << his->GetBinError(ii,jj);
      }
    }
  }

  theFile << "," << his->GetEntries() << "," << his->GetMean(1);
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetMeanError(1);
  }
  
  theFile << "," << his->GetRMS(1);
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetRMSError(1); 
  } 

  theFile << "," << his->GetMean(2);
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetMeanError(2);  
  }
  
  theFile << "," << his->GetRMS(2);  
  if( bHistoCSVErrors ) {
    theFile << "," << his->GetRMSError(2) << G4endl;  
  } else {
    theFile << G4endl;  
  }

}
#endif
