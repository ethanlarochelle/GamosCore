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
#include "GmVDataProf1DHisto.hh"
#include "GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4Event.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmVDataProf1DHisto::GmVDataProf1DHisto() 
{

}

//----------------------------------------------------------------
GmVDataProf1DHisto::~GmVDataProf1DHisto()
{

}

//----------------------------------------------------------------
void GmVDataProf1DHisto::BookHisto( G4String hNamePrefix, G4int hNumber, G4int index, GmAnalysisMgr* anaMgr )
{
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  G4String hName = hNamePrefix+sepa+theName;

  anaMgr->CreateHistoProfile1D(hName, theSubData[0]->GetHNBins(), theSubData[0]->GetHMin(), theSubData[0]->GetHMax(),
			       hNumber);
  theHistosProf1[index] = anaMgr->GetHistoProf1(hNumber);

}


//----------------------------------------------------------------
void GmVDataProf1DHisto::FillHisto( const G4Step* aStep, G4int index )
{
  G4double data1 = theSubData[0]->GetValueFromStep( aStep, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf1DHisto::FillHisto Step data1 " <<  theSubData[0]->GetName() << " = " << data1 << G4endl; 
#endif
  G4double data2 = theSubData[1]->GetValueFromStep( aStep, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf1DHisto::FillHisto Step data2 " <<  theSubData[1]->GetName() << " = " << data2 << G4endl; 
#endif
  theHistosProf1[index]->Fill( data1, data2, aStep->GetTrack()->GetWeight());
}


//----------------------------------------------------------------
void GmVDataProf1DHisto::FillHisto( const G4Track* aTrack, G4int index )
{
  G4double weight = 1.;
  if( aTrack ) weight = aTrack->GetWeight(); // Event or Run DataUser do not pass G4Track pointer

  G4double data1 = theSubData[0]->GetValueFromTrack( aTrack, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf1DHisto::FillHisto Track data1 " << theSubData[0]->GetName() << " = " << data1 << G4endl; 
#endif
  G4double data2 = theSubData[1]->GetValueFromTrack( aTrack, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf1DHisto::FillHisto Track data2 " <<  theSubData[1]->GetName() << " = " << data2 << G4endl; 
#endif
  theHistosProf1[index]->Fill( data1, data2, weight);
  
}

//----------------------------------------------------------------
void GmVDataProf1DHisto::FillHisto( const G4Track* aTrack1, const G4Track* aTrack2, G4int index )
{ 
  //    G4cout << " fill histo " << data->GetName() << " data1 " << theSubData[0]->GetName() << " data2 "<< theSubData[1]->GetName() << G4endl;
  G4double data1 = theSubData[0]->GetValueFromSecoTrack( aTrack1, aTrack2, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf1DHisto::FillHisto SecoTrack data1 " <<  theSubData[0]->GetName() << " = " << data1 << G4endl; 
#endif
  G4double data2 = theSubData[1]->GetValueFromSecoTrack( aTrack1, aTrack2, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf1DHisto::FillHisto SecoTrack data2 " <<  theSubData[1]->GetName() << " = " << data2 << G4endl; 
#endif
  theHistosProf1[index]->Fill( data1, data2, aTrack2->GetWeight());

}

//----------------------------------------------------------------
void GmVDataProf1DHisto::FillHisto( const G4Event* aEvent, G4int index )
{
  G4double data1 = theSubData[0]->GetValueFromEvent( aEvent, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf1DHisto::FillHisto Event data1 " <<  theSubData[0]->GetName() << " = " << data1 << G4endl; 
#endif
  G4double data2 = theSubData[1]->GetValueFromEvent( aEvent, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf1DHisto::FillHisto Event data2 " <<  theSubData[1]->GetName() << " = " << data2 << G4endl; 
#endif
  theHistosProf1[index]->Fill( data1, data2, aEvent->GetPrimaryVertex()->GetWeight());
}

//----------------------------------------------------------------
void GmVDataProf1DHisto::FillHisto( const G4Run* aRun, G4int index )
{
  G4double data1 = theSubData[0]->GetValueFromRun( aRun, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf1DHisto::FillHisto Run data1 " <<  theSubData[0]->GetName() << " = " << data1 << G4endl; 
#endif
  G4double data2 = theSubData[1]->GetValueFromRun( aRun, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf1DHisto::FillHisto Run data2 " <<  theSubData[1]->GetName() << " = " << data2 << G4endl; 
#endif
  theHistosProf1[index]->Fill( data1, data2, 1. );
}
