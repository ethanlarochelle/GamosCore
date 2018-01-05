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
#ifndef GmVDeadTimeDUList_HH
#define GmVDeadTimeDUList_HH

#include "GamosCore/GamosSD/include/GmHit.hh"
#include <map>
struct DTHitInfo{
  G4double time;
  G4int eventID;
};

class GmVDeadTimeDUList : public std::map<unsigned long long,DTHitInfo>
{
public:
  GmVDeadTimeDUList(){};
  virtual ~GmVDeadTimeDUList(){};

  virtual void AddDetUnit( GmHit* ){}; //t = 0
  void Clean(G4double time);

  virtual G4bool FindDetUnit( GmHit*, G4double deadTime );

protected:
  G4int theNShift;

};

#endif
