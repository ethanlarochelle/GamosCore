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
#ifndef GmIsotopeSource_H
#define GmIsotopeSource_H
//
#include "GmParticleSource.hh"
#include "GmIsotope.hh"

class GmIsotopeSource : public GmParticleSource
{
public:
  GmIsotopeSource( const G4String& name,  GmIsotope* isotope, G4double activity );
  ~GmIsotopeSource(){}

  virtual G4PrimaryVertex* GenerateVertex( G4double time );

  virtual G4String GetParticleName() const {
    return theIsotope->GetName(); }

  GmIsotopeDecay* GetCurrentDecay() const { return theCurrentDecay; }

private:
  GmIsotope* theIsotope;

  GmIsotopeDecay* theCurrentDecay; 

};

#endif // GmIsotopeSource_H
