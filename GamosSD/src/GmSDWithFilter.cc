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
#include "GmSDWithFilter.hh"
#include "GmHit.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"
#include "GmSDVerbosity.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"

//-------------------------------------------------------------------
GmSDWithFilter::GmSDWithFilter(G4String sdtype): GmSDSimple( sdtype )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << "GmSDWithFilter: SD type ->" << theSDType << " SD name " << SensitiveDetectorName << G4endl;
#endif

  theDetUnitIDBuilder = new GmDetUnitIDBuilderFromAncestors(theSDType);

  std::vector<G4String> filterNames;
  filterNames = GmParameterMgr::GetInstance()->GetVStringValue("SD:Filters:"+theSDType,filterNames);

  for( size_t ii = 0; ii < filterNames.size(); ii++ ) {
    std::vector<G4String> params;
    params.push_back( filterNames[ii] );
    params.push_back( filterNames[ii] );
    GmVFilter* filter2 = GmFilterMgr::GetInstance()->FindOrBuildFilter(params, true );
    theFilters.push_back( filter2 );
  }

}

//-------------------------------------------------------------------
GmSDWithFilter::~GmSDWithFilter()
{
}

G4bool GmSDWithFilter::IsInterestingStep(G4Step*aStep)
{
  std::vector<GmVFilter*>::const_iterator ite;
  for( ite = theFilters.begin(); ite != theFilters.end(); ite++ ) {
    if( !(*ite)->AcceptStep(aStep) ) {
      return false;
    }
  }

  return true;
}
 
//-------------------------------------------------------------------
void GmSDWithFilter::EndOfEvent(G4HCofThisEvent*HCE)
{
#ifndef GAMOS_NO_VERBOSE
  if(theHitsInEvent.size() != 0) if( SDVerb(debugVerb) ) G4cout << "GmSDWithFilter::EndOfEvent SD: " << SensitiveDetectorName << " Nhits= " << theHitsInEvent.size() << G4endl;
#endif
  GmVSD::EndOfEvent( HCE );
}
