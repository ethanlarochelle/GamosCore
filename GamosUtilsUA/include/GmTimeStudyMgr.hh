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
#ifndef GmTimeStudyMgr_H
#define GmTimeStudyMgr_H

#include "G4RunManager.hh"
#include <vector>
#include "G4VSteppingVerbose.hh"
class G4SteppingVerbose;
class GmTimeStudyUA;

class GmTimeStudyMgr : public G4VSteppingVerbose
{
private:
  GmTimeStudyMgr();

public:
  ~GmTimeStudyMgr();

  static GmTimeStudyMgr* GetInstance();
  void AddTimeStudy( GmTimeStudyUA* ts );
  void InitManagers();

  virtual void NewStep();
  virtual void StepInfo();

  // Following methods are not used
  virtual void TrackBanner(){}
  virtual void AtRestDoItInvoked();
  virtual void AlongStepDoItAllDone();
  virtual void PostStepDoItAllDone();
  virtual void AlongStepDoItOneByOne();
  virtual void PostStepDoItOneByOne();
  virtual void TrackingStarted();
  virtual void DPSLStarted();
  virtual void DPSLUserLimit();
  virtual void DPSLPostStep();
  virtual void DPSLAlongStep();
  virtual void VerboseTrack();
  virtual void VerboseParticleChange();

private:
  static GmTimeStudyMgr* theInstance;

  std::vector<GmTimeStudyUA*> theTimeStudies;

  static G4SteppingVerbose* theG4Verbose;
  static int theG4VerboseLevel;

};

#endif
