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
#include "GmTrajStepPosMom.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4TrackVector.hh"


//----------------------------------------------------------------------
GmTrajStepPosMom::GmTrajStepPosMom( const G4Step* aStep, TSPUse pointToUse )
{
  thePointToUse = pointToUse;

  if( pointToUse == TSPUPrePost || pointToUse == TSPUPre ) {
    G4StepPoint* sp = aStep->GetPreStepPoint();
    thePositionPre = sp->GetPosition();
    theMomentumPre = sp->GetMomentum();
  }

  if( pointToUse == TSPUPrePost || pointToUse == TSPUPost ) {
    G4StepPoint* sp = aStep->GetPostStepPoint();
    thePositionPost = sp->GetPosition();
    theMomentumPost = sp->GetMomentum();
  }

}


//----------------------------------------------------------------------
GmTrajStepPosMom::~GmTrajStepPosMom()
{
}

//----------------------------------------------------------------------
G4ThreeVector GmTrajStepPosMom::GetMomentumDirectionPre() const 
{ 
  G4double mommag = theMomentumPre.mag();

  if(mommag != 0. ) {
    return theMomentumPre/theMomentumPre.mag(); 
  } else {
    return G4ThreeVector();
  }

}

//----------------------------------------------------------------------
G4ThreeVector GmTrajStepPosMom::GetMomentumDirectionPost() const 
{ 
  G4double mommag = theMomentumPost.mag();

  if(mommag != 0. ) {
    return theMomentumPost/theMomentumPost.mag(); 
  } else {
    return G4ThreeVector();
  }

}

//----------------------------------------------------------------------
G4ThreeVector GmTrajStepPosMom::GetMomentumDirection() const 
{ 
  if( thePointToUse == TSPUPre ) {
    return GetMomentumDirectionPre();
  } else if( thePointToUse == TSPUPost ) {
    return GetMomentumDirectionPost();
  } else {
    G4Exception("GmTrajStepPosMom::GetMomentumDirection",
		"Error",
		FatalException,
		"Asking when Pre and Post variables are filled, it is not possible to know which to use. Please ask for GetMomentumDirectionPre() or GetMomentumDirectionPost()");
  }

  return G4ThreeVector();

}

//----------------------------------------------------------------------
G4ThreeVector GmTrajStepPosMom::GetMomentum() const 
{ 
  if( thePointToUse == TSPUPre ) {
    return GetMomentumPre();
  } else if( thePointToUse == TSPUPost ) {
    return GetMomentumPost();
  } else {
    G4Exception("GmTrajStepPosMom::GetMomentum",
		"Error",
		FatalException,
		"Asking when Pre and Post variables are filled, it is not possible to know which to use. Please ask for GetMomentumPre() or GetMomentumPost()");
  }

  return G4ThreeVector();

}

//----------------------------------------------------------------------
G4ThreeVector GmTrajStepPosMom::GetPosition() const 
{ 
  if( thePointToUse == TSPUPre ) {
    return GetPositionPre();
  } else if( thePointToUse == TSPUPost ) {
    return GetPositionPost();
  } else {
    G4Exception("GmTrajStepPosMom::GetPosition",
		"Error",
		FatalException,
		"Asking when Pre and Post variables are filled, it is not possible to know which to use. Please ask for GetPositionPre() or GetPositionPost()");
  }

  return G4ThreeVector();

}


//----------------------------------------------------------------------
G4ThreeVector GmTrajStepPosMom::GetDeltaPosition() const 
{ 
  if( thePointToUse == TSPUPrePost ) {
    return thePositionPost-thePositionPre;
  } else {
    G4Exception("GmTrajStepPosMom::GetPosition",
		"Error",
		FatalException,
		"Asking when not Pre and Post variables are filled, it is not possible to know which to use. Please instantiate it with TSPUPrePost");
  }

  return G4ThreeVector();

}


//----------------------------------------------------------------------
G4ThreeVector GmTrajStepPosMom::GetDeltaMomentum() const 
{ 
  if( thePointToUse == TSPUPrePost ) {
    return theMomentumPost-theMomentumPre;
  } else {
    G4Exception("GmTrajStepPosMom::GetMomentum",
		"Error",
		FatalException,
		"Asking when not Pre and Post variables are filled, it is not possible to know which to use. Please instantiate it with TSPUPrePost");
  }

  return G4ThreeVector();

}


//----------------------------------------------------------------------
G4ThreeVector GmTrajStepPosMom::GetDeltaMomentumDirection() const 
{ 
  if( thePointToUse == TSPUPrePost ) {
    return GetMomentumDirectionPost()-GetMomentumDirectionPre();
  } else {
    G4Exception("GmTrajStepPosMom::GetMomentumDirection",
		"Error",
		FatalException,
		"Asking when not Pre and Post variables are filled, it is not possible to know which to use. Please instantiate it with TSPUPrePost");
  }

  return G4ThreeVector();

}

//----------------------------------------------------------------------
G4double GmTrajStepPosMom::GetDeltaAngle() const
{
  if( thePointToUse == TSPUPrePost ) {
    return GetMomentumDirectionPre().angle( GetMomentumDirectionPost() );
  } else {
    G4Exception("GmTrajStepPosMom::GetMomentumDirection",
		"Error",
		FatalException,
		"Asking when not Pre and Post variables are filled, it is not possible to know which to use. Please instantiate it with TSPUPrePost");
  }

  return 0.;

}
