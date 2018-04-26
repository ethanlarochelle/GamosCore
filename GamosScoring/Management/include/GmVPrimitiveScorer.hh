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
#ifndef GmVPrimitiveScorer_hh
#define GmVPrimitiveScorer_hh 1
// class description:
//
// This is the base class of the GAMOS G4VPrimitiveScorer.
// It construct the G4VPrimitiveSensor object passing the corresponding arguments

#include "GmEnergySplitter.hh"
#include "GamosCore/GamosReadDICOM/include/GmRegularParamUtils.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"
#include <vector>
#include <map>
#include <set>
class GmVFilter;
class GmVPSPrinter;
class GmVClassifier;
class G4VPhysicalVolume;
class GmVData;
class GmVDistribution;
class GmConvergenceTester;
class GmGeometryUtils;
enum ScoreNEventsType { SNET_ByRun, SNET_ByEvent, SNET_ByNFilled };

class GmVPrimitiveScorer : public G4VPrimitiveScorer
{
  friend class G4MultiFunctionalDetector;
  
public: // with description
  GmVPrimitiveScorer(G4String name);
  virtual ~GmVPrimitiveScorer();
  
protected: // with description
  virtual G4int GetIndex(G4Step*);
  // This is a function mapping from copy number(s) to an index of 
  // the hit collection. In the default implementation, just the
  // copy number of the physical volume is taken.

  void AddDefaultPrinter();
  void AddDefaultClassifier();
  
public:
  virtual void SetParameters( const std::vector<G4String>& ){
    G4cout << " GmVPrimitiveScorer::SetParameters: no parameters " << G4endl;
};
   
  virtual void DumpAll(G4THitsMap<G4double>* RunMap);
  
  void Initialize(G4HCofThisEvent* HCE);

  G4bool FillScorer(G4Step* aStep, G4double val, G4double wei);
  G4bool FillScorer(G4Step* aStep, G4int index, G4double val, G4double wei);
  G4bool IsRegularScoring( G4Step*aStep );

  void SumEndOfEvent();

  void SetGmFilter(GmVFilter* f)
  { theFilter = f; }
  inline GmVFilter* GetGmFilter() const
  { return theFilter; }

  void AddPrinter( GmVPSPrinter* prt );

  GmVClassifier* GetClassifier() const{
    return theClassifier; 
  }
  void SetClassifier( GmVClassifier* idx );

  virtual void SetUseTrackWeight( G4bool val ){
    fWeighted = val;}
  virtual void SetScoreErrors( G4bool val ){
    bScoreErrors = val;}

  G4double GetSumV2( G4int index ) {
    if( theSumV2.find( index ) == theSumV2.end() ) return 0.;
    return theSumV2[index];
  }

  std::map<G4int,G4double>* GetSumV2() const {
    return const_cast<std::map<G4int,G4double>* >( &theSumV2 ); 
  }

  void SetSumV2( std::map<G4int,G4double>& sumw2 ) {
    theSumV2 = sumw2; }

  void Normalize(G4THitsMap<G4double>* RunMap);
  void CalculateErrors(G4THitsMap<G4double>* RunMap);
  G4double GetError( G4int index );
  G4double GetErrorRelative( G4int index, G4double sumWX );
  
  void SetNewEvent( G4bool val ){
    bNewEvent = val; }

  G4bool ScoreErrors() const { return bScoreErrors; }
  G4bool UseTrackWeight() const { return fWeighted;}
  G4double GetUnit() const { return theUnit;}
  G4String GetUnitName() const { return theUnitName;}

  virtual void SetScoreByEvent( G4bool val ){
    if( val ) {
      theNEventsType = SNET_ByEvent;
    } else {
      theNEventsType = SNET_ByRun;
    }
  }
  G4bool ScoreByEventType() const { return theNEventsType; }
  void SetNEventsType( ScoreNEventsType net ) {
    theNEventsType = net;
  }
  ScoreNEventsType GetNEventsType() const {
    return theNEventsType;
  }    
    
  virtual G4int GetNEvents( G4int index );
  virtual std::map<G4int,size_t> GetNFilled() const;
  G4int GetNFilled(size_t index) const {
    std::map<G4int,size_t>::const_iterator ite = theNFilled.find(index);
    if( ite != theNFilled.end() ) {
      return G4int((*ite).second);
    } else {
      return -1;
    }
  }
  std::set<size_t> GetNFilled_tmp() const {
    return theNFilled_tmp;
  }
  
  G4bool AcceptByFilter( G4Step*aStep );

  void SetUnit( const G4String& unitName, G4double val );
  void RegisterMFD( G4MultiFunctionalDetector * mfd );

  std::vector<G4LogicalVolume*> GetMFDVolumes();
  void CalculateTotalVolume();
  G4double GetVolume( const G4Step* aStep );
  
  void SetErrors( std::map<G4int,G4double> err ) {
    theError = err;
  }
  
  G4THitsMap<G4double>* GetEvtMap() {
    return EvtMap;
  }
  std::map<G4int,G4double> GetSumV_tmp(){
    return theSumV_tmp;
  }

  G4MultiFunctionalDetector* GetMFD() const {
    return theMFD;
  }

  virtual void PropagateMFDToSubScorers(){};
  virtual void PropagateFilterToSubScorers(){};
  virtual void PropagateClassifierToSubScorers(){};
  virtual void PropagatePrinterToSubScorers(GmVPSPrinter* ){};
  virtual void PropagateTrkWeightToSubScorers(){}; 
  virtual void PropagateScoreErrorsToSubScorers(){};

  void SetDefaultPrinter( G4bool bdf ) {
    bDefaultPrinter = bdf;
  }

  void SetSubScorer( G4bool bss ) {
    bSubScorer = bss;
  }
  G4bool IsSubScorer() const {
    return bSubScorer;
  }
  void ClearSumV_tmp();
  void ClearNFilled_tmp();

  virtual void PrintAll();
  void EndOfEvent(G4HCofThisEvent*);
  void DrawAll();
 
private:
  G4double GetError( G4int index, G4double sumWX, G4double nEvents );

protected:
  GmVFilter* theFilter;
  std::vector<GmVPSPrinter*> thePrinters;
  GmVClassifier* theClassifier;
  G4THitsMap<G4double>* EvtMap;
  G4int HCID;
  G4bool fWeighted;
  //--- Error calculations
  std::map<G4int,G4double> theSumV_tmp;
  std::map<G4int,G4double> theSumV2;
  std::map<G4int,G4double> theError;

  G4bool bNewEvent;

  G4bool bScoreErrors;
  G4double theUnit;
  G4String theUnitName;

  ScoreNEventsType theNEventsType;
  std::map<G4int,size_t> theNFilled;
  std::set<size_t> theNFilled_tmp;
  
  G4double sumALL; 

  GmVData* theMultiplyingData;
  GmVDistribution* theMultiplyingDistribution;

  GmConvergenceTester* theConvergenceTester;

  GmEnergySplitter* theEnergySplitter;

  G4bool bSkipEqualMaterials;
  G4bool bUseClassifierIndex;

  GmRegularParamUtils* theRegularParamUtils;

  G4MultiFunctionalDetector* theMFD;
  GmGeometryUtils* theGeometryUtils;
  G4bool bIntegrateVolumes;
  G4double theTotalVolume;
  G4bool bUseVolume;
  G4bool bDefaultPrinter;
  G4bool bSubScorer;
};

#endif
