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
///////////////////////////////////////////////////////////////////////////////
// File : GmCountProcessesUA.h
// Author: P.Arce  12.09.01
// Description:
// Modifications:
//  
///////////////////////////////////////////////////////////////////////////////

#ifndef GmCountProcessesUA_h
#define GmCountProcessesUA_h 1

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include "globals.hh"

#include <string>
#include <map>
#include "G4ParticleDefinition.hh"

typedef std::multimap< G4String, G4String > mss;
typedef std::pair<G4String,G4String> pss;
typedef std::map <pss, int> mpssi;
typedef std::map<long int, mpssi* > mlmpssi;
typedef std::map<G4String,int > msi;
typedef std::map<long int, msi* > mlmsi;

class G4RRun;
class G4Track;
class G4Step;

class GmCountProcessesUA : public GmUserRunAction, 
			   public GmUserTrackingAction, 
			   public GmUserSteppingAction
{

 public:
  GmCountProcessesUA();
  ~GmCountProcessesUA();

 public:
 virtual void BeginOfRunAction(const G4Run* );
 virtual void EndOfRunAction(const G4Run* );
 virtual void PreUserTrackingAction(const G4Track* aTrack);
 virtual void UserSteppingAction(const G4Step* aStep);

  //---- Dump list of processes for each particle. printNsteps = 1 print in how many step the process was called, print only those processes with this number <> 0
  void DumpProcessList( std::ostream& out = G4cout);
  void DumpProcessCount( std::ostream& out = G4cout);
  void DumpCreatorProcessCount( std::ostream& out = G4cout);
  void DumpParticleCount( std::ostream& out = G4cout);

 private:
  mss theProcessList; // list of processes 
  mlmpssi theProcessCount;  
  mlmpssi theCreatorProcessCount;

  mlmsi theParticleCount;

  G4String theNameSuffix;
  G4bool bCreatorModel;
};

#endif
