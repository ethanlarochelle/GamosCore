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
//---------------------------------------------------------------
//
// GmVEWBiasingOperation
//


#ifndef GmVEWBiasingOperation_hh
#define GmVEWBiasingOperation_hh 1

#include "G4VBiasingOperation.hh"
#include "G4ParticleChange.hh"


class GmVEWBiasingOperation : public G4VBiasingOperation {
public:
  // -- Constructor :
  GmVEWBiasingOperation(G4String name);
  // -- destructor:
  virtual ~GmVEWBiasingOperation();
  
public:
  G4bool IsInPlane(const G4ThreeVector dir, const G4ThreeVector pos);
  
public:
  // ----------------------------------------------
  // -- Additional methods, specific to this class:
  // ----------------------------------------------
  // -- Splitting factor:
  void SetNSplitAndInverse(G4int splittingFactor)
  {
    fNSplit = splittingFactor;
    fNSplitInv = 1./splittingFactor;
  }
  G4int GetNSplit() const { return fNSplit; }
  
protected:
  G4int fNSplit;
  G4double fNSplitInv;
  G4ParticleChange fParticleChange;
  G4double thePlaneX;
  G4double thePlaneY;
  G4double thePlaneZ;
  G4double theGasDensity;

  G4double theRRZ;

};

#endif
