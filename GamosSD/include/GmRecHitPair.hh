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
#ifndef GmRecHitPair_HH
#define GmRecHitPair_HH

#include "globals.hh"
#include <vector>
class GmRecHit;


class GmRecHitPair 
{

public:
  GmRecHitPair(){};
  
  GmRecHitPair( GmRecHit* rhit1, GmRecHit* rhit2 );
  virtual ~GmRecHitPair(){};

  G4double GetEnergy() const;
  GmRecHit* recHit1() const { return theRecHit1; }
  GmRecHit* recHit2() const { return theRecHit2; }
private:
  GmRecHit *theRecHit1, *theRecHit2;
};

typedef std::vector<GmRecHitPair> GmRecHitPairList;

#endif
