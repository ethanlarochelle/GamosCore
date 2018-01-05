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
#ifndef GmVNumericDistribution_h
#define GmVNumericDistribution_h 1

#include "GamosCore/GamosBase/Base/include/GmVDistribution.hh"

class GmVNumericDistribution : public GmVDistribution
{
  
public: 
  GmVNumericDistribution(G4String name);
  virtual ~GmVNumericDistribution(){};

  virtual void BuildData();
  virtual void ReadFile();

  virtual G4double GetValueFromStep(const G4Step* aStep);
  virtual G4double GetValueFromTrack(const G4Track* aTrack);
  virtual G4double GetNumericValueFromIndex(const G4double indexVal);

  virtual G4double InterpolateValue(const G4double& indexVal, const G4double& indexDown, const G4double& indexUp, const G4double& valDown, const G4double& valUp ) = 0;

  virtual void ReadFileUnknown( G4String& fileName );

  virtual void Normalize();

  G4int GetFileType( G4String& fileName );
  std::map<G4double,G4double> GetValues() const {
    return theValues;
  }

protected:
  void CheckOtherFileNames( G4String& fileNameROOT, G4String& fileNameCSV, G4String& fileNameText, G4String& fileName, G4int fNumber );
  void ReadFileName( G4String& fileName );
#ifndef GAMOS_NO_ROOT
  void ReadFileROOT( G4String& fileName );
#endif
  void ReadFileCSV( G4String& fileName );
  void ReadFileText( G4String& fileName );

protected:
  
  std::map<G4double,G4double> theValues;

  G4double theMinimum;
  G4double theMaximum;

  G4bool bAllowOutOfLimits;

};

#endif
