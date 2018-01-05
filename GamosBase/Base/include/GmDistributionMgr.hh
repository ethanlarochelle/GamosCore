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
#ifndef GmDistributionMgr_hh
#define GmDistributionMgr_hh

#include "G4RunManager.hh"
#include <map>
class GmVDistribution;
// FindOrBuildDistribution : first looks if a filter with same name already exists (using GetDistributionFromList)
// if it dose not exists creates a new one (using CreateDistribution)
// CreateDistribution : it looks for a plug-in filter with the corresponding name and if it exits add parameters to it: when creating a filter from a user action command no parameters are passed, while creating it from '/gamos/filter' command parameters may exist 

class GmDistributionMgr
{
private:
  GmDistributionMgr();

public:
  ~GmDistributionMgr();

  static GmDistributionMgr* GetInstance();

  // for a user action it maybe a filter or an indexer
  GmVDistribution* CreateDistribution( std::vector<G4String> wl, G4bool bExists = true );

  GmVDistribution* FindOrBuildDistribution(std::vector<G4String> params, G4bool bExists = true );

  GmVDistribution* GetDistributionFromList(const G4String& filterName ) const;

  void DeleteDistributions();

private:
  void AddDistribution( GmVDistribution* filter );

  static GmDistributionMgr* theInstance;

  std::map<G4String,GmVDistribution*> theDistributions;

};

#endif
