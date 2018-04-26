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
#include "RTVPlanBeam.hh"
#include "RTVPlanControlPoint.hh"
#include "G4PrimaryVertex.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include <fstream>
 
//-----------------------------------------------------------------------
ScanSpotData RTVPlanBeam::NextScanSpot( G4int& inb )
{
  if( GenerVerb(debugVerb) ) G4cout << GetIndex() << " RTVPlanBeam::NextScanSpot iNextBeam " << inb << " iCurrentControlPoint " << iCurrentControlPoint << G4endl;

  //  G4cout << "  RTVPlanBeam::NextScanSpot " << inb << " " << iCurrentControlPoint <<G4endl; //GDEB
  ScanSpotData spdata;
  G4int iNextControlPoint = iCurrentControlPoint;
  //  while( iNextControlPoint == iCurrentControlPoint ) {
  spdata = theControlPoints[iCurrentControlPoint]->NextScanSpot(iNextControlPoint);
  if( GenerVerb(debugVerb) ) G4cout << " RTVPlanBeam::NextScanSpot  iNextControlPoint !=? iCurrentControlPoint " << iNextControlPoint << " !=? " << iCurrentControlPoint << " NCPs= " << theControlPoints.size() << G4endl; 
  if( iNextControlPoint != iCurrentControlPoint ) {  // ControlPoint ScanSpot's exhausted, use next ControlPoint
    //    G4cout << " RTVPlanBeam::NextScanSpot use NextControlPoint " << iNextControlPoint << G4endl; //GDEB 
    if( iNextControlPoint >= G4int(theControlPoints.size()) ) { // all ControlPoint's of this beam exhausted, change to next beam
      inb++;
      iCurrentControlPoint = 0;
      return spdata;
    }
    spdata = theControlPoints[iNextControlPoint]->NextScanSpot(iNextControlPoint);
    iCurrentControlPoint = iNextControlPoint;
    G4cout << "End RTVPlanBeam::NextScanSpot  iCurrentControlPoint " << iCurrentControlPoint << G4endl; 
  }
  // }

  return spdata;
}

//-----------------------------------------------------------------------
RTVPlanControlPoint* RTVPlanBeam::GetControlPoint( G4int ii )
{
  if( ii < 0 || ii >= G4int(theControlPoints.size()) ) {
    G4Exception("RTVPlanSource::GetControlPoint",
		"",
		FatalErrorInArgument,
		G4String("Asking for control point " + GmGenUtils::itoa(ii) + " while there are only " + GmGenUtils::itoa(theControlPoints.size())).c_str());
  }

  return theControlPoints[ii];

}
