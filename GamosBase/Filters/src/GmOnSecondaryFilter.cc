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
#include "GmOnSecondaryFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "G4EventManager.hh"
#include "G4SteppingManager.hh"

//----------------------------------------------------------------
GmOnSecondaryFilter::GmOnSecondaryFilter(G4String name)
  :GmVANDFilter(name)
{
}

//----------------------------------------------------------------
GmOnSecondaryFilter::~GmOnSecondaryFilter()
{ 
}


//----------------------------------------------------------------
G4bool GmOnSecondaryFilter::AcceptStep(const G4Step* )
{
  G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
  
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "GmOnSecondaryFilter::AcceptStep  number of filters " << theFilters.size() << G4endl;
#endif
  G4TrackVector::const_iterator ite;
  for( ite = secos.begin(); ite != secos.end(); ite++ ) {
    // Check if one of the secondary pass filters 
    if( AcceptTrackAND(*ite) ) {
#ifndef GAMOS_NO_VERBOSE
      if( BaseVerb(debugVerb) ) G4cout << "GmOnSecondaryFilter::AcceptStep return TRUE " << G4endl;
#endif
      return TRUE; // one secondary passed all the filters
    }
  }
    
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "GmOnSecondaryFilter::AcceptStep  return FALSE " << G4endl;
#endif
  
  return FALSE;

}

//----------------------------------------------------------------
G4bool GmOnSecondaryFilter::AcceptTrack(const G4Track* secoTrack)
{
  // assumes the track is a secondary track (user should check it)
   
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "GmOnSecondaryFilter::AcceptTrack  number of filters " << theFilters.size() << G4endl;
#endif
  if( AcceptTrackAND(secoTrack) ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "GmOnSecondaryFilter::AcceptTrack return TRUE " << G4endl;
#endif
    return TRUE; // one secondary passed all the filters
  }
    
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "GmOnSecondaryFilter::AcceptTrack  return FALSE " << G4endl;
#endif
  
  return FALSE;
  
}
