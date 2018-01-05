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
#include "GmTouchableFilterLogicalVolumeChildren.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//---------------------------------------------------------------------
GmTouchableFilterLogicalVolumeChildren::GmTouchableFilterLogicalVolumeChildren()
{
  theLogicalVolumes.clear();
}

//---------------------------------------------------------------------
GmTouchableFilterLogicalVolumeChildren::~GmTouchableFilterLogicalVolumeChildren()
{ 
  theLogicalVolumes.clear();
}

//---------------------------------------------------------------------
G4bool GmTouchableFilterLogicalVolumeChildren::AcceptTouchable(const G4VTouchable* touch)
{
  if( !touch->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmTouchableFilterLogicalVolumeChildren::AcceptTouchable return FALSE, no touch->GetVolume() " << G4endl;
#endif
    return FALSE; // it should have detected before, but fWorldBoundary is not set
  }
  G4int siz = touch->GetHistoryDepth();
  for(G4int ii = 0; ii < siz; ii++ ){
    if( theLogicalVolumes.find(touch->GetVolume(ii)->GetLogicalVolume()) != theLogicalVolumes.end() ) {
#ifndef GAMOS_NO_VERBOSE
      if( BaseVerb(debugVerb) ) G4cout << " GmTouchableFilterLogicalVolumeChildren::AcceptTouchable return TRUE " << touch->GetVolume(ii)->GetName() << G4endl;
#endif
      return TRUE;
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmTouchableFilterLogicalVolumeChildren::AcceptTouchable return FALSE " << touch->GetVolume()->GetName() << G4endl;
#endif

  return FALSE;
}

//---------------------------------------------------------------------
void GmTouchableFilterLogicalVolumeChildren::show()
{
  G4cout << "----GmTouchableFilterLogicalVolumeChildren volume list------"<<G4endl; 
  std::set<G4LogicalVolume*>::const_iterator iter;
  for ( iter = theLogicalVolumes.begin(); iter != theLogicalVolumes.end(); iter++ ){
    G4cout << "GmTouchableFilterLogicalVolumeChildren volume " << (*iter)->GetName() << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//---------------------------------------------------------------------
void GmTouchableFilterLogicalVolumeChildren::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmTouchableFilterLogicalVolumeChildren::SetParameters","There should be at least one parameter: LOGICAL_VOLUME_NAME",FatalErrorInArgument,"There is no parameter");
  }

  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    std::vector<G4LogicalVolume*> volumes = GmGeometryUtils::GetInstance()->GetLogicalVolumes( params[ii] );
    for(unsigned int jj = 0; jj < volumes.size(); jj++ ){
      theLogicalVolumes.insert( volumes[jj] );
#ifndef GAMOS_NO_VERBOSE
      if( BaseVerb(debugVerb) ) G4cout << "GmTouchableFilterLogicalVolumeChildren::SetParameters add parameter " << volumes[jj]->GetName() << G4endl;
#endif
     }
  }
}
