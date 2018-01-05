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
#include "GmInMassRegionFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//-------------------------------------------------------------
GmInMassRegionFilter::GmInMassRegionFilter(G4String name)
  :GmVFilter(name)
{
  theRegions.clear();
}

//-------------------------------------------------------------
GmInMassRegionFilter::~GmInMassRegionFilter()
{ 
  theRegions.clear();
}

//-------------------------------------------------------------
G4bool GmInMassRegionFilter::AcceptTrack(const G4Track* aTrack )
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmInMassRegionFilter::AcceptTrack " << G4endl;
#endif

  G4VPhysicalVolume* physvol = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndSetup( aTrack->GetPosition(), (G4ThreeVector*)0, false, false ); 
  if( physvol ) {
    G4Region* region = physvol->GetLogicalVolume()->GetRegion();
    
    if( theRegions.find(region) != theRegions.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassRegionFilter::AcceptTrack  return 1 " << region->GetName() << G4endl;
#endif
      return TRUE;
    }
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassRegionFilter::AcceptTrack  return 0 " << region->GetName() << G4endl;
#endif
    return FALSE;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassRegionFilter::AcceptTrack  return 0 , no vol " << G4endl;
#endif
    return FALSE;
  }
}

//-------------------------------------------------------------
G4bool GmInMassRegionFilter::AcceptStep(const G4Step* aStep)
{
  G4Region* region = aStep->GetTrack()->GetVolume()->GetLogicalVolume()->GetRegion();
  if( theRegions.find(region) != theRegions.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassRegionFilter::AcceptStep  return 1 " << region->GetName() << G4endl;
#endif
    return TRUE;
  } 
  
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmInMassRegionFilter::AcceptStep  return 0 " << region->GetName() << G4endl;
#endif
  
  return FALSE;
}


//-------------------------------------------------------------
void GmInMassRegionFilter::show()
{
  G4cout << "----G4RegionFilter volume list------"<<G4endl; 
  std::set<G4Region*>::const_iterator iter;
  for ( iter = theRegions.begin(); iter != theRegions.end(); iter++ ){
    G4cout << (*iter)->GetName() << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//-------------------------------------------------------------
void GmInMassRegionFilter::SetParameters( std::vector<G4String>& params)
{
  GmTouchableFilterRegion::SetParameters(params);
}
