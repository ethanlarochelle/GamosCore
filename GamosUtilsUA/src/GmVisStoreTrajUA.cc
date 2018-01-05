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
#include "GmVisStoreTrajUA.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4Event.hh"

//---------------------------------------------------------------------
GmVisStoreTrajUA::GmVisStoreTrajUA()
{
 std::vector<G4double> evlis;
 evlis = GmParameterMgr::GetInstance()->GetVNumericValue("GmVisStoreTrajUA:EventList",evlis);

 theNEvents = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":NEvents",5));

 G4cout << " GmVisStoreTrajUA List of Events: " << evlis.size() <<  G4endl;
 for( unsigned int ii = 0; ii < evlis.size(); ii++ ){
   theEventList.insert(G4int(evlis[ii]));
   G4cout << " GmVisStoreTrajUA EVENT  " << evlis[ii] << G4endl;
 }
 G4cout << " GmVisStoreTrajUA N Events " << theNEvents << " " << theEventList.size() << G4endl;
 
}

//---------------------------------------------------------------------
void GmVisStoreTrajUA::BeginOfEventAction( const G4Event* evt )
{
  G4int evtID = evt->GetEventID();
  //  if( std::find(theEventList.begin(),theEventList.end(), evtID-theNEvents  ) != theEventList.end() ) {
  if( theEventList.find(evtID+theNEvents) != theEventList.end() ) {
    G4cout << " GmVisStoreTrajUA SetStoreTrajectory 1 in event " << evtID << G4endl;
    fpTrackingManager->SetStoreTrajectory(1);
  } else {
    /*    G4cout << " GmVisStoreTrajUA SetStoreTrajectory not set  " << evtID << " " <<  theEventList.size() << G4endl;
    std::set<G4int>::const_iterator ite;
    for( ite = theEventList.begin(); ite != theEventList.end(); ite++ ){
      G4cout << " GmVisStoreTrajUA EVENT " << *ite << G4endl;
      } */
  } 

  if( theEventList.find(evtID-theNEvents) != theEventList.end() ) {
    G4cout << " GmVisStoreTrajUA SetStoreTrajectory 0 in event " << evtID << G4endl;
    fpTrackingManager->SetStoreTrajectory(0);
  }

}
