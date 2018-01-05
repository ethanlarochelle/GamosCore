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
#ifndef GmAnalysisMgr_h
#define GmAnalysisMgr_h 1

#ifndef GAMOS_NO_ROOT
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TFile.h"
typedef TH1F GmHisto1;
typedef TH2F GmHisto2;
typedef TProfile GmHistoProfile1;
typedef TProfile2D GmHistoProfile2;
#else 
#include "GamosCore/GamosBase/Base/include/GmHisto1.hh"
#include "GamosCore/GamosBase/Base/include/GmHisto2.hh"
#include "GamosCore/GamosBase/Base/include/GmHistoProfile1.hh"
#include "GamosCore/GamosBase/Base/include/GmHistoProfile2.hh"
#endif 

#include "globals.hh"
#include <map>
#include <set>
#include <string>

//#include "HTL/Histograms.h" // Transient histograms.
typedef std::map< int, GmHisto1* > mih1;
typedef std::map< int, GmHisto2* > mih2;
typedef std::map< int, GmHistoProfile1* > mihp1;
typedef std::map< int, GmHistoProfile2* > mihp2;

class GmAnalysisMgr 
{
private:
  GmAnalysisMgr(const G4String& filename);

public:

  ~GmAnalysisMgr();
  void Save( const G4String& name, const G4String& format );

  static GmAnalysisMgr* GetInstance(const G4String& filename);
  static std::map<G4String,GmAnalysisMgr*> GetAllInstances()
  {return theInstances; }

  //  bool CreateHistogram1D(const G4String & path, const G4String & title, int nBins, double lowerEdge, double upperEdge, int nHisto = -1);
  bool CreateHisto1D(const G4String & pathAndTitle, int nBins, double lowerEdge, double upperEdge, int nHisto = -1 );
  //  bool CreateHisto1D(const G4String & pathAndTitle, int nBins, double lowerEdge, double upperEdge, int nHisto = -1 ) { 
  //    return CreateHistogram1D( pathAndTitle, nBins, lowerEdge, upperEdge, nHisto ); }

  //  bool CreateHistogram2D(const G4String & path, const G4String & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nHisto = -1);
  bool CreateHisto2D(const G4String & pathAndTitle, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nHisto = -1);
  // bool CreateHisto2D(const G4String & pathAndTitle, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nHisto = -1) { 
  //   return CreateHistogram2D( pathAndTitle, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, nHisto); }

  bool CreateHistoProfile1D(const G4String & pathAndTitle, int nBins, double lowerEdge, double upperEdge, int nHisto = -1 );

  bool CreateHistoProfile2D(const G4String & pathAndTitle, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nHisto = -1 );


  GmHisto1* GetHisto1D( int ih, bool itExists = TRUE );
  GmHisto1* GetHisto1D( const G4String& hnam, bool itExists = TRUE );
  GmHisto1* GetHisto1( int ih, bool itExists = TRUE ){ return GetHisto1D( ih, itExists); }
  GmHisto2* GetHisto2D( int ih, bool itExists = TRUE );
  GmHisto2* GetHisto2( int ih, bool itExists = TRUE ){ return GetHisto2D( ih, itExists); }
  GmHisto2* GetHisto2D( const G4String& hnam, bool itExists = TRUE );
  GmHistoProfile1* GetHistoProf1( int ih, bool itExists = TRUE );
  GmHistoProfile2* GetHistoProf2( int ih, bool itExists = TRUE );
  GmHistoProfile1* GetHistoProf1( const G4String& hnam, bool itExists = TRUE );
  GmHistoProfile2* GetHistoProf2( const G4String& hnam, bool itExists = TRUE );

  void HistoDoesNotExists( int ih, const G4String& hisname, bool itExists = TRUE);
  void HistoDoesNotExists( const G4String& hnam, const G4String& histype, bool itExists = TRUE );

  static void DeleteInstances();

  static void AddFileFormat( G4String& ff );
  static void SetFileFormat( G4String& ff );

  static void AddHisto1NBins(G4String values);
  static void AddHisto1Min(G4String values);
  static void AddHisto1Max(G4String values);
  static void AddHisto2NBinsX(G4String values);
  static void AddHisto2MinX(G4String values);
  static void AddHisto2MaxX(G4String values);
  static void AddHisto2NBinsY(G4String values);
  static void AddHisto2MinY(G4String values);
  static void AddHisto2MaxY(G4String values);

  static void AddHistoPrint(G4String values);

  static void AddHistoNorm(G4String values);

  void DeleteEmptyHistos();
  static std::map<G4String,GmAnalysisMgr*> GetInstances() { 
    return theInstances;
  }
  static std::set<G4String> GetFileFormats() {
    return theFileFormats; 
  }
  G4String GetFileName() const {
    return theFileName;
  }

  void SetDefaultNormalize( G4bool bdn ) {
      bDefaultNormalize = bdn;
  }

private:
  G4int BuildHistoNumber( const G4String& htyp);
  template< class TYP >
  G4int BuildHistoNumberForType( const std::map< int, TYP* >& histos);
  void PrintInfo();
  void PrintInfo1( GmHisto1* his, G4int iverbose );
  void PrintInfo2( GmHisto2* his, G4int iverbose );
  void Normalize();
  void NormalizeToNEvents( GmHisto1* his );
  void NormalizeToNEvents( GmHisto2* his );

 private:
  static std::map<G4String,GmAnalysisMgr*> theInstances;

  mih1 theHistos1;
  mih2 theHistos2;
  mihp1 theHistosProf1;
  mihp2 theHistosProf2;

  G4String theFileName;
  static std::set<G4String> theFileFormats; 

  static std::map<G4String,G4double> theHisto1NBins;
  static std::map<G4String,G4double> theHisto1Min;
  static std::map<G4String,G4double> theHisto1Max;
  static std::map<G4String,G4double> theHisto2NBinsX;
  static std::map<G4String,G4double> theHisto2MinX;
  static std::map<G4String,G4double> theHisto2MaxX;
  static std::map<G4String,G4double> theHisto2NBinsY;
  static std::map<G4String,G4double> theHisto2MinY;
  static std::map<G4String,G4double> theHisto2MaxY;

  static std::map<G4String,G4int> theHistoPrint;
  static std::map<G4String,G4double> theHistoNormFactor;

  G4bool bNormalizeToNEvents;
  G4bool bDefaultNormalize;
};

#endif
