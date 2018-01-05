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
#include "GmCreatorProcessFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

//--------------------------------------------------------------------
GmCreatorProcessFilter::GmCreatorProcessFilter(G4String name)
  :GmVFilter(name)
{
  theProcesses.clear();
}

//--------------------------------------------------------------------
GmCreatorProcessFilter::~GmCreatorProcessFilter()
{ 
  theProcesses.clear();
}

//--------------------------------------------------------------------
G4bool GmCreatorProcessFilter::AcceptTrack(const G4Track* aTrack)
{
  G4VProcess* proc = const_cast<G4VProcess*>(aTrack->GetCreatorProcess());
  if( theProcesses.find(proc) != theProcesses.end() ) {
    return TRUE;
  }
  return FALSE;
}

//--------------------------------------------------------------------
void GmCreatorProcessFilter::show()
{
  G4cout << "----G4VPhysicalVolumeFilter volume list------"<<G4endl;
  std::set<G4VProcess*>::const_iterator iter;
  for ( iter = theProcesses.begin(); iter != theProcesses.end(); iter++ ){
    G4cout << (*iter)->GetProcessName() << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//--------------------------------------------------------------------
void GmCreatorProcessFilter::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4Exception("GmCreatorProcessFilter::SetParameters","There should be at least one parameter: PROCESS_NAME",FatalErrorInArgument,"There is no parameter");
  }

  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    std::vector<G4VProcess*> procs = GmGetParticleMgr::GetInstance()->GetG4ProcessList( params[ii] );
    for(unsigned int jj = 0; jj < procs.size(); jj++ ){
      theProcesses.insert( procs[jj] );
    }
  }
}
