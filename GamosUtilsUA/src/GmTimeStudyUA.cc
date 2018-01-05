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
#include "GmTimeStudyUA.hh"
#include "GmTimeStudyMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include <math.h>

//------------------------------------------------------------------
GmTimeStudyUA::GmTimeStudyUA()
{
  GmTimeStudyMgr::GetInstance()->AddTimeStudy( this );
}

//------------------------------------------------------------------
GmTimeStudyUA::~GmTimeStudyUA()
{
}

//------------------------------------------------------------------
void GmTimeStudyUA::BeginOfRunAction( const G4Run* )
{
  theName = "GmTimeStudyUA";
  if( theClassifier ) {
    theName += "_" + theClassifier->GetName();
  }

  GmTimeStudyMgr::GetInstance()->InitManagers();

}

//------------------------------------------------------------------
void GmTimeStudyUA::UserSteppingAction( const G4Step* aStep )
{
  StopTimer(aStep);
}


//------------------------------------------------------------------
void GmTimeStudyUA::StartTimer( const G4Step* aStep )
{
  //----- Start new timer
  unsigned int timerID = 0; 
  G4Step* aStepnc = const_cast< G4Step*>(aStep);

  timerID += theClassifier->GetIndexFromStep(aStepnc);
    //    G4cout << theName << " timerID " << timerID << " " << pow(theNShift,ii) << " " << theClassifiers[ii]->GetIndexFromStep(aStepnc) << G4endl;

  mut::const_iterator ite = fTimers.find(timerID);
  if( ite == fTimers.end() ) {
    G4SliceTimer* stimer = new G4SliceTimer;
    stimer->Clear();
    fTimers[timerID] = stimer;
  }
  fTimers[timerID]->Start();

  theCurrentTimerID = timerID;

  /*
  G4String cname = theClassifier1->GetClassificationName( timerID.first);
  if( theClassifier2 ) cname += "/" + theClassifier2->GetClassificationName( timerID.second);
  G4cout << cname << "  User=" <<  fTimers[timerID]->GetUserElapsed() << G4endl;
  */

}

//------------------------------------------------------------------
void GmTimeStudyUA::StopTimer( const G4Step* ){
  //----- Stop previous timer
  if( fTimers.find( theCurrentTimerID ) == fTimers.end() ) {
    G4Exception("GmTimeStudyUA::StopTimer",
		"Erro",
		FatalException,
		"Cannot work if GEANT4 has been compiled with variable 'G4_NO_VERBOSE' ");
  }
  fTimers[theCurrentTimerID]->Stop();

  //  if( theCurrentTimerID )  G4cout << " GmTimeStudyUA::UserSteppingAction old " << theCurrentTimerID << " = " << fTimers[theCurrentTimerID]->GetUserElapsed() << G4endl;

} 


//------------------------------------------------------------------
void GmTimeStudyUA::EndOfRunAction( const G4Run* )
{
  Report();

  mut::const_iterator ite;
  for(ite = fTimers.begin(); ite != fTimers.end(); ite++) {
    delete (*ite).second;
  }
  fTimers.clear();
}

//------------------------------------------------------------------
void GmTimeStudyUA::Report()
{
  G4cout << G4endl;
  G4cout << "%%%%% TIMING RESULTS for timer " << theName << G4endl;
  std::map<G4String,G4SliceTimer*> fTimersByName;
  mut::const_iterator ite;
  for(ite = fTimers.begin(); ite != fTimers.end(); ite++) {
    G4SliceTimer* stimer = (*ite).second;
    G4String cname = "";
    unsigned int indexID = (*ite).first;
    cname += theClassifier->GetIndexName( indexID );
    fTimersByName[cname] = stimer;
  }

  std::map<G4String,G4SliceTimer*>::const_iterator iten;
  for( iten = fTimersByName.begin(); iten != fTimersByName.end(); iten++ ){
    G4SliceTimer* stimer = (*iten).second;
    G4cout << (*iten).first << ": ";      
    G4cout << "  User=" << stimer->GetUserElapsed()
     << "  Real=" << stimer->GetRealElapsed()
     << "  Sys=" << stimer->GetSystemElapsed() << G4endl;
  }
  G4cout << G4endl;
}
