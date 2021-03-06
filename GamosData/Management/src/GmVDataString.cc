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
#include "GmVDataString.hh"
#include "GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------
GmVDataString::GmVDataString()
{
  theCType = "char";
  theNChar = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":NumberOfChars",theNChar));
}

//----------------------------------------------------------------
GmVDataString::~GmVDataString()
{
}


//----------------------------------------------------------------
void GmVDataString::BookHisto( G4String , G4int , G4int, GmAnalysisMgr* )
{

  G4Exception("GmVDataString::BookHisto",
	      "A string data cannot be used in an histogram",
	      FatalErrorInArgument,
	      G4String("Data= " + theName).c_str());
  
}


//----------------------------------------------------------------
void GmVDataString::FillHisto( const G4Step*, G4int )
{

  G4Exception("GmVDataString::FillHisto",
	      "A string data cannot be used in an histogram",
	      FatalErrorInArgument,
	      G4String("Data= " + theName).c_str());

}

//----------------------------------------------------------------
void GmVDataString::FillHisto( const G4Track*, G4int )
{

  G4Exception("GmVDataString::FillHisto",
	      "A string data cannot be used in an histogram",
	      FatalErrorInArgument,
	      G4String("Data= " + theName).c_str());

}

//----------------------------------------------------------------
void GmVDataString::FillHisto( const G4Track*, const G4Track*, G4int )
{ 
  G4Exception("GmVDataString::FillHisto",
	      "A string data cannot be used in an histogram",
	      FatalErrorInArgument,
	      G4String("Data= " + theName).c_str());

}

//----------------------------------------------------------------
void GmVDataString::FillHisto( const G4Event*, G4int )
{

  G4Exception("GmVDataString::FillHisto",
	      "A string data cannot be used in an histogram",
	      FatalErrorInArgument,
	      G4String("Data= " + theName).c_str());

}
//----------------------------------------------------------------
void GmVDataString::FillHisto( const G4Run*, G4int )
{

  G4Exception("GmVDataString::FillHisto",
	      "A string data cannot be used in an histogram",
	      FatalErrorInArgument,
	      G4String("Data= " + theName).c_str());

}


//----------------------------------------------------------------
void GmVDataString::WriteText( const G4Step* aStep, G4bool bIsFirst )
{
  G4String data = GetStringValueFromStep( aStep );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << "\"" << data << "\"";

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataString::WriteText " << theName << " " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataString::WriteText( const G4Track* aTrack, G4bool bIsFirst )
{

  G4String data = GetStringValueFromTrack( aTrack );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << "\"" << data << "\"";

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataString::WriteText " << theName << " " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataString::WriteText( const G4Track* aTrack1, const G4Track* aTrack2, G4bool bIsFirst )
{
  G4String data = GetStringValueFromSecoTrack( aTrack1, aTrack2 );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << "\"" << data << "\"";

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataString::WriteText " << theName << " " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataString::WriteText( const G4Event* aEvent, G4bool bIsFirst )
{

  G4String data = GetStringValueFromEvent( aEvent );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << "\"" << data << "\"";

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataString::WriteText " << theName << " " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataString::WriteText( const G4Run* aRun, G4bool bIsFirst )
{

  G4String data = GetStringValueFromRun( aRun );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << "\"" << data << "\"";

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataString::WriteText " << theName << " " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataString::WriteBin( const G4Step* aStep )
{
  G4String data = GetStringValueFromStep( aStep );
  theFileOutBin->write(data.c_str(), sizeof(char)*theNChar);

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataString::WriteBin " << theName << " " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataString::WriteBin( const G4Track* aTrack )
{
  G4String data = GetStringValueFromTrack( aTrack );
  theFileOutBin->write(data.c_str(), sizeof(char)*theNChar);

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataString::WriteBin " << theName << " " << data << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVDataString::WriteBin( const G4Track* aTrack1, const G4Track* aTrack2 )
{
  G4String data = GetStringValueFromSecoTrack( aTrack1, aTrack2 ).c_str();
  theFileOutBin->write(data.c_str(), sizeof(char)*theNChar);

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataString::WriteBin " << theName << " " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataString::WriteBin( const G4Event* aEvent )
{

  G4String data = GetStringValueFromEvent( aEvent );
  theFileOutBin->write(data.c_str(), sizeof(char)*theNChar);

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataString::WriteBin " << theName << " " << data << G4endl;
#endif

}

void GmVDataString::WriteBin( const G4Run* aRun )
{

  G4String data = GetStringValueFromRun( aRun );
  theFileOutBin->write(data.c_str(), sizeof(char)*theNChar);

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataString::WriteBin " << theName << " " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataString::WriteCout( const G4Step* aStep, G4bool bIsFirst )
{
  G4String data = GetStringValueFromStep( aStep );
  if( !bIsFirst ) G4cout << ",";
  G4cout << "\"" << data << "\"";

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataString::WriteCout " << theName << " " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataString::WriteCout( const G4Track* aTrack, G4bool bIsFirst )
{

  G4String data = GetStringValueFromTrack( aTrack );
  if( !bIsFirst ) G4cout << ",";
  G4cout << "\"" << data << "\"";

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataString::WriteCout " << theName << " " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataString::WriteCout( const G4Track* aTrack1, const G4Track* aTrack2, G4bool bIsFirst )
{
  G4String data = GetStringValueFromSecoTrack( aTrack1, aTrack2 );
  if( !bIsFirst ) G4cout << ",";
  G4cout << "\"" << data << "\"";

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataString::WriteCout " << theName << " " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataString::WriteCout( const G4Event* aEvent, G4bool bIsFirst )
{

  G4String data = GetStringValueFromEvent( aEvent );
  if( !bIsFirst ) G4cout << ",";
  G4cout << "\"" << data << "\"";

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataString::WriteCout " << theName << " " << data << G4endl;
#endif

}
//----------------------------------------------------------------
void GmVDataString::WriteCout( const G4Run* aRun, G4bool bIsFirst )
{

  G4String data = GetStringValueFromRun( aRun );
  if( !bIsFirst ) G4cout << ",";
  G4cout << "\"" << data << "\"";

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataString::WriteCout " << theName << " " << data << G4endl;
#endif

}

//----------------------------------------------------------------
G4String GmVDataString::GetStringValueFromStep( const G4Step* )
{
  G4Exception("GmVDataString::GetStringValue(aTrack)",
	      "This method should not be called, check implementation of data class",
	      FatalErrorInArgument,
	      G4String("DATA= "+theName).c_str()); 
  return "";
}
  
//----------------------------------------------------------------
G4String GmVDataString::GetStringValueFromTrack( const G4Track* )
{
  G4Exception("GmVDataString::GetStringValue(aTrack)",
	      "This method should not be called, check implementation of data class",
	      FatalErrorInArgument,
	      G4String("DATA= "+theName).c_str()); 
  return "";
}

//----------------------------------------------------------------
G4String GmVDataString::GetStringValueFromSecoTrack( const G4Track* , const G4Track* )
{
  G4Exception("GmVDataString::GetStringValue(aTrack1,aTrack2)",
	      "This method should not be called, check implementation of data class",
	      FatalErrorInArgument,
	      G4String("DATA= "+theName).c_str()); 
  return "";
}

//----------------------------------------------------------------
G4String GmVDataString::GetStringValueFromEvent( const G4Event* )
{
  G4Exception("GmVDataString::GetStringValue(aTrack)",
	      "This method should not be called, check implementation of data class",
	      FatalErrorInArgument,
	      G4String("DATA= "+theName).c_str()); 
  return "";
}

//----------------------------------------------------------------
G4String GmVDataString::GetStringValueFromRun( const G4Run* )
{
  G4Exception("GmVDataString::GetStringValue(aTrack)",
	      "This method should not be called, check implementation of data class",
	      FatalErrorInArgument,
	      G4String("DATA= "+theName).c_str()); 
  return "";
}


