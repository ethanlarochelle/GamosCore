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
#include "GmNumberOfEvent.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

G4double GmNumberOfEvent::theNEvent = -1;
G4double GmNumberOfEvent::theNEventFactor = 1.;

//----------------------------------------------------------------------
G4double GmNumberOfEvent::GetNumberOfEvent()
{
  if( theNEvent == -1 ) {
    //    G4cout << " GmNumberOfEvent::GetNumberOfEvent() from run " <<  G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEvent() << G4endl;
    return G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEvent();
  } else {
    //     G4cout << " GmNumberOfEvent::GetNumberOfEvent() " << theNEvent * theNEventFactor << theNEvent << " * " << theNEventFactor << G4endl;
    return theNEvent * theNEventFactor;
  }

}


//----------------------------------------------------------------------
void GmNumberOfEvent::SetNumberOfEvent( G4double nev )
{
  theNEvent = nev;
}


//----------------------------------------------------------------------
void GmNumberOfEvent::SetNumberOfEventFactor(G4double nevfac)
{
  theNEventFactor = nevfac;
}

//----------------------------------------------------------------------
G4double GmNumberOfEvent::GetNumberOfEventFactor()
{
  return theNEventFactor; 
}

//----------------------------------------------------------------------
void GmNumberOfEvent::AddEvent()
{
  theNEvent++;
}
