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
#ifndef GmScoringMgr_H
#define GmScoringMgr_H

#include "G4RunManager.hh"
class G4MultiFunctionalDetector;
class GmVPrimitiveScorer;
class GmVFilter;
class GmVPSPrinter;
class GmVClassifier;
class GmScoringRun;
#include "GmVPrimitiveScorer.hh"
#include <map>


class GmScoringMgr
{
private:
  GmScoringMgr();

public:
  ~GmScoringMgr();

  static GmScoringMgr* GetInstance();

  void CreateMFD( const std::vector<G4String>& wl );
  void CreateScorer( const G4String& scorerName, const G4String& scorerClass, std::vector<G4String>& params );
  void AddScorer2MFD( std::vector<G4String>& wl );
  void AddFilter2Scorer( std::vector<G4String> params ); 
  void AddPrinter2Scorer(std::vector<G4String> params ); 
  void AssignClassifier2Scorer( std::vector<G4String>& params ); 
  void AddTrkWeight2Scorer( const G4String& scorerName, G4String& val );
  void AddScoreErrors2Scorer( const G4String& scorerName, G4String& val );
  void AddPrintByEvent2Scorer( const G4String& scorerName, G4String& val );
  void AddPrintNEventsType2Scorer( const G4String& scorerName, G4String& val );
  void ChangeScorerUnit( const G4String& scorerName, const G4String& unitName, G4String& unitValue );

  void PrintAllScorers();

  void AddScorer( G4String scorerName, GmVPrimitiveScorer* scorer ) {
    theScorers[scorerName] = scorer;
  }
  std::map<G4String,GmVPrimitiveScorer*> GetScorers() const{ 
    return theScorers; }
  std::map<G4String,G4MultiFunctionalDetector*> GetMFDs() const {
    return theMFDs; }

  G4THitsMap<G4double>* GetRunMap(GmVPrimitiveScorer* scorer );

  void SetScoringRun( GmScoringRun* sr ) {
    theScoringRun = sr;
  }

  GmVPrimitiveScorer* GetScore( G4String, G4bool bMsutExists ) const;

private:
  static GmScoringMgr* theInstance;

  GmScoringRun* theScoringRun;
  std::map<G4String,G4MultiFunctionalDetector*> theMFDs;
  std::map<G4String,GmVPrimitiveScorer*> theScorers;
  //  std::map<G4String,GmVFilter*> theFilters;
  std::map<G4String,GmVPSPrinter*> thePrinters;
  //  std::map<G4String,GmVClassifier*> theClassifiers;

  G4bool bPrintingUA;

};

#endif
