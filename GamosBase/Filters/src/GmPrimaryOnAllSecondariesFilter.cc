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
#include "GmPrimaryOnAllSecondariesFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Track.hh"
#include "G4EventManager.hh"

//----------------------------------------------------------------
GmPrimaryOnAllSecondariesFilter::GmPrimaryOnAllSecondariesFilter(G4String name)
  :GmVANDFilter(name)
{
}

//----------------------------------------------------------------
GmPrimaryOnAllSecondariesFilter::~GmPrimaryOnAllSecondariesFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmPrimaryOnAllSecondariesFilter::AcceptTrack(const G4Track*)
{

  G4SteppingManager* theSteppingManager = G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager();
  const G4TrackVector* secos = theSteppingManager->GetSecondary();

  return AcceptAllSecoTracks( *secos, "AcceptTrack" );

}


//----------------------------------------------------------------
G4bool GmPrimaryOnAllSecondariesFilter::AcceptStep(const G4Step* )
{
  G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
  
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "GmPrimaryOnAllSecondariesFilter::AcceptStep number of secondaries " << secos.size() << " number of filters " << theFilters.size() << G4endl;
#endif
  return AcceptAllSecoTracks( secos, "AcceptStep" );

}

//----------------------------------------------------------------
G4bool GmPrimaryOnAllSecondariesFilter::AcceptAllSecoTracks( const G4TrackVector& secos, const G4String& name )
{

  G4bool bAcceptAll = FALSE;
  for( unsigned int ii = 0; ii < secos.size(); ii++) {
    G4bool bAccept = AcceptTrackAND(secos[ii]);
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "GmPrimaryOnAllSecondariesFilter::" << name << " seco accepted " << ii << " return 1 " << G4endl;
#endif
    if( bAccept ) {
      bAcceptAll = TRUE; // one secondary passed all the filters
    } else {
      bAcceptAll = FALSE; // one secondary did not pass all the filters
      break;
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "GmPrimaryOnAllSecondariesFilter::" << name << " return " << bAcceptAll << G4endl;
#endif

  return bAcceptAll;

}
