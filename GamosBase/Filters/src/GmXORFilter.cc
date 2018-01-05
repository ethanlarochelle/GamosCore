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
#include "GmXORFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//----------------------------------------------------------------
GmXORFilter::GmXORFilter(G4String name)
  :GmVFilterOfFilters(name)
{
}

//----------------------------------------------------------------
GmXORFilter::~GmXORFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmXORFilter::AcceptTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "GmXORFilter::AcceptTrack number of filters " << theFilters.size() << G4endl;
#endif
  G4bool bAccept = FALSE;
  for( unsigned int ii = 0; ii < theFilters.size(); ii++) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "GmXORFilter::AcceptTrack filter " << theFilters[ii] << G4endl;
#endif
    if( theFilters[ii]->AcceptTrack(aTrack) ) {
      if( !bAccept ) {
	bAccept = TRUE;
      } else {
	bAccept = FALSE;
	break;
      }
    }
  }

  return bAccept;
}


//----------------------------------------------------------------
G4bool GmXORFilter::AcceptStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "GmXORFilter::AcceptStep number of filters " << theFilters.size() << G4endl;
#endif
  G4bool bAccept = FALSE;  
  for( unsigned int ii = 0; ii < theFilters.size(); ii++) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << "GmXORFilter::AcceptStep filter " << theFilters[ii] << G4endl;
#endif
    if( theFilters[ii]->AcceptStep(aStep) ) {
      if( !bAccept ) {
	bAccept = TRUE;
      } else {
	bAccept = FALSE;
	break;
      }
    }
  }

  return bAccept;

}
