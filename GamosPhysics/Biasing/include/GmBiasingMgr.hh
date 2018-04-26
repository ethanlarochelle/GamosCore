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
#ifndef GmBiasingMgr_h
#define GmBiasingMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmBiasingMgr    
Author:      P. Arce
Changes:     03/2016: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Manager for biasing */

#include "G4ThreeVector.hh"

#include <map>
class GmVBiasingOperator;
class GmGenericBiasingPhysics;

//------------------------------------------------------------------------
class GmBiasingMgr 
{ 
private:
  GmBiasingMgr();
 public:
  ~GmBiasingMgr();
  static GmBiasingMgr* GetInstance();

  void CreateOperator(std::vector<G4String> params );
  void AssocOper2LogVol(std::vector<G4String> params );
  void AddProcesses2Oper(std::vector<G4String> params );
  void AddParticles2Oper(std::vector<G4String> params );
  void AddParticleProcesses2Oper(std::vector<G4String> params );
  void AddSplittingProcess( std::vector<G4String> params);
  
private:
  static GmBiasingMgr* theInstance;
  std::map<G4String,GmVBiasingOperator*> theOperators;
  GmGenericBiasingPhysics* theBiasingPhysics;
};

#endif
