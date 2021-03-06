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
#ifndef GmGenerDistEnergyFromFile_h
#define GmGenerDistEnergyFromFile_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4ParticleGun;
class G4Event;
class GmAnalysisMgr;
#include <map>

#include "GamosCore/GamosGenerator/include/GmVGenerDistEnergy.hh"
class GmParticleSource;
enum EFFCalcType { EFFCT_Fixed, EFFCT_Histogram, EFFCT_Interpolate, EFFCT_InterpolateLog };

class GmGenerDistEnergyFromFile : public GmVGenerDistEnergy
{
public:
  GmGenerDistEnergyFromFile();
  virtual ~GmGenerDistEnergyFromFile(){};

  virtual G4double GenerateEnergy( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4String theFileName;
  
  void ReadEnergyDist();

  G4double theEnergyUnit;

  std::multimap<G4double,G4double> theProbaccumEner; // multimap <added up probability, energy>
  std::map<G4double,G4double> theEnerProb; // map <energy, probability>

  G4double theHBin;

  EFFCalcType theCalculationType;

  G4double theUnit;

};

#endif
