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
#include "GmFutureFilter.hh"
#include "GmStepMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//------------------------------------------------------------------------
GmFutureFilter::GmFutureFilter(G4String name)
: GmVFilter(name)
{
  bFutureFilter = TRUE;
  bFilterWithChildren = FALSE;
  GmStepMgr::GetInstance(); // to create GmFutureFilterUA
}

//------------------------------------------------------------------------
GmFutureFilter::~GmFutureFilter()
{;}

//------------------------------------------------------------------------
void GmFutureFilter::SetParameters( std::vector<G4String>& params )
{
  if( params.size() != 2 ) {
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmFutureFilter::SetParameters",
		"There should be two parameters: past_filter future_filter",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }

  GmFilterMgr* filterMgr = GmFilterMgr::GetInstance();
  std::vector<G4String> filterParams;
  filterParams.push_back( params[0] );
  filterParams.push_back( params[0] );
  thePastFilter = filterMgr->FindOrBuildFilter( filterParams, TRUE );

  filterParams.clear();
  filterParams.push_back( params[1] );
  filterParams.push_back( params[1] );
  theFutureFilter = filterMgr->FindOrBuildFilter( filterParams, TRUE );

}

//------------------------------------------------------------------------
G4bool GmFutureFilter::AcceptFutureStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) {
    G4cout << "GmFutureFilter::AcceptFutureStep " << G4endl;
  }
#endif
  return theFutureFilter->AcceptStep( aStep );
}

//------------------------------------------------------------------------
G4bool GmFutureFilter::AcceptPastStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) {
    G4cout << "GmFutureFilter::AcceptPastStep " << G4endl;
  }
#endif
  return thePastFilter->AcceptStep( aStep );
}

