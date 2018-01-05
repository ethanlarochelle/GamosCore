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
#include "GmInMassPhysicalVolumeReplicatedFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//-----------------------------------------------------------------
GmInMassPhysicalVolumeReplicatedFilter::GmInMassPhysicalVolumeReplicatedFilter(G4String name)
  :GmVFilter(name)
{
  theVPhysicalVolumes.clear();
}

//-----------------------------------------------------------------
GmInMassPhysicalVolumeReplicatedFilter::~GmInMassPhysicalVolumeReplicatedFilter()
{ 
  theVPhysicalVolumes.clear();
}

//-----------------------------------------------------------------
G4bool GmInMassPhysicalVolumeReplicatedFilter::AcceptTrack(const G4Track* aTrack )
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeReplicatedFilter::AcceptTrack " << G4endl;
#endif

  G4VPhysicalVolume* physvol = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndSetup( aTrack->GetPosition(), (G4ThreeVector*)0, false, false ); 
  if( physvol ) {
    G4String pvName = physvol->GetName() + ":" + GmGenUtils::ftoa(physvol->GetCopyNo());
    if( theVPhysicalVolumes.find(pvName) != theVPhysicalVolumes.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeReplicatedFilter::AcceptTrack  return 1 " << physvol->GetName() << G4endl;
#endif
      return TRUE;
    }
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeReplicatedFilter::AcceptTrack  return 0 " << physvol->GetName() << G4endl;
#endif
    return FALSE;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeReplicatedFilter::AcceptTrack  return 0 , no vol " << G4endl;
#endif
    return FALSE;
  }
}

//-----------------------------------------------------------------
G4bool GmInMassPhysicalVolumeReplicatedFilter::AcceptStep(const G4Step* aStep)
{
  G4VPhysicalVolume* physvol = aStep->GetTrack()->GetVolume();
  G4String pvName = physvol->GetName() + ":" + GmGenUtils::ftoa(physvol->GetCopyNo());
  if( theVPhysicalVolumes.find(pvName) != theVPhysicalVolumes.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeReplicatedFilter::AcceptStep  return 1 " << physvol->GetName() << G4endl;
#endif
    return TRUE;
  }
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeReplicatedFilter::AcceptStep  return 0 " << physvol->GetName() << G4endl;
#endif
  return FALSE;
}

//-----------------------------------------------------------------
void GmInMassPhysicalVolumeReplicatedFilter::show()
{
  G4cout << "----G4VPhysicalVolumeFilter volume list------"<<G4endl; 
  std::set<G4String>::const_iterator iter;
  for ( iter = theVPhysicalVolumes.begin(); iter != theVPhysicalVolumes.end(); iter++ ){
    G4cout << (*iter) << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//-------------------------------------------------------------
void GmInMassPhysicalVolumeReplicatedFilter::SetParameters( std::vector<G4String>& params)
{
  GmTouchableFilterPhysicalVolumeReplicated::SetParameters(params);
}
