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
#ifndef GmG4PSPassageTrackLength_h
#define GmG4PSPassageTrackLength_h 1

#include "GmG4PSTrackLength.hh"
#include "G4THitsMap.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring only track length.
//   The tracks which passed a geometry is taken into account.
// 
///////////////////////////////////////////////////////////////////////////////

class GmG4PSPassageTrackLength : public GmG4PSTrackLength
{
 
public: // with description
  GmG4PSPassageTrackLength(G4String name);
  virtual ~GmG4PSPassageTrackLength();
  
  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
      G4bool IsPassed(G4Step*);

  private:
      G4int fCurrentTrkID;
      G4double fTrackLength;

};

#endif
