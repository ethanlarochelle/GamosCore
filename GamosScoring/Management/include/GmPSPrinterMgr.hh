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
#ifndef GmPSPrinterMgr_H
#define GmPSPrinterMgr_H

#include <map>
#include <vector>
#include "globals.hh"
class GmVPSPrinter;
// FindOrBuildPSPrinter : first looks if a PSPrinter with same name already exists (using GetPSPrinterFromList)
// if it dose not exists creates a new one (using CreatePSPrinter)
// CreatePSPrinter : it looks for a plug-in PSPrinter with the corresponding name and if it exits add parameters to it: when creating a PSPrinter from a user action command no parameters are passed, while creating it from '/gamos/PSPrinter' command parameters may exist 

class GmPSPrinterMgr
{
private:
  GmPSPrinterMgr();

public:
  ~GmPSPrinterMgr();

  static GmPSPrinterMgr* GetInstance();

  // for a user action it maybe a PSPrinter or an indexer
  GmVPSPrinter* CreatePSPrinter( std::vector<G4String> wl, G4bool bExists = true );

  GmVPSPrinter* FindOrBuildPSPrinter(std::vector<G4String> params, G4bool bExists = true );

  GmVPSPrinter* GetPSPrinterFromList(const G4String& PSPrinterName ) const;

private:
  void AddPSPrinter( GmVPSPrinter* PSPrinter );

  static GmPSPrinterMgr* theInstance;

  std::map<G4String,GmVPSPrinter*> thePSPrinters;

};

#endif
