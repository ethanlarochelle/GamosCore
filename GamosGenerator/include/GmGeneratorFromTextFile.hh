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
#ifndef GmGeneratorFromTextFile_HH
#define GmGeneratorFromTextFile_HH

class G4Event;
class GmParticleSource;
class GmIsotopeMgr;

#include "globals.hh"
#include <vector>
#include <iostream>
#include "G4VUserPrimaryGeneratorAction.hh"
#include <fstream>
#include "GmGeneratorFromFile.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

struct GenerTextFileData{
  G4int eventID;
  G4String partName;
  G4double posx, posy, posz, momx, momy, momz, time, weight;
};

class GmGeneratorFromTextFile: public GmGeneratorFromFile
{
public:
  GmGeneratorFromTextFile();
  ~GmGeneratorFromTextFile();
  void GeneratePrimaries(G4Event* evt);

private:
  void GenerateParticle( const GenerTextFileData& evtDat, G4Event* evt );

private:
  GmFileIn theFileIn;

  G4int thePrevEvent;
  GenerTextFileData thePrevData;
  
  G4int theNLinesRead;
};

#endif
