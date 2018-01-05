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
#include "GmVFilterOfFilters.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"

//----------------------------------------------------------------
GmVFilterOfFilters::GmVFilterOfFilters(G4String name)
  :GmVFilter(name)
{
}

//----------------------------------------------------------------
GmVFilterOfFilters::~GmVFilterOfFilters()
{ 
}

//----------------------------------------------------------------
void GmVFilterOfFilters::show()
{
  for( unsigned int ii = 0; ii < theFilters.size(); ii++) {
    theFilters[ii]->show();
  }
}

//----------------------------------------------------------------
void GmVFilterOfFilters::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmVFilterOfFilters::SetParameters","There should be at least one parameter: LOGICAL_VOLUME_NAME",FatalErrorInArgument,"There is no parameter");
  }

  GmFilterMgr* filterMgr = GmFilterMgr::GetInstance();
  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    std::vector<G4String> paramsF; 
    paramsF.push_back( params[ii] );
    paramsF.push_back( params[ii] );
    GmVFilter* filter = filterMgr->FindOrBuildFilter(paramsF, true);
    theFilters.push_back(filter);
  }
}
