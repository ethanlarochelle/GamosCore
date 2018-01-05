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
#ifndef GmPSWithDirection_h
#define GmPSWithDirection_h 1

#include "globals.hh"
#include <vector>
#include "G4PSDirectionFlag.hh"
////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring Surface Flux.
//  Current version assumes only for G4Box shape, and the surface
//  is defined at the -Z plane of the box.
//   The surface flux is given in the unit of area. 
//    e.g.  sum of 1/cos(T)/mm2,  where T is a incident angle of the
//                                track on the surface.
//
//
// Surface is defined at the -Z surface.
// Direction                  -Z   +Z
//   0  IN || OUT            ->|<-  |        fFlux_InOut
//   1  IN                   ->|    |        fFlux_In
//   2  OUT                    |<-  |        fFlux_Out
//
///////////////////////////////////////////////////////////////////////////////

class GmPSWithDirection
{
  public: // with description
      GmPSWithDirection();
      virtual ~GmPSWithDirection();

  virtual void SetParameters( const std::vector<G4String>& params );

  protected:
      G4int  fDirection;

};

#endif
