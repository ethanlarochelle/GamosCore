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
#ifndef GmVBiasingOperator_hh
#define GmVBiasingOperator_hh 1

#include "G4VBiasingOperator.hh"
class G4ParticleDefinition;
class G4VProcess;
class G4BiasingProcessInterface;

typedef std::multimap<G4String, G4String> mmss;
typedef std::multimap<G4ParticleDefinition*, G4VProcess*> mmpp;

class GmVBiasingOperator : public G4VBiasingOperator
{
public:
// ---------------
// -- Constructor:
// ---------------
  GmVBiasingOperator(G4String name);
  virtual ~GmVBiasingOperator();
  G4bool IsPhysical() const {
    return bIsPhysical;
  }
  virtual void AddParticleProcess( G4String partName, G4String procName );
  void ConvertParticleProcessToPointers();
  virtual G4bool CheckParticleProcess(G4ParticleDefinition* part, const G4BiasingProcessInterface* proc);

protected:
  mmss theParticleProcessNames;
  mmpp theParticleProcesses;
  G4bool bIsPhysical;  

};

#endif
