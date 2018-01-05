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
#ifndef GmVDistribution_h
#define GmVDistribution_h 1

class G4Track;
class G4Step;
#include "globals.hh"
#include <vector>
#include <map>

class GmVData;
// class description:
//
//  This is the abstract base class of a distribution

class GmVDistribution
{
  
public: 
  GmVDistribution(G4String name);
  virtual ~GmVDistribution(){};

  virtual void BuildData() = 0;
  virtual void ReadFile() = 0;

  virtual void SetParameters( std::vector<G4String>& ){}
  
  virtual G4double GetValueFromStep(const G4Step* aStep) = 0;
  virtual G4double GetValueFromTrack(const G4Track* aTrack) = 0;

  virtual void Normalize() = 0;

  inline G4String GetName() const {
    return theName; }
  inline G4String GetClass() const {
    return theClass; }
  inline void SetClass( G4String cl ) {
    theClass = cl;  }

protected:
  G4String theName;
  G4String theClass;
  
  GmVData* theData;

};

#endif
