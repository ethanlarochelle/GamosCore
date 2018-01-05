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
//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: GmBSLivermoreBremsstrahlungModel.hh,v 1.24 2016/10/09 18:10:15 arce Exp $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     GmBSLivermoreBremsstrahlungModel
//
// Author:        Andreas Schaelicke & Vladimir Ivantchenko
//
// Creation date: 04.10.2011
//
// Modifications:
//
//
// Class Description:
//
// Implementation of the bremssrahlung energy spectrum using
// 1. S.M. Seltzer and M.J. Berger Nucl. Instr. Meth. B12 (1985) 95
// 2. S.M. Seltzer and M.J. Berger Atomic data and Nuclear Data 
//    Tables 35 (1986) 345
// Cross section computation in the base class G4eBremsstrahlungRelModel

// -------------------------------------------------------------------
//

#ifndef GmBSLivermoreBremsstrahlungModel_h
#define GmBSLivermoreBremsstrahlungModel_h 1
#include "G4LivermoreBremsstrahlungModel.hh"
#include "GmVPSModelHelper.hh"
#include "globals.hh"

class G4Physics2DVector;
class GmAnalysisMgr;

class GmBSLivermoreBremsstrahlungModel  : public G4LivermoreBremsstrahlungModel, public GmVPSModelHelper
{

public:

  GmBSLivermoreBremsstrahlungModel(const G4ParticleDefinition* p = 0, 
				 const G4String& nam = "LowEnBrem");

  virtual ~GmBSLivermoreBremsstrahlungModel();

  virtual void SampleSecondariesPS(std::vector<G4DynamicParticle*>*,
				   std::vector<G4double>& weightVect,
				   const G4MaterialCutsCouple*,
				   const G4DynamicParticle*,
				   G4double tmin,
				   G4double maxEnergy,
				   const G4Track& track);

private:

  GmAnalysisMgr* theAnaMgr;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


#endif
