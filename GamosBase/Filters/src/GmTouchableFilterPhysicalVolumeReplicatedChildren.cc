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
#include "GmTouchableFilterPhysicalVolumeReplicatedChildren.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//---------------------------------------------------------------------
GmTouchableFilterPhysicalVolumeReplicatedChildren::GmTouchableFilterPhysicalVolumeReplicatedChildren()
{
  theVPhysicalVolumes.clear();
}

//---------------------------------------------------------------------
GmTouchableFilterPhysicalVolumeReplicatedChildren::~GmTouchableFilterPhysicalVolumeReplicatedChildren()
{ 
  theVPhysicalVolumes.clear();
}

//---------------------------------------------------------------------
G4bool GmTouchableFilterPhysicalVolumeReplicatedChildren::AcceptTouchable(const G4VTouchable* touch)
{
  if( !touch->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmTouchableFilterPhysicalVolumeReplicatedChildren::AcceptTouchable return 0, no touch->GetVolume() " << G4endl;
#endif
    return FALSE; // it should have detected before, but fWorldBoundary is not set
  }
  G4int siz = touch->GetHistoryDepth();
  for( G4int ii = 0; ii < siz; ii++ ){
    G4VPhysicalVolume* pv = touch->GetVolume(ii);
    G4String pvName = pv->GetName() + ":" + GmGenUtils::ftoa(pv->GetCopyNo());
    if( theVPhysicalVolumes.find(pvName) != theVPhysicalVolumes.end() ) {
#ifndef GAMOS_NO_VERBOSE
      if( BaseVerb(debugVerb) ) G4cout << " GmTouchableFilterPhysicalVolumeReplicatedChildren::AcceptTouchable TRUE return 1 " << pvName << G4endl;
#endif
      return TRUE;
    }
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmTouchableFilterPhysicalVolumeReplicatedChildren::AcceptTouchable return 0 " << touch->GetVolume()->GetName() << G4endl;
#endif

  return FALSE;
}

//---------------------------------------------------------------------
void GmTouchableFilterPhysicalVolumeReplicatedChildren::show()
{
  G4cout << "----GmTouchableFilterPhysicalVolumeReplicatedChildren volume list------"<<G4endl; 
  std::set<G4String>::const_iterator iter;
  for ( iter = theVPhysicalVolumes.begin(); iter != theVPhysicalVolumes.end(); iter++ ){
    G4cout << "GmTouchableFilterPhysicalVolumeReplicatedChildren volume " << (*iter) << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//---------------------------------------------------------------------
void GmTouchableFilterPhysicalVolumeReplicatedChildren::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmTouchableFilterPhysicalVolumeReplicatedChildren::SetParameters","There should be at least one parameter: LOGICAL_VOLUME_NAME",FatalErrorInArgument,"There is no parameter");
  }

  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    std::vector<G4String> volumes = GmGeometryUtils::GetInstance()->GetPhysicalVolumeNames( params[ii] );
    for(unsigned int jj = 0; jj < volumes.size(); jj++ ){
      theVPhysicalVolumes.insert( volumes[jj] );
#ifndef GAMOS_NO_VERBOSE
      if( BaseVerb(debugVerb) ) G4cout << "GmTouchableFilterPhysicalVolumeReplicatedChildren::SetParameters add parameter " << volumes[jj] << G4endl;
#endif
     }
  }
}
