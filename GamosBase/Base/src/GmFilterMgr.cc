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
#include "GmFilterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "Reflex/PluginService.h"

GmFilterMgr* GmFilterMgr::theInstance = 0;

//----------------------------------------------------------------------
GmFilterMgr::GmFilterMgr()
{
}

//----------------------------------------------------------------------
GmFilterMgr* GmFilterMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmFilterMgr;
  }

  return theInstance;
}

//----------------------------------------------------------------------
GmFilterMgr::~GmFilterMgr()
{
}

//----------------------------------------------------------------------
GmVFilter* GmFilterMgr::CreateFilter( std::vector<G4String> params, G4bool bExists )
{
 Reflex::PluginService::SetDebug(2);
 GmVFilter* filter = Reflex::PluginService::Create<GmVFilter*>(params[1],params[0]);

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " CreateFilter NAME=" << params[0] << " CLASS=" << params[1] << " = " << filter << G4endl;
#endif
  if( filter != 0 ) {
    filter->SetClass(params[1]);
    params.erase(params.begin()); 
    params.erase(params.begin());  
    
    filter->SetParameters( params );
    AddFilter( filter );

  } else if( bExists ) {
    G4Exception(" GmFilterMgr::CreateFilter() filter type not found ",params[1],FatalException,"Please check with 'SealPluginDump ' and '/gamos/filter' commands");
  }

  return filter;
}

//----------------------------------------------------------------------
void GmFilterMgr::AddFilter( GmVFilter* filter )
{
  std::map<G4String,GmVFilter*>::const_iterator ite = theFilters.find( filter->GetName() );
  if( ite == theFilters.end() ) {
    theFilters[filter->GetName()] = filter;
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmFilterMgr::AddFilter " << filter->GetName() << G4endl;
#endif
  } else {
    G4Exception("GmFilterMgr::Addfilter"," Adding two filters with the same name, review your '/gamos/filter' commands",FatalErrorInArgument,filter->GetName());
  }

}


//----------------------------------------------------------------------
GmVFilter* GmFilterMgr::FindOrBuildFilter(std::vector<G4String> params, G4bool bExists )
{
  GmVFilter* filter = GetFilterFromList( params[0] );
  if( filter != 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmFilterMgr::FindOrBuildFilter filter found " << params[0] << G4endl;
#endif
    return filter;
  } else {
    return CreateFilter( params, bExists );
  }

}

//----------------------------------------------------------------------
GmVFilter* GmFilterMgr::GetFilterFromList(const G4String& filterName ) const
{
  std::map<G4String,GmVFilter*>::const_iterator ite = theFilters.find( filterName );
  if( ite != theFilters.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmFilterMgr::GetFilterFromList found " << filterName << G4endl;
#endif
    return (*ite).second;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmFilterMgr::GetFilterFromList NOT found " << filterName << G4endl;
#endif
    return 0;
  }
}


//----------------------------------------------------------------------
void GmFilterMgr::DeleteFilters()
{
  std::map<G4String,GmVFilter*>::iterator ite;

  for( ite = theFilters.begin(); ite != theFilters.end(); ite++ ){
    delete (*ite).second;
  }

  theFilters.clear();

}
