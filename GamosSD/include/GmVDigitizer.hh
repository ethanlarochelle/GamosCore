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
#ifndef GmVDigitizer_HH
#define GmVDigitizer_HH

#include <vector>
#include "globals.hh"

class GmDigit;
class GmHit;

class GmVDigitizer
{

public:
  
  GmVDigitizer(){};
  virtual ~GmVDigitizer(){};

  void SetName( const G4String& name ){
    theName = name; }
  void SetParams(){};

  virtual std::vector<GmDigit*> DigitizeHits(const std::vector<GmHit*>*) = 0;

  virtual void CleanDigits() = 0; //cannot be done here, because there is no info about GmDigi

  void SetSDType( G4String sdtyp ) {
    theSDType = sdtyp;
  }

protected:
  std::vector<GmDigit*> theDigits;
  G4String theName;
  G4String theSDType;
};

#endif
