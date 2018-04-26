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
#ifndef GmGenerDistPositionDiscFromFile_h
#define GmGenerDistPositionDiscFromFile_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4ParticleGun;
class G4Event;
class GmAnalysisMgr;
#include <map>

#include "GmVGenerDistPosition2D.hh"
class GmParticleSource;

class GmGenerDistPositionDiscFromFile : public GmVGenerDistPosition2D
{
public:
  GmGenerDistPositionDiscFromFile();
  virtual ~GmGenerDistPositionDiscFromFile(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4String theFileName;
  
  void ReadPositionDist();

  G4double thePositionUnit;

  std::multimap<G4double,G4double> theProbaccumRadius; // multimap <added up probability, radius>
  std::map<G4double,G4double> theRadiusProb; // map <energy, probability>

  G4double theHBin;

  EFFCalcType theCalculationType;

  G4double theUnit;

};

#endif
