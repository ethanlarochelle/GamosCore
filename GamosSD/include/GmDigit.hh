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
#ifndef GmDigit_HH
#define GmDigit_HH

#include "globals.hh"
#include "GamosCore/GamosSD/include/GmEDepo.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include <map>
#include <iostream>

class GmHit;

class GmDigit
{

public:
  
  GmDigit( GmHit* hit );
  ~GmDigit(){};

  void AddHit( GmHit* hit );

  void Draw();
  void Print(std::ostream& os = G4cout );

  G4double GetEnergy() const { return theEnergy;}
  G4double GetTime() const;
  G4double GetTimeMin() const { return theTimeMin;}
  G4double GetTimeMax() const { return theTimeMax;}
  G4ThreeVector GetPosition() const { return thePosition;}
  std::vector<GmHit*> GetHits() const { return theHits; }
  G4int GetNHits() const { return theHits.size(); }
  G4String GetSDType() const { return theSDType; }

  void SetEnergy( G4double ener ) { theEnergy = ener;}
  void SetTimeMin( G4double tim ) { theTimeMin = tim;}
  void SetTimeMax( G4double tim ) { theTimeMax = tim;}
  void SetPosition( G4ThreeVector pos ) { thePosition = pos;}
  void SetSDType( G4String sdtyp )  { theSDType = sdtyp; }


protected:
  G4double theEnergy;
  G4double theTimeMin;
  G4double theTimeMax;
  G4ThreeVector thePosition;
  std::vector<GmHit*> theHits;
  G4String theSDType;
  G4int theHitsTimeType;

};

typedef std::map< G4String, std::vector<GmDigit*> > gamosSDDigitMap;
typedef std::vector<GmDigit*> gamosDigitList;

std::ostream& operator<<(std::ostream& os, const GmDigit& );

#endif
