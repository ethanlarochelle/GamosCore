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
#include "GmInverseFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"

//----------------------------------------------------------------
GmInverseFilter::GmInverseFilter(G4String name)
  :GmVFilter(name)
{
}

//----------------------------------------------------------------
GmInverseFilter::~GmInverseFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmInverseFilter::AcceptTrack(const G4Track* aTrack)
{
  G4bool bAccept = !(theFilters->AcceptTrack(aTrack));
  
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << theName << " GmInverseFilter::AcceptTrack " << bAccept << G4endl;
#endif
  
  return bAccept;
}


//----------------------------------------------------------------
G4bool GmInverseFilter::AcceptStep(const G4Step* aStep)
{
  G4bool bAccept = !(theFilters->AcceptStep(aStep));
  
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << theName << " GmInverseFilter::AcceptStep " << bAccept << G4endl;
#endif

  return bAccept;
}

//----------------------------------------------------------------
void GmInverseFilter::show()
{
  G4cout << "GmInverseFilter" << G4endl;
  theFilters->show();
}

//----------------------------------------------------------------
void GmInverseFilter::SetParameters( std::vector<G4String>& params)
{
  if( params.size() != 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmVFilterOfFilters::SetParameters","There should be at one parameter: FILTER TO INVERSE",FatalErrorInArgument,G4String("Parameters are:"+parastr).c_str());
  }

  GmFilterMgr* filterMgr = GmFilterMgr::GetInstance();
  std::vector<G4String> paramsF; 
  paramsF.push_back( params[0] );
  paramsF.push_back( params[0] );
  theFilters = filterMgr->FindOrBuildFilter(paramsF, true);
}

