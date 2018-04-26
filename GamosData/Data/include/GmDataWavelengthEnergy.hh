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
////////////////////////////////////////////////////////////////////////
// Optical Photon Wavelength Data Type
////////////////////////////////////////////////////////////////////////
//
// File GmDataWavelengthEnergy.hh
// Description: Wavelength data type.
// Created: 2013-02-22
// Author: Adam Glaser
//
// Creates wavelength data type for optical photons.
//
// mail:  adam.k.glaser@dartmouth.edu
//
////////////////////////////////////////////////////////////////////////

#ifndef GmDataWavelengthEnergy_hh
#define GmDataWavelengthEnergy_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"

class GmDataWavelengthEnergy : public GmVData
{
public:
  GmDataWavelengthEnergy();
  ~GmDataWavelengthEnergy();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );
  virtual G4double GetValueFromTrack( const G4Track* aTrack, G4int index = 0 );

};
#endif
