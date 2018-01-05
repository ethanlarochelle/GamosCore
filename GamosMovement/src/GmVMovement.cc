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
#include "GmVMovement.hh"
#include "GmMovementMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

//------------------------------------------------------------------------
GmVMovement::GmVMovement(std::vector<G4String>& words, GmMovementType mt )
{
  theVolumes = GmGeometryUtils::GetInstance()->GetPhysicalVolumes( words[1], true );
  theValue = GmGenUtils::GetValue(words[2]);
  theAxis = G4ThreeVector(GmGenUtils::GetValue(words[3]),GmGenUtils::GetValue(words[4]),GmGenUtils::GetValue(words[5]));
  if( fabs(theAxis.mag() - 1.) > 1.E-9 ) {
    G4cerr << "WARNING GmVMovement::GmVMovement axis is not normalised to 1 " << theAxis;
    theAxis = theAxis.unit();
  } 
  theInterval = GmGenUtils::GetValue(words[6]);
  if( words.size() > 7 ) {
    theOffset = GmGenUtils::GetValue(words[7]);
  } else { 
    theOffset = 0.;
  }
  if( words.size() > 8 ) {
    theNIntervals = GmGenUtils::GetInteger(words[8]);
  } else {
    theNIntervals = INT_MAX;
  }

  theType = mt;

  theNMoved = 0;

  InitialiseLastValue();

  GmMovementMgr::GetInstance()->AddMovement(this);

}


//------------------------------------------------------------------------
G4bool GmVMovement::IsNewMovement( G4double timeOrNEvents )
{
  G4bool bIsNew = FALSE;
  if( theNMoved >= theNIntervals ) {
    //    G4cout << " GmVMovement::IsNewMovement theNMoved " << theNMoved << " ltheNIntervals " << theNIntervals << G4endl;
    return FALSE;
  }

  if( timeOrNEvents - theLastValue >= theInterval ){
    theLastValue = theInterval * int(timeOrNEvents/theInterval);
    bIsNew = TRUE;
    theNMoved++;
  } else {
    bIsNew = FALSE;
  }
  //    G4cout << " GmVMovement::IsNewMovement timeOrNEvents " << timeOrNEvents << " lastValue " << theLastValue << " interval " << theInterval << " = " << bIsNew << G4endl;
  return bIsNew;
}

//------------------------------------------------------------------------
void GmVMovement::InitialiseLastValue()
{
  theLastValue = theOffset;  
}

//------------------------------------------------------------------------
GmVMovement::~GmVMovement() 
{
}
