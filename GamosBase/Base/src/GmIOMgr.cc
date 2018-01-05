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
#include "GmIOMgr.hh"
#include "GmBaseVerbosity.hh"
#include "GmParameterMgr.hh"

#include <iomanip>
#include <iostream>
#include "CLHEP/Units/SystemOfUnits.h"

//-----------------------------------------------------------------------
GmIOMgr::GmIOMgr()
{
}

//-----------------------------------------------------------------------
void GmIOMgr::InitDebugging()
{
  G4String fnout = "Output:"+theOutputType+":DebugIn";
  G4int debugInOld = GmParameterMgr::GetInstance()->GetNumericValue(fnout,-99);
  if( debugInOld != -99 ) {
    G4Exception("GmIOMgr::InitDebugging",
		"Old parameter format, please use the new one: delete 'Output:'",
		JustWarning,
		G4String("Parameter used: " + fnout).c_str());
  } else {
    debugInOld = 0;
  }

  fnout = theOutputType+":DebugIn";
  bDebugIn = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(fnout,G4bool(debugInOld)));

  fnout = "Output:"+theOutputType+":DebugOut";
  G4int debugOutOld = GmParameterMgr::GetInstance()->GetNumericValue(fnout,-99);
  if( debugOutOld != -99 ) {
    G4Exception("GmIOMgr::InitDebugging",
		"Old parameter format, please use the new one: delete 'Output:'",
		JustWarning,
		G4String("Parameter used: " + fnout).c_str());
  } else {
    debugOutOld = 0;
  }

  fnout = theOutputType+":DebugOut";
  bDebugOut = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(fnout,0));
  
}

//-----------------------------------------------------------------------
GmIOMgr::~GmIOMgr()
{
}

//-----------------------------------------------------------------------
void GmIOMgr::SetFileNameIn( const G4String& fname )
{
  if( fname != "" ) {
    theFileNameIn = fname;
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(infoVerb) ) G4cout << " GmIOMgr::SetFileNameIn " << theFileNameIn << G4endl;
#endif
  } else {

    G4String fnout = "Output:"+theOutputType+":FileNameIn";
    theFileNameIn = GmParameterMgr::GetInstance()->GetStringValue(fnout,"");

    if( theFileNameIn != "" ) {
      G4Exception("GmIOMgr::SetFileNameIn",
		"Old parameter format, please use the new one: delete 'Output:' and substitute 'FileNameIn' by 'FileName'",
		JustWarning,
		G4String("Parameter used: " + fnout).c_str());
    }else {
      theFileNameIn = theOutputType+".out";
    }

    fnout = theOutputType+":FileName";
    theFileNameIn = GmParameterMgr::GetInstance()->GetStringValue(fnout,theFileNameIn);

#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(infoVerb) ) G4cout << " GmIOMgr::SetFileNameIn " << theFileNameIn <<  " fnout " << fnout << G4endl;
#endif

  }

}


//-----------------------------------------------------------------------
void GmIOMgr::SetFileNameOut( const G4String& fname )
{
  if( fname != "" ) {
    theFileNameOut = fname;
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(infoVerb) ) G4cout << " GmIOMgr::SetFileNameOut " << theFileNameOut << G4endl;
#endif
  } else {
    G4String fnout = "Output:"+theOutputType+":FileNameOut";
    theFileNameOut = GmParameterMgr::GetInstance()->GetStringValue(fnout,"");

    if( theFileNameOut != "" ) {
      G4Exception("GmIOMgr::SetFileNameOut",
		"Old parameter format, please use the new one: delete 'Output:' and substitute 'FileNameOut' by 'FileName'",
		JustWarning,
		G4String("Parameter used: " + fnout).c_str());
    }else {
      theFileNameOut = theOutputType+".out";
    }

    fnout = theOutputType+":FileName";
    theFileNameOut = GmParameterMgr::GetInstance()->GetStringValue(fnout,theFileNameOut);

    G4String prefix = GmParameterMgr::GetInstance()->GetStringValue("GmAnalysisMgr:FileNamePrefix","");
    theFileNameOut = prefix + theFileNameOut;

    G4String suffix = GmParameterMgr::GetInstance()->GetStringValue("GmAnalysisMgr:FileNameSuffix","");
    theFileNameOut += suffix;

#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(infoVerb) ) G4cout << "  GmIOMgr::SetFileNameOut " << theFileNameOut << G4endl;
#endif

  }

}

//-----------------------------------------------------------------------
void GmIOMgr::PrintCart(const G4String& text, const G4ThreeVector& pos )
{
  G4cout << text << " X_Y_Z: " << pos.x() << " " << pos.y() << " " << pos.z(); 
}

//-----------------------------------------------------------------------
void GmIOMgr::PrintCyl(const G4String& text, const G4ThreeVector& pos )
{
  G4cout << text << " R2_PHI_Z: " << pos.perp() << " " << pos.phi()/CLHEP::deg << " " << pos.z();

}

//-----------------------------------------------------------------------
void GmIOMgr::PrintSphe(const G4String& text, const G4ThreeVector& pos )
{
  G4cout << text << " R_THETA_PHI: " << pos.mag() << " " << pos.theta()/CLHEP::deg << " " << pos.phi()/CLHEP::deg;

}
