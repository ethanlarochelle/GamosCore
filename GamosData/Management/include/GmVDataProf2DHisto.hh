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
#ifndef GmVDataProf2DHisto_hh
#define GmVDataProf2DHisto_hh

#include "GmVDataNon1DHisto.hh"

class GmVDataProf2DHisto : public GmVDataNon1DHisto
{
public:
  GmVDataProf2DHisto();
  ~GmVDataProf2DHisto();

  virtual void BookHisto( G4String hNamePrefix, G4int hNumber, G4int index, GmAnalysisMgr* anaMgr );

  virtual void FillHisto( const G4Step* aStep, G4int index );
  virtual void FillHisto( const G4Track* aTrack, G4int index );
  virtual void FillHisto( const G4Track* aTrack1, const G4Track* aTrack2, G4int index );
  virtual void FillHisto( const G4Event* anEvent, G4int index );
  virtual void FillHisto( const G4Run* aRun, G4int index );

protected:

};
#endif
