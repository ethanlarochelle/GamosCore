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
#include "GmUniformMagField.hh"

#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ClassicalRK4.hh"
#include "G4FieldManager.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------

GmUniformMagField::GmUniformMagField(G4ThreeVector FieldVector) 
{
  G4UniformMagField* fEMfield = new G4UniformMagField( FieldVector );
  // Create an equation of motion for this field
  G4Mag_UsualEqRhs* fEquation = new  G4Mag_UsualEqRhs(fEMfield); 
  G4int nvar = 8;
  G4ClassicalRK4* fStepper = new G4ClassicalRK4( fEquation, nvar ); 
  // Get the global field manager 
  G4FieldManager* fFieldManager= G4TransportationManager::GetTransportationManager()->GetFieldManager();
  // Set this field to the global field manager 
  fFieldManager->SetDetectorField(fEMfield);
  G4double fMinStep = 0.010*CLHEP::mm ; // minimal step of 10 microns
  G4MagInt_Driver* fIntgrDriver = new G4MagInt_Driver(fMinStep, fStepper, fStepper->GetNumberOfVariables() );
  G4ChordFinder* fChordFinder = new G4ChordFinder(fIntgrDriver);
  fFieldManager->SetChordFinder( fChordFinder );
}




