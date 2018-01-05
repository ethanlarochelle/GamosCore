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
#include "GmVDataInt.hh"
#include "GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------
GmVDataInt::GmVDataInt()
{
  theNBytes = 4; // sizeof(int)
  theCType = "int";
}

//----------------------------------------------------------------
GmVDataInt::~GmVDataInt()
{
}

//----------------------------------------------------------------
void GmVDataInt::WriteText( const G4Step* aStep, G4bool bIsFirst )
{
  long unsigned int data = (long unsigned int) (GetValueFromStep( aStep, 0 ));
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteText Step " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataInt::WriteText( const G4Track* aTrack, G4bool bIsFirst )
{
  long unsigned int data = (long unsigned int) (GetValueFromTrack( aTrack, 0 ));
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteText Track " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataInt::WriteText( const G4Track* aTrack1, const G4Track* aTrack2, G4bool bIsFirst )
{
  G4double data = GetValueFromSecoTrack( aTrack1, aTrack2, 0 );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteText SecoTrack " << theName << " = " << data << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVDataInt::WriteText( const G4Event* anEvent, G4bool bIsFirst )
{
  G4double data = GetValueFromEvent( anEvent, 0 );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteText Event " << theName << " = " << data << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVDataInt::WriteText( const G4Run* aRun, G4bool bIsFirst )
{
  G4double data = GetValueFromRun( aRun, 0 );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteText Run " << theName << " = " << data << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVDataInt::WriteBin( const G4Step* aStep )
{
  int data = int(GetValueFromStep( aStep ));
  theFileOutBin->write((char*)&data, sizeof(int));

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteBin Step " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataInt::WriteBin( const G4Track* aTrack )
{
  int data = int(GetValueFromTrack( aTrack ));
  theFileOutBin->write((char*)&data, sizeof(int));
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteBin Track " << theName << " = " << data << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVDataInt::WriteBin( const G4Track* aTrack1, const G4Track* aTrack2 )
{
  int data = int(GetValueFromSecoTrack( aTrack1, aTrack2 ));
  theFileOutBin->write((char*)&data, sizeof(int));
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteBin SecondaryTrack " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataInt::WriteBin( const G4Event* aEvent )
{
  int data = int(GetValueFromEvent( aEvent ));
  theFileOutBin->write((char*)&data, sizeof(int));
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteBin Event " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataInt::WriteBin( const G4Run* aRun )
{
  int data = int(GetValueFromRun( aRun ));
  theFileOutBin->write((char*)&data, sizeof(int));
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteBin Run " << theName << " = " << data << G4endl;
#endif

}
