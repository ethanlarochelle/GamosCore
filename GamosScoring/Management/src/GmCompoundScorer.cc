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
//#define VERBOSE_DOSEDEP
#include "GmCompoundScorer.hh"
#include "G4VPrimitiveScorer.hh"
#include "GmScoringMgr.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"

#include "Reflex/PluginService.h"

//--------------------------------------------------------------------
GmCompoundScorer::GmCompoundScorer(G4String name) 
   :GmVPrimitiveScorer(name)
{ 
  theUnit = 1;
  theUnitName = "";

  bInitialized = false;

  theScoringMgr = GmScoringMgr::GetInstance();
}


//--------------------------------------------------------------------
G4bool GmCompoundScorer::ProcessHits(G4Step* ,G4TouchableHistory*)
{
  if( !bInitialized ) {
    //--- set unit
    if( theSubExpressions.size() == 0 ) {
      theUnit = theSubScorers[0]->GetUnit();
      theUnitName = theSubScorers[0]->GetUnitName();
    } else {
      G4String expression;
      unsigned int siz = theSubExpressions.size();
      unsigned int sizScorers = theSubScorers.size();
      for( unsigned int ii = 0; ii < siz; ii++ ){
	expression += theSubExpressions[ii];
      if( ii < sizScorers )  expression += GmGenUtils::ftoa( theSubScorers[ii]->GetUnit());
      //      G4cout << ii << " EXPRESSION " << expression << G4endl;//GDEB
      }
      theUnitName = expression;
      theUnit = GmGenUtils::GetValue( expression );
    }
    bInitialized = true;
  }

  // subscorers already called by MFD
  /*  std::vector<GmVPrimitiveScorer*>::const_iterator ite;
  for( ite = theSubScorers.begin(); ite != theSubScorers.end(); ite++ ){
    (*ite)->ProcessHits(aStep, (G4TouchableHistory*)0 );
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb)) G4cout  << "GmCompoundScorer::ProcessHits for scorer " << (*ite)->GetName() << G4endl;
#endif
  }
  */

  //-  FillScorer( aStep, -1, 0., 0., true ); // bOnlyNewEvent=true
  
  return TRUE;
} 

//--------------------------------------------------------------------
void GmCompoundScorer::EndOfEvent(G4HCofThisEvent*)
{
}

//--------------------------------------------------------------------
void GmCompoundScorer::DrawAll()
{;}

//--------------------------------------------------------------------
void GmCompoundScorer::AddSubScorer(GmVPrimitiveScorer* scorer )
{
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmCompoundScorer::AddSubScorer " << theSubScorers.size() << " scorer " << scorer->GetName() << G4endl; 
#endif

  theSubScorers.push_back( scorer );
  scorer->SetSubScorer(true);
}


//--------------------------------------------------------------------
void GmCompoundScorer::AddSubExpression( G4String expr )
{
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmCompoundScorer::AddSubExpression " << theSubExpressions.size() << " expr " << expr << G4endl; 
#endif

  theSubExpressions.push_back( expr );
}

//--------------------------------------------------------------------
void GmCompoundScorer::BuildCompoundScores()
//-- Fill SumV_tmp and EvtMap from subScorers
{
  unsigned int siz = theSubScorers.size();
  std::vector<std::map<G4int,G4double>> SumV_tmps;
  std::vector<std::set<size_t>> NFilled_tmps;
  for( unsigned int ii = 0; ii < siz; ii++ ){
    SumV_tmps.push_back( theSubScorers[ii]->GetSumV_tmp() );
    NFilled_tmps.push_back( theSubScorers[ii]->GetNFilled_tmp() );
  }
  //  G4cout << " GmCompoundScorer::BuildCompoundScores SumV_size " << SumV_tmps.size() << G4endl; //GDEB
  std::map<G4int,G4double>::iterator itee;

  std::map<G4int,G4double> sumV_tmp0 = SumV_tmps[0];
 #ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << " GmCompoundScorer::BuildCompoundScores " << GetName() << " Nindices " << sumV_tmp0.size() << G4endl;
#endif
  unsigned int sizScorers = theSubScorers.size();
  for( itee = sumV_tmp0.begin(); itee != sumV_tmp0.end(); itee++ ){
    //-      G4cout << " EvtMap " << (*itee).first << " = " << (*itee).second << G4endl;
    G4int index = (*itee).first;
    //    G4cout << "GmCompoundScorer::BuildCompoundScores() CS " << index << G4endl; //GDEB
    G4String expression;
    unsigned int sizExpr = theSubExpressions.size();
    for( unsigned int ii = 0; ii < sizExpr; ii++ ){
      expression += theSubExpressions[ii];
      if( ii < sizScorers ) expression += GmGenUtils::ftoa( (SumV_tmps[ii])[index]);
      //      G4cout << ii << " CS expression " << expression << " " << theSubExpressions[ii] << " " << (SumV_tmps[ii])[index] << G4endl; //GDEB
    }
    G4double newVal = GmGenUtils::GetValue( expression );
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(testVerb) ) G4cout << " GmCompoundScorer::BuildCompoundScores " << GetName() << " index "<< index << " expression " << expression << " " << newVal << G4endl; 
#endif
    theSumV_tmp[index] = newVal;
    //    G4cout << this << " " << GetName() << " BUILD " << bFillEvtMap << " " << EvtMap << G4endl; //GDEB
    EvtMap->add(index,newVal);
  }

  std::vector<std::set<size_t>>::const_iterator itemnf;
  std::set<size_t> NFilled_tmpAll;
  std::set<size_t>::const_iterator itenf;
  for( itemnf = NFilled_tmps.begin(); itemnf != NFilled_tmps.end(); itemnf++ ) {
    for( itenf = (*itemnf).begin(); itenf != (*itemnf).end(); itenf++ ) {
      NFilled_tmpAll.insert(*itenf); // all indices from all scores
    }
  }
  G4bool bFound = true;
  for( itenf = NFilled_tmpAll.begin(); itenf != NFilled_tmpAll.end(); itenf++ ) {
    for( itemnf = NFilled_tmps.begin(); itemnf != NFilled_tmps.end(); itemnf++ ) {
      if( (*itemnf).find(*itenf) == (*itemnf).end() ) { // only if index is in all scorers
	bFound = false;
	break;
      }
    }
    if( bFound ) {
      theNFilled[*itenf]++;      
    }
  }
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << " GmCompoundScorer::BuildCompoundScores NFilled0 " << NFilled_tmps[0].size() << " NFilledAll " <<NFilled_tmpAll.size() << " theNFilled " << theNFilled.size() << G4endl;
#endif
  
  for( unsigned int ii = 0; ii < sizScorers; ii++ ){
    theSubScorers[ii]->ClearSumV_tmp();
    theSubScorers[ii]->ClearNFilled_tmp();
  } 
}

//--------------------------------------------------------------------
void GmCompoundScorer::SetParameters( const std::vector<G4String>& params)
{
  std::map<G4String,GmVPrimitiveScorer*> theScorers = theScoringMgr->GetScorers();

  if( params.size() == 0 ) return; // GmPSLETD creates parameters itself

  if( params.size() != 1 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmCompoundScorer::SetParameters",
		"There should be one parameters: ARITHMETIC_EXPRESSION_OF_SCORERS",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
  }
  
  G4String scorerName = params[0];
  G4int iSeparator = GmGenUtils::GetNextSeparator( 0, scorerName );
  //  G4cout << " CS: iSeparator " << iSeparator << " " << scorerName << G4endl; //GDEB
  if( iSeparator == G4int(std::string::npos) ) {
    GmVPrimitiveScorer* subScorer = FindOrCreateScorer(scorerName);
    theScoringMgr->AddScorer( scorerName, subScorer );
    subScorer->SetDefaultPrinter(false);
    AddSubScorer( subScorer );
    AddSubExpression( "" );
    subScorer->SetNEventsType( GetNEventsType() );
    
  } else {
    //-----  decompose the regular expression
    unsigned int nLen = scorerName.length();
    std::string expressions;
    std::vector<G4String> words;
    std::vector<G4bool> isSeparator;
    
    G4String word = scorerName.substr(0,iSeparator);
    if( iSeparator != 0 ) {
      // first word has to be a separator
      words.push_back("");
      isSeparator.push_back(1);
      words.push_back( word );
      isSeparator.push_back( GmGenUtils::IsSeparator(word) );
      
      word = scorerName.substr(iSeparator,1);
      words.push_back( word );
      isSeparator.push_back(1);
      G4cout << " FIRST SEPARATOR " << word << " N= " << isSeparator.size() << G4endl;
    } else {
      word = scorerName.substr(0,1);
      words.push_back( word );
      isSeparator.push_back(1);
    }
    //    G4cout << " FIRST WORDS " << words.size() << G4endl; //GDEB
    //    G4cout << " FIRST WORDS " << words[0] << " " << words[1] << " " << words[2] << G4endl;  //GDEB
    
    G4int iSeparatorNew;
    for( ;; ){
      iSeparatorNew = GmGenUtils::GetNextSeparator( iSeparator+1, scorerName );
      //      G4cout << " iSeparatorNew " << iSeparatorNew << G4endl; //GDEB
      if( iSeparatorNew != G4int(std::string::npos) ) {
	if( iSeparatorNew-iSeparator-1 != 0 )  {
	  word = scorerName.substr(iSeparator+1, iSeparatorNew-iSeparator-1);
	  words.push_back( word );
	  isSeparator.push_back( GmGenUtils::IsSeparator(word) );
	}
	word = scorerName.substr(iSeparatorNew, 1);
	words.push_back( word );
	isSeparator.push_back(1);
	
	iSeparator = iSeparatorNew;
      } else {
	break;
      } 
    }
    if( iSeparator != G4int(nLen) - 1 ) {
      word = scorerName.substr(iSeparator+1, nLen);
      words.push_back( word );
      //      G4cout << " LAST WORD " << word << G4endl; //GDEB
    }
    isSeparator.push_back( GmGenUtils::IsSeparator(word) );
    
    std::vector<G4String> subExpressions;
    for( unsigned int ii = 0; ii < words.size(); ii++ ){
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(infoVerb) ) G4cout << "GmCompoundScorer::CreateScorer WORD " << words[ii] << " " << isSeparator[ii] << G4endl;
#endif
      if( isSeparator[ii] ) {
	if( ii && isSeparator[ii-1] ) {
	  // modify last subExpressions
	  subExpressions[subExpressions.size()-1] += words[ii];
	} else {
	  subExpressions.push_back(words[ii]);
	}
      } else {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(infoVerb) ) G4cout << "GmCompoundScorer::CreateScorer  AddSubScorer " << words[ii] << " ii " << ii << G4endl;
#endif
	//----- Check that Scorer exists
	G4String subScorerName = words[ii];
	GmVPrimitiveScorer* subScorer = FindOrCreateScorer(subScorerName);
	std::vector<G4String> wl;
	theScoringMgr->AddScorer( subScorerName, subScorer );
	subScorer->SetDefaultPrinter(false);
	AddSubScorer( subScorer );
	subScorer->SetNEventsType( GetNEventsType() );
      }
    }
      
    for( unsigned int ii2 = 0; ii2 < subExpressions.size(); ii2++ ){
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(infoVerb) ) G4cout << " GmCompoundScorer:SetParameters  AddSubExpression " << subExpressions[ii2] << " ii " << ii2 << G4endl;
#endif
      AddSubExpression( subExpressions[ii2] );
    }
  }

  if( GetMFD() ) PropagateMFDToSubScorers(); //-- If called by createScorer, it does not exists; if called by LET scorers SetParameters, it does
}

//--------------------------------------------------------------------
GmVPrimitiveScorer* GmCompoundScorer::FindOrCreateScorer(G4String scorerName )
{
  GmVPrimitiveScorer* scorer = 0;

  std::map<G4String,GmVPrimitiveScorer*> theScorers = theScoringMgr->GetScorers();

  //----- Check that Scorer exists  
  std::map<G4String,GmVPrimitiveScorer*>::iterator itesco = theScorers.find(scorerName);
  if( itesco != theScorers.end() ) {
    scorer = (*itesco).second;
  } else {
  //--- Create Scorer
    scorer = Reflex::PluginService::Create<GmVPrimitiveScorer*>(scorerName,scorerName);
    if( !scorer ) {
      G4Exception(" GmCompoundScorer::FindOrCreateScorer",
		  "Wrong argument",
		  FatalErrorInArgument,
		  G4String("Scorer class not found " + scorerName).c_str());
    }
  }

  return scorer;
}

//--------------------------------------------------------------------
void GmCompoundScorer::PropagateMFDToSubScorers()
{
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    G4MultiFunctionalDetector* mfd = GetMFD();
    GmVPrimitiveScorer* subScorer = theSubScorers[ii];
    mfd->RegisterPrimitive(subScorer);   
    subScorer->RegisterMFD( mfd );
  }
}

//--------------------------------------------------------------------
void GmCompoundScorer::PropagateFilterToSubScorers()
{
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->SetGmFilter( GetGmFilter() );
  }
}

//--------------------------------------------------------------------
void GmCompoundScorer::PropagateClassifierToSubScorers()
{
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->SetClassifier( GetClassifier() );
  }
}
  

//--------------------------------------------------------------------
void GmCompoundScorer::PropagatePrinterToSubScorers(GmVPSPrinter* prt)
{
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->AddPrinter( prt );
  }
}

//--------------------------------------------------------------------
void GmCompoundScorer::PropagateTrkWeightToSubScorers()
{
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->SetUseTrackWeight( UseTrackWeight() );
  }
}

//--------------------------------------------------------------------
void GmCompoundScorer::PropagateScoreErrorsToSubScorers()
{
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->SetScoreErrors( ScoreErrors() );
  }
}

//--------------------------------------------------------------------
G4int GmCompoundScorer::GetNEvents( G4int index )
{
  switch (theNEventsType) {
    case SNET_ByRun:
      return 1;
    case SNET_ByEvent:
      return GmNumberOfEvent::GetNumberOfEvent();
    case SNET_ByNFilled:
      for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
	G4int nev = theSubScorers[ii]->GetNFilled(index);
	G4cout << "  GmCompoundScorer::GetNEvents " << index << " = " << nev << G4endl; //GDEB
	if( nev == -1 ) {
	  G4Exception("GmVPrimitiveScorer::GetNEvents",
		      "",
		      JustWarning,
		      ("Scorer " + theSubScorers[ii]->GetName() + " : score does not for index " + GmGenUtils::itoa(index)).c_str());
	} else {
	  return nev;
	}
      }
  }
  
  return 1;
}


//--------------------------------------------------------------------
std::map<G4int,size_t> GmCompoundScorer::GetNFilled() const
{
  return theSubScorers[0]->GetNFilled();
}
