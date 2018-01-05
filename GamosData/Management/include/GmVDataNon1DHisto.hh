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
#ifndef GmVDataNon1DHisto_hh
#define GmVDataNon1DHisto_hh

#include "GmVData.hh"

class GmVDataNon1DHisto : public GmVData
{
public:
  GmVDataNon1DHisto();
  ~GmVDataNon1DHisto();

  virtual void BookHisto( G4String hNamePrefix, G4int hNumber, G4int index, GmAnalysisMgr* anaMgr ) = 0;

  virtual void FillHisto( const G4Step* , G4int ){};
  virtual void FillHisto( const G4Track* , G4int ){};
  virtual void FillHisto( const G4Track* , const G4Track* , G4int ){};
  virtual void FillHisto( const G4Event* , G4int ){};
  virtual void FillHisto( const G4Run* , G4int ){};

  virtual void WriteText( const G4Step* aStep, G4bool bIsFirst );
  virtual void WriteText( const G4Track* aTrack, G4bool bIsFirst );
  virtual void WriteText( const G4Track* aTrack1, const G4Track* aTrack2, G4bool bIsFirst );
  virtual void WriteText( const G4Event* anEvent, G4bool bIsFirst );
  virtual void WriteText( const G4Run* aRun, G4bool bIsFirst );

  virtual void WriteBin( const G4Step* aStep );
  virtual void WriteBin( const G4Track* aTrack );
  virtual void WriteBin( const G4Track* aTrack1, const G4Track* aTrack2 );
  virtual void WriteBin( const G4Event* anEvent );
  virtual void WriteBin( const G4Run* aRun );

  virtual void WriteCout( const G4Step* aStep, G4bool bIsFirst );
  virtual void WriteCout( const G4Track* aTrack, G4bool bIsFirst );
  virtual void WriteCout( const G4Track* aTrack1, const G4Track* aTrack2, G4bool bIsFirst );
  virtual void WriteCout( const G4Event* anEvent, G4bool bIsFirst );
  virtual void WriteCout( const G4Run* aRun, G4bool bIsFirst );

protected:

};
#endif
