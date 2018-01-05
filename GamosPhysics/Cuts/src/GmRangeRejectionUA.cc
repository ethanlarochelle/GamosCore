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
#include "GmRangeRejectionUA.hh"
#include "G4Run.hh"
#include "G4Track.hh"
#include "G4RToEConvForGamma.hh"
#include "G4RToEConvForElectron.hh"
#include "G4RToEConvForPositron.hh"

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ProductionCuts.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4EnergyLossTables.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------
GmRangeRejectionUA::GmRangeRejectionUA()
{
}

//----------------------------------------------------------------
void GmRangeRejectionUA::BeginOfRunAction( const G4Run* )
{
  bUseDirection = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":UseDirection",0));
}

//----------------------------------------------------------------
G4ClassificationOfNewTrack GmRangeRejectionUA::ClassifyNewTrack(const G4Track* aTrack)
{
  G4ParticleDefinition* part = aTrack->GetDefinition();
  if( part->GetPDGCharge() == 0 ) return fUrgent; // ONLY FOR CHARGED PARTICLES

  // G4double safety = aTrack->GetStep()->GetPreStepPoint()->GetSafety();
  G4VPhysicalVolume* pv = aTrack->GetVolume();
  if( !pv ) return fUrgent;
  G4LogicalVolume* lv = pv->GetLogicalVolume();
  G4double range = G4EnergyLossTables::GetRange(part,aTrack->GetKineticEnergy(),lv->GetMaterialCutsCouple());

  G4double safety = 0;
  if(bUseDirection) {
     safety = GmGeometryUtils::GetInstance()->GetDistanceToOutDirection(aTrack);
  } else {
    safety = GmGeometryUtils::GetInstance()->GetDistanceToOutNoDirection(aTrack);  
  }

  //  G4cout << part->GetParticleName() << " VOL " << lv->GetName() << " E " << aTrack->GetKineticEnergy() << " safety " << safety << " range " << range << G4endl;
  if( safety < range ) {
    return fUrgent;
  } else {
    return fKill;
  }

  return fUrgent;
}
