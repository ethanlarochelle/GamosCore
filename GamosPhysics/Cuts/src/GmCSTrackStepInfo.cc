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
#include "GmCSTrackStepInfo.hh"
#include "GmCutsStudyMgr.hh"
#include "GmCutsEnergy2Range.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Region.hh"
#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4EnergyLossTables.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4VSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

GmCSTrackStepInfo::GmCSTrackStepInfo(const G4Track* aTrack)
{
  if( GetEnergy2Range() == 0 ) CreateEnergy2Range();
  theTrackID = aTrack->GetTrackID();
  G4LogicalVolume* lv = aTrack->GetVolume()->GetLogicalVolume();
  theRegion = lv->GetRegion();
  theProcess = 0; //no interest 
  theParticle = aTrack->GetDefinition();
  theEnergy = aTrack->GetStep()->GetPreStepPoint()->GetKineticEnergy(); //use energy before step (min range would have cut it and this step would have not been produced)
  //  G4cout << " particle " << theParticle << " = " << aTrack->GetDefinition() << G4endl;
  theRange = GetEnergy2Range()->Convert( theEnergy, theParticle, lv->GetMaterialCutsCouple() );
  theSafety = GmGeometryUtils::GetInstance()->GetDistanceToOutNoDirection(aTrack);
  
  bAccepted = FALSE;

  GmCutsStudyMgr::GetInstance()->AddTrackInfo( this );

 //  G4cout << " GmCSTrackStepInfo::GmCSTrackStepInfo track " << theTrackID << " theEnergy " << theEnergy << " theRange " << theRange << " theParticle " << theParticle->GetParticleName() << " material " << lv->GetMaterialCutsCouple()->GetMaterial()->GetName() << G4endl;
  //  G4cout << " particle " << theParticle << " = " << aTrack->GetDefinition() << G4endl;

  thePosition = aTrack->GetPosition();
  theTrackLength = aTrack->GetTrackLength();
  theParent = 0;
  if( PhysicsVerb(debugVerb) ) Print();
}

GmCSTrackStepInfo::~GmCSTrackStepInfo()
{;}

void GmCSTrackStepInfo::Print(std::ostream& out ) const
{
  out << " GmCSTrackStepInfo ID= "<< theTrackID
      << " parent= ";
 if( theParent ) { 
    out  << theParent->GetTrackID();
  } else {
    out << "-1"; 
  }
  out << " energy= " << theEnergy
      << " range= " << theRange
      << " accepted= " << bAccepted 
      << " reg= " << theRegion->GetName()
      << " part= " << theParticle->GetParticleName()
      << " energy= " << theEnergy
      << " accepted= " << bAccepted 
      << " trkLength= " << theTrackLength
      << " position= " << thePosition
      << G4endl;

}
