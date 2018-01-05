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
#ifndef GmFilterMgr_hh
#define GmFilterMgr_hh

#include "G4RunManager.hh"
#include <map>
class GmVFilter;
// FindOrBuildFilter : first looks if a filter with same name already exists (using GetFilterFromList)
// if it dose not exists creates a new one (using CreateFilter)
// CreateFilter : it looks for a plug-in filter with the corresponding name and if it exits add parameters to it: when creating a filter from a user action command no parameters are passed, while creating it from '/gamos/filter' command parameters may exist 

class GmFilterMgr
{
private:
  GmFilterMgr();

public:
  ~GmFilterMgr();

  static GmFilterMgr* GetInstance();

  // for a user action it maybe a filter or an indexer
  GmVFilter* CreateFilter( std::vector<G4String> wl, G4bool bExists = true );

  GmVFilter* FindOrBuildFilter(std::vector<G4String> params, G4bool bExists = true );

  GmVFilter* GetFilterFromList(const G4String& filterName ) const;

  void DeleteFilters();

private:
  void AddFilter( GmVFilter* filter );

  static GmFilterMgr* theInstance;

  std::map<G4String,GmVFilter*> theFilters;

};

#endif
