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
#include "GmORFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//----------------------------------------------------------------
GmORFilter::GmORFilter(G4String name)
  :GmVFilterOfFilters(name)
{
}

//----------------------------------------------------------------
GmORFilter::~GmORFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmORFilter::AcceptTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "GmORFilter::AcceptTrack number of filters " << theFilters.size() << G4endl;
#endif
  for( unsigned int ii = 0; ii < theFilters.size(); ii++) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "GmORFilter::AcceptTrack filter " << theFilters[ii] << G4endl;
#endif
    if( theFilters[ii]->AcceptTrack(aTrack) ) return TRUE;
  }
  return FALSE;
}


//----------------------------------------------------------------
G4bool GmORFilter::AcceptStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "GmORFilter::AcceptStep number of filters " << theFilters.size() << G4endl;
#endif
  for( unsigned int ii = 0; ii < theFilters.size(); ii++) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "GmORFilter::AcceptStep filter " << theFilters[ii] << G4endl;
#endif
    if( theFilters[ii]->AcceptStep(aStep) ) return TRUE;
  }
  return FALSE;
}
