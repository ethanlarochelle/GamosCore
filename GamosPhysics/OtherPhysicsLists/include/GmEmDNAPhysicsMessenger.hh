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
// ----------------------------------------------------------------------------
//                 GEANT 4 - Hadrontherapy example
// ----------------------------------------------------------------------------
// Code developed by:
//
// G.A.P. Cirrone(a)*, F. Di Rosa(a), S. Guatelli(b), G. Russo(a)
// 
// (a) Laboratori Nazionali del Sud 
//     of the National Institute for Nuclear Physics, Catania, Italy
// (b) National Institute for Nuclear Physics Section of Genova, genova, Italy
// 
// * cirrone@lns.infn.it
// ----------------------------------------------------------------------------

#ifndef GmEmDNAPhysicsMessenger_HH
#define GmEmDNAPhysicsMessenger_HH 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GmEmDNAPhysics;
class G4UIdirectory;
class GmUIcmdWithAString;

class GmEmDNAPhysicsMessenger: public G4UImessenger {

public:
  
  GmEmDNAPhysicsMessenger(GmEmDNAPhysics* physList);
  
  ~GmEmDNAPhysicsMessenger();
  
  virtual void SetNewValue(G4UIcommand*, G4String);
  
private:
  
  GmEmDNAPhysics* theGmPhysicsList;   
  G4UIdirectory* listDir;
  GmUIcmdWithAString* theGmPhysicsListCmd;
  GmUIcmdWithAString* theGmPhysicsListCmdOld;
};

#endif
