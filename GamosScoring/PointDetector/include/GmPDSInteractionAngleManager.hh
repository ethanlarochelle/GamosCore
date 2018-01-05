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
#ifndef GmPDSInteractionAngleManager_h
#define GmPDSInteractionAngleManager_h
#include "globals.hh"
#include <map>
#include <vector>
typedef std::map<G4double,G4String> mds;
typedef std::map<G4String, std::map<G4double,G4String>*> msmds;

class GmPDSInteractionAngleManager
{
  public:
  GmPDSInteractionAngleManager( const G4String& );
  ~GmPDSInteractionAngleManager(){};

  G4double GetHistoValue(const G4String& procname, const G4String& matname, G4double ener, const G4double cosang );

private:
  std::map<G4double,G4String>::iterator GetEnergyRange(mds* energies, G4double ener );

private:
  std::map<G4String, std::vector<G4double> > theHistoEntries;
  G4double theXmin;
  G4double theXmax;
  G4double theBinStep;
  msmds theEnergies;
  G4bool bEnergyLog;
};
#endif
