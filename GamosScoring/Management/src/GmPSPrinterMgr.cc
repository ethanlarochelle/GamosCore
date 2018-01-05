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
#include "GmPSPrinterMgr.hh"

#include "GmVPSPrinter.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"

#include "Reflex/PluginService.h"

GmPSPrinterMgr* GmPSPrinterMgr::theInstance = 0;

//----------------------------------------------------------------------
GmPSPrinterMgr::GmPSPrinterMgr()
{
}

//----------------------------------------------------------------------
GmPSPrinterMgr* GmPSPrinterMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmPSPrinterMgr;
  }

  return theInstance;
}

//----------------------------------------------------------------------
GmPSPrinterMgr::~GmPSPrinterMgr()
{
}

//----------------------------------------------------------------------
GmVPSPrinter* GmPSPrinterMgr::CreatePSPrinter( std::vector<G4String> params, G4bool bExists )
{
  GmVPSPrinter* PSPrinter = Reflex::PluginService::Create<GmVPSPrinter*>(params[1],params[0]);

  if( PSPrinter != 0 ) {
    params.erase(params.begin()); 
    params.erase(params.begin());  
    
    PSPrinter->SetParameters( params );
    AddPSPrinter( PSPrinter );

  } else if( bExists ) {
    G4Exception(" GmPSPrinterMgr::CreatePSPrinter",
		"Wrong argument",
		FatalException,
		G4String(" PSPrinter type not found " + params[1] + " .Please check documentation and your '/gamos/scoring/printer' commands").c_str());
  }

  return PSPrinter;
}

//----------------------------------------------------------------------
void GmPSPrinterMgr::AddPSPrinter( GmVPSPrinter* PSPrinter )
{
  std::map<G4String,GmVPSPrinter*>::const_iterator ite = thePSPrinters.find( PSPrinter->GetName() );
  if( ite == thePSPrinters.end() ) {
    thePSPrinters[PSPrinter->GetName()] = PSPrinter;
  } else {
    G4Exception("GmPSPrinterMgr::AddPSPrinter",
" Adding two PSPrinters with the same name, review your '/gamos/scoring/printer' commands",FatalErrorInArgument,PSPrinter->GetName());
  }

}


//----------------------------------------------------------------------
GmVPSPrinter* GmPSPrinterMgr::FindOrBuildPSPrinter(std::vector<G4String> params, G4bool bExists )
{
  GmVPSPrinter* PSPrinter = GetPSPrinterFromList( params[0] );
  if( PSPrinter != 0 ) {
    return PSPrinter;
  } else {
    return CreatePSPrinter( params, bExists );
  }

}

//----------------------------------------------------------------------
GmVPSPrinter* GmPSPrinterMgr::GetPSPrinterFromList(const G4String& PSPrinterName ) const
{
  std::map<G4String,GmVPSPrinter*>::const_iterator ite = thePSPrinters.find( PSPrinterName );
  if( ite != thePSPrinters.end() ) {
    return (*ite).second;
  } else {
    return 0;
  }
}
