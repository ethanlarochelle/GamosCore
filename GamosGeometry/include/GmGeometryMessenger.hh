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
#ifndef GmGeometryMessenger_h
#define GmGeometryMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class GmUIcmdWithAString;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmGeometryMessenger: public G4UImessenger
{
public:
  GmGeometryMessenger();
  ~GmGeometryMessenger();
  
  virtual void SetNewValue(G4UIcommand*, G4String);
  
private:
  void SetMagField(const G4String& newValues);
  void SetElecField(const G4String& newValues);
  void SetEMField(const G4String& newValues);
  void SetMagFieldLocal(const G4String& newValues);
  void SetRefractionIndex(const G4String& newValues);
  void PrintTouchables(const G4String& newValues); 
  void PrintVolumeTree(const G4String& newValues);
  void PrintMaterials(const G4String& newValues);

private:
  G4UIdirectory*  theMagFieldDir;
  GmUIcmdWithAString* theMagFieldCmd;
  GmUIcmdWithAString* theElecFieldCmd;
  GmUIcmdWithAString* theEMFieldCmd;
  GmUIcmdWithAString* theMagFieldLocalCmd;

  G4UIdirectory*  theGeometryDir;
  GmUIcmdWithAString* theParallelToMassCmd;
  GmUIcmdWithAString* theRefracIndexCmd;
  GmUIcmdWithAString* thePrintTouchablesCmd;
  G4UIcmdWithAnInteger* thePrintVolumeTreeCmd;
  G4UIcmdWithAnInteger* thePrintMaterialsCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
