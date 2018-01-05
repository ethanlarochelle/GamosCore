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
#include "GmUniformEMField.hh"
#include "G4EqMagElectricField.hh"
#include "G4ClassicalRK4.hh"
#include "G4FieldManager.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"

GmUniformEMField::GmUniformEMField( G4ThreeVector MagFieldVector,G4ThreeVector ElecFieldVector ) 
{ 
  // Create an equation of motion for this field
  G4EqMagElectricField* fEquation = new G4EqMagElectricField(this); 
  G4int nvar = 8;
  G4ClassicalRK4* fStepper = new G4ClassicalRK4( fEquation, nvar ); 
  // Get the global field manager 
  G4FieldManager* fFieldManager= G4TransportationManager::GetTransportationManager()->GetFieldManager();
  // Set this field to the global field manager 
  fFieldManager->SetDetectorField(this);
  G4double fMinStep = 0.010*CLHEP::mm ; // minimal step of 10 microns
  G4MagInt_Driver* fIntgrDriver = new G4MagInt_Driver(fMinStep, fStepper, fStepper->GetNumberOfVariables() );
  G4ChordFinder* fChordFinder = new G4ChordFinder(fIntgrDriver);
  fFieldManager->SetChordFinder( fChordFinder );

  theField = new double(6);
  theField[0] = MagFieldVector.x();
  theField[1] = MagFieldVector.y();
  theField[2] = MagFieldVector.z();
  theField[3] = ElecFieldVector.x();
  theField[4] = ElecFieldVector.y();
  theField[5] = ElecFieldVector.z();
}

void GmUniformEMField::GetFieldValue(const double*, double *Bfield ) const
{ 
  Bfield = theField;  
  G4cout << " GmUniformEMField::GetFieldValue " << Bfield[0] << " " 
	 << Bfield[1] << " " 
	 << Bfield[2] << " " 
	 << Bfield[3] << " " 
	 << Bfield[4] << " " 
	 << Bfield[5] << G4endl;
}

