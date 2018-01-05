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
#include "GmVDataAccumulated.hh"
#include "GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmVDataAccumulated::GmVDataAccumulated()
{
  bAccumulating = TRUE;

}

//----------------------------------------------------------------
void GmVDataAccumulated::Initialise()
{
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmVDataAccumulated::Initialise data " << theName << G4endl; 
#endif
  theDataAccumulated.clear();
}

//----------------------------------------------------------------
GmVDataAccumulated::~GmVDataAccumulated()
{

}

//----------------------------------------------------------------
G4double GmVDataAccumulated::GetValueFromTrack( const G4Track*, G4int index )
{
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << theName << "  GmVDataAccumulated::GetValueFromTrack " << index << " = " << theDataAccumulated[index] << G4endl;
#endif 
  return theDataAccumulated[index];
}

//----------------------------------------------------------------
G4double GmVDataAccumulated::GetValueFromSecoTrack( const G4Track* , const G4Track* , G4int index)
{
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << theName << "  GmVDataAccumulated::GetValueFromSecoTrack " << index << " = " << theDataAccumulated[index] << G4endl;
#endif 
  return theDataAccumulated[index];
}

//----------------------------------------------------------------
G4double GmVDataAccumulated::GetValueFromEvent( const G4Event*, G4int index )
{
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << theName << "  GmVDataAccumulated::GetValueFromEvent " << index << " = " << theDataAccumulated[index] << G4endl;
#endif 
  return theDataAccumulated[index];
}

//----------------------------------------------------------------
G4double GmVDataAccumulated::GetValueFromRun( const G4Run*, G4int index )
{
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << theName << "  GmVDataAccumulated::GetValueFromRun " << index << " = " << theDataAccumulated[index] << G4endl;
#endif 
  return theDataAccumulated[index];
}
