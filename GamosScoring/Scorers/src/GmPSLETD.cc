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
//#define VERBOSE_DOSEDEP
#include "GmPSLETD.hh"
#include "G4VPrimitiveScorer.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "Reflex/PluginService.h"

//--------------------------------------------------------------------
GmPSLETD::GmPSLETD(G4String name) 
  :GmCompoundScorer(name)
{ 
  theUnit = 1;
  theUnitName = "CLHEP::MeV/CLHEP::mm";

  bInitialized = false;
  theNEventsType = SNET_ByNFilled;

}


//--------------------------------------------------------------------
G4bool GmPSLETD::ProcessHits(G4Step* aStep,G4TouchableHistory* th)
{
  GmCompoundScorer::ProcessHits(aStep,th);  
  return TRUE;
} 

//--------------------------------------------------------------------
void GmPSLETD::SetParameters( const std::vector<G4String>& params)
{
  if( params.size() != 0 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSLETD::SetParameters",
		"There should no parameters",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
  }

  std::vector<G4String> paramsNC;
  paramsNC.push_back("GmPSEdep_LET/GmG4PSEnergyDeposit");

  GmCompoundScorer::SetParameters(paramsNC);
}

