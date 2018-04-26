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
#include "G4VPrimitiveScorer.hh"
#include "GmPSCylindricalZPhiDoseDeposit.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4VSolid.hh"
#include "G4UnitsTable.hh"
#include "G4PhantomParameterisation.hh"
#include "G4EmCalculator.hh"
#include "CLHEP/Random/RandFlat.h"

//--------------------------------------------------------------------
GmPSCylindricalZPhiDoseDeposit::GmPSCylindricalZPhiDoseDeposit(G4String name) 
   :GmVPrimitiveScorer(name)
{ 
  theUnit = CLHEP::gray;
  theUnitName = G4String("Gy");

  theGeomUtils = GmGeometryUtils::GetInstance();  
}


//--------------------------------------------------------------------
G4bool GmPSCylindricalZPhiDoseDeposit::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return false;  // it is 0 when called by GmScoringMgr after last event
  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return false;

  //----- Choose a random point between pre and post step, in local sphere coordinates
  G4ThreeVector postPos = aStep->GetPostStepPoint()->GetPosition() - theCentre;
  G4ThreeVector prePos = aStep->GetPreStepPoint()->GetPosition() - theCentre;
  postPos = theAxisRM * postPos;
  prePos = theAxisRM * prePos;
  G4ThreeVector pos = prePos + CLHEP::RandFlat::shoot() * (postPos-prePos);
  //  G4cout << " GmPSCylindricalZPhiDoseDeposit prepost " << prePos << " " << postPos << " ORIG " <<  aStep->GetPreStepPoint()->GetPosition() << " " << aStep->GetPostStepPoint()->GetPosition()  << G4endl;
  //--- Get the sphere slice 
  G4int indexZ = (pos.z() - theMinZ ) / theStepZ;
  G4int indexPhi = (pos.phi() - theMinPhi ) / theStepPhi;
  //  G4cout << " GmPSCylindricalZPhiDoseDeposit indexZ " << indexZ << " Phi " << indexPhi << G4endl;

  if( indexZ < 0 || indexZ >= theNBinsZ 
      || indexPhi < 0 || indexPhi >= theNBinsPhi 
      || pos.perp() < theMinR || pos.perp() > theMaxR ) {
    return false;
  } else {
    G4int index = theNBinsZ*indexPhi + indexZ;

    G4double volume;
    if( bTotalVolume ) {
      volume = theTotalVolume;
    } else {
      volume = theVolumes[index];
    }

    G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
    G4double dose  = edep / ( density * volume );
    G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) 
      G4cout << "  GmPSCylindricalZPhiDoseDeposit::ProcessHits dose " << dose
	     << " edep " << edep
	     << " density " << density / (CLHEP::g/CLHEP::cm3)
	     << " Z slice " << indexZ
	     << " Phi slice " << indexPhi
	     << " volume " << volume << G4endl;
#endif
    
    FillScorer( aStep, index, dose, weight );

  }

  return TRUE;
} 

//--------------------------------------------------------------------
void GmPSCylindricalZPhiDoseDeposit::SetParameters( const std::vector<G4String>& params)
{
  if( params.size() != 8
      && params.size() != 11 
      && params.size() != 14 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmG4PSData::SetParameters",
		"There should be six, nine or twelve parameters: NSLICES_Z MIN_Z MAX_Z NSLICES_PHI MIN_PHI MAX_PHI CENTRE_X CENTRE_Y CENTRE_Z ZAXIS_X ZAXIS_Y ZAXIS_Z",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }
  
  theNBinsZ = G4int(GmGenUtils::GetValue(params[0]));
  theMinZ = GmGenUtils::GetValue(params[1]);
  G4double maxZ = GmGenUtils::GetValue(params[2]);
  theStepZ = (maxZ-theMinZ)/theNBinsZ;
  theNBinsPhi = G4int(GmGenUtils::GetValue(params[3]));
  theMinPhi = GmGenUtils::GetValue(params[4]);
  G4double maxPhi = GmGenUtils::GetValue(params[5]);
  theStepPhi = (maxPhi-theMinPhi)/theNBinsPhi;
  theMinR = GmGenUtils::GetValue(params[6]);
  theMaxR = GmGenUtils::GetValue(params[7]);
  if( params.size() >= 11 ) {
    theCentre = G4ThreeVector(GmGenUtils::GetValue(params[8]),GmGenUtils::GetValue(params[9]),GmGenUtils::GetValue(params[10]));
  }
  if( params.size() == 14 ) {
    G4ThreeVector zAxis(GmGenUtils::GetValue(params[10]),GmGenUtils::GetValue(params[12]),GmGenUtils::GetValue(params[13]));
    //normalize to 1
    zAxis /= zAxis.mag();
    CLHEP::HepRep3x3 rottemp(1.,0.,zAxis.x(),
			     0.,zAxis.z(),zAxis.y(),
			     0.,-zAxis.y(),zAxis.z());
    G4RotationMatrix* rotMat = new G4RotationMatrix(rottemp);
    theAxisRM = rotMat->inverse();
  }

  bTotalVolume = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(GetName()+":UseTotalVolume",0));

  //--- Calculate volumes
  if( !bTotalVolume ) {
    G4int index;
    for( int iZ = 0; iZ < theNBinsZ; iZ++ ){
      for( int iPhi = 0; iPhi < theNBinsPhi; iPhi++ ){
	index = theNBinsZ*iPhi+iZ;
	theVolumes[index] = theStepPhi*(pow(theMaxR,2)-pow(theMinR,2))*(theStepZ);
      }
    } 
  } else {
    theTotalVolume = (theStepPhi*theNBinsPhi)*(pow(theMaxR,2)-pow(theMinR,2))*(theNBinsZ*theStepZ);
  }

}

