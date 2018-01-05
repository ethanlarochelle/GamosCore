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
#include "G4StackManager.hh"
#include "GmPDSInvertGeantinoStackUA.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GmPDSUtils.hh"

#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"

#include "G4Geantino.hh"
#include "G4Track.hh"


GmPDSInvertGeantinoStackUA* GmPDSInvertGeantinoStackUA::theInstance = 0;

//---------------------------------------------------------------------------
GmPDSInvertGeantinoStackUA* GmPDSInvertGeantinoStackUA::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmPDSInvertGeantinoStackUA;
  } 

  return theInstance;
}


//---------------------------------------------------------------------------
GmPDSInvertGeantinoStackUA::GmPDSInvertGeantinoStackUA()
{

}

//---------------------------------------------------------------------------
G4ClassificationOfNewTrack GmPDSInvertGeantinoStackUA::ClassifyNewTrack(const G4Track* )
{
  return fWaiting; // so that NewStage will invert the order

  /*  G4TrackStack * waitingStack = stackManager->waitingStack;
  G4cout << " CNT stackManager " << stackManager << " waitingStack " << waitingStack  << " ntracks " << waitingStack->GetNTrack() << G4endl;

  if( aTrack->GetDefinition() == G4Geantino::Geantino() ) {
    return fWaiting;
  } else {
    return fUrgent;
  }
  */
}


//---------------------------------------------------------------------------
void GmPDSInvertGeantinoStackUA::NewStage()
{
  G4TrackStack * urgentStack = stackManager->urgentStack;

  G4StackedTrack aStackedTrack;

  std::vector<G4StackedTrack> invertedStack;
  if( ScoringVerb(debugVerb) ) G4cout << " GmPDSInvertGeantinoStackUA::NewStage ntracks urgent " << urgentStack->GetNTrack() << G4endl;
  while( urgentStack->GetNTrack() > 0 ) {
    //  while( (aStackedTrack=urgentStack->PopFromStack()) != 0 ) {
    if( ScoringVerb(debugVerb) && aStackedTrack.GetTrack() ) G4cout << "GmPDSInvertGeantinoStackUA::NewStage()  track popped " << " ID " << aStackedTrack.GetTrack()->GetTrackID() << " pos " << aStackedTrack.GetTrack()->GetPosition() << G4endl;
    invertedStack.push_back(aStackedTrack);
  }

  std::vector<G4StackedTrack>::const_iterator ite;
  for( ite = invertedStack.begin(); ite != invertedStack.end(); ite++ ){
    urgentStack->PushToStack( *ite );
  }

}

//---------------------------------------------------------------------------
void GmPDSInvertGeantinoStackUA::ChangeGeantinoWeight( G4double weiFactor, G4int parentID, G4int detID )
{
  G4TrackStack * urgentStack = stackManager->urgentStack;

  G4StackedTrack  aStackedTrack;

  std::vector<G4StackedTrack> tmpStack;
  while( urgentStack->GetNTrack() > 0 ) {
    //  while( (aStackedTrack=urgentStack->PopFromStack()) != 0 ) {
    if( ScoringVerb(debugVerb) && aStackedTrack.GetTrack() ) G4cout << "GmPDSInvertGeantinoStackUA::ChangeGeantinoWeight()  track " << " ID " << aStackedTrack.GetTrack()->GetTrackID()  << "  " << aStackedTrack.GetTrack()->GetDefinition()->GetParticleName()<< " pos " << aStackedTrack.GetTrack()->GetPosition() << G4endl;
    tmpStack.push_back(aStackedTrack);
    G4Track* aTrack = aStackedTrack.GetTrack();
    if( aTrack->GetDefinition() == G4Geantino::Geantino() ) {
      G4VUserTrackInformation* trki = aTrack->GetUserInformation();
      GmTrackInfo* gmtrki = dynamic_cast<GmTrackInfo*>( trki );
      if( gmtrki->GetIntValue("DetectorID") == detID && aTrack->GetParentID() == parentID ) {
	if( ScoringVerb(debugVerb) ) G4cout << "GmPDSInvertGeantinoStackUA::ChangeGeantinoWeight() track "<< aTrack->GetTrackID() << " weight changed to " << GetWeight(aTrack)*weiFactor << G4endl;
	SetWeight( aTrack, GetWeight(aTrack)*weiFactor );
      }
    } else if ( aTrack->GetParentID() == parentID ) { 
      if( ScoringVerb(debugVerb) ) G4cout << "GmPDSInvertGeantinoStackUA::ChangeGeantinoWeight() track "<< aTrack->GetTrackID() << " weight changed to " << aTrack->GetWeight()*weiFactor << G4endl;
      aTrack->SetWeight( aTrack->GetWeight()*weiFactor );
    }
  }

  for( unsigned int ii = 0; ii < tmpStack.size(); ii++ ){
    urgentStack->PushToStack( tmpStack[ii] );
  }

}
