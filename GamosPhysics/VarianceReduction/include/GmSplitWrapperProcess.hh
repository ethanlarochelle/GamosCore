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
// ------------------------------------------------------------
//        GAMOS class header file 
//
// Class Description
//
// This class wraps a process invoking NSplit times the PostStepDoIt method// ------------------------------------------------------------

#ifndef GmSplitWrapperProcess_h 
#define GmSplitWrapperProcess_h 1

#include "G4WrapperProcess.hh"

class GmSplitWrapperProcess : public G4WrapperProcess
{
  public: // with description
  //  constructor requires the process name and type
      GmSplitWrapperProcess(const G4String& aName =  "Wrapped",
                 G4ProcessType   aType = fNotDefined );

  public: 
  //  destructor 
      virtual ~GmSplitWrapperProcess();

  public: 
      virtual G4VParticleChange* PostStepDoIt(
                             const G4Track& track,
                             const G4Step&  stepData
                            );

  G4int GetNSplit() const
  {
    return theNSplit;
  }
  void SetNSplit( G4int nspl ) 
  {
    theNSplit = nspl;
  }


private:
  G4int theNSplit;
};

#endif
