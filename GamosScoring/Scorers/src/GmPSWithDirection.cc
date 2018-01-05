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
#include "GmPSWithDirection.hh"
#include "G4StepStatus.hh"
////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring Surface Flux.
//  Current version assumes only for G4Box shape, and the surface
//  is fixed on -Z plane.
//
// Surface is defined at the -Z surface.
// Direction                  -Z   +Z
//   0  IN || OUT            ->|<-  |
//   1  IN                   ->|    |
//   2  OUT                    |<-  |
///////////////////////////////////////////////////////////////////////////////

GmPSWithDirection::GmPSWithDirection()
{
  fDirection = fFlux_InOut;
}

GmPSWithDirection::~GmPSWithDirection()
{;}

void GmPSWithDirection::SetParameters( const std::vector<G4String>& params)
{
 if( params.size() != 1 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSWithDirection::SetParameters",
"There should be one parameters: DIRECTION",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
 }

 if( params[0] == "InOut" ) {
   fDirection = fFlux_InOut;
 } else if( params[0] == "In" ) {
   fDirection = fFlux_In;
 } else if( params[0] == "Out" ) {
   fDirection = fFlux_Out;
 } else {
   G4Exception("GmPSWithDirection::SetParameters",
	       "The parameter should be InOut, In or Out",
	       FatalErrorInArgument,G4String("It is: "+params[0]).c_str());
 }
 G4cout << " GmPSWithDirection  direction " << fDirection << G4endl;

}

