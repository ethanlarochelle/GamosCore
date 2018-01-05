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
#include "GmCSTrackInfo.hh"
#include "GmCutsStudyMgr.hh"
#include "GmCutsEnergy2Range.hh"
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

GmCutsEnergy2Range* GmCSTrackInfo::theEnergy2Range = 0;

GmCSTrackInfo::GmCSTrackInfo(const G4Track* aTrack)
{
  if( theEnergy2Range == 0 ) theEnergy2Range = new GmCutsEnergy2Range;
  theTrackID = aTrack->GetTrackID();
  G4LogicalVolume* lv = aTrack->GetVolume()->GetLogicalVolume();
  theRegion = lv->GetRegion();
  theProcess = const_cast<G4VProcess*>(aTrack->GetCreatorProcess());
  theParticle = aTrack->GetDefinition();
  theEnergy = aTrack->GetKineticEnergy();
  //  G4cout << " particle " << theParticle << " = " << aTrack->GetDefinition() << G4endl;
  theRange = theEnergy2Range->Convert( theEnergy, theParticle, lv->GetMaterialCutsCouple() );
  //  theRange = G4EnergyLossTables::GetRange(theParticle,theEnergy,lv->GetMaterialCutsCouple());
  theSafety = GmGeometryUtils::GetInstance()->GetDistanceToOutNoDirection(aTrack);
  
  bAccepted = FALSE;

  GmCutsStudyMgr::GetInstance()->AddTrackInfo( this );

  //  G4cout << " GmCSTrackInfo::GmCSTrackInfo track " << theTrackID << " theEnergy " << theEnergy << " theRange " << theRange << " theParticle " << theParticle->GetParticleName() << " material " << lv->GetMaterialCutsCouple()->GetMaterial()->GetName() << G4endl;
  
  theParent = 0;
}

GmCSTrackInfo::~GmCSTrackInfo()
{;}

void GmCSTrackInfo::Print(std::ostream& out ) const
{
  out << " GmCSTrackInfo ID= "<< theTrackID
      << " parent= ";
  if( theParent ) { 
    out  << theParent->GetTrackID();
  } else {
    out << "-1"; 
  }
  out << " reg= " << theRegion->GetName()
      << " proc= " << theProcess->GetProcessName()
      << " part= " << theParticle->GetParticleName()
      << " range= " << theRange
      << " energy= " << theEnergy
      << " accepted " << bAccepted << G4endl;

}
