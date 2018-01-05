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
#include "GmUserAction.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmClassifierMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "globals.hh"

//-----------------------------------------------------------------------
GmUserAction::GmUserAction()
{
  isEnabled = TRUE;
  theClassifier = 0;

  bApplyFiltersToStepping = TRUE;
  bApplyFiltersToPreTracking = TRUE;
  bApplyFiltersToPostTracking = TRUE;
  bApplyFiltersToStacking = TRUE;

  bCheckAllFiltersAtStepping = false;
  bCheckAllFiltersAtPreTracking = false;
  bCheckAllFiltersAtPostTracking = false;
  bCheckAllFiltersAtStacking = false;
}

//-----------------------------------------------------------------------
GmUserAction::~GmUserAction() 
{
}
//-----------------------------------------------------------------------
void GmUserAction::SetFilterOrClassifier(const G4String& objectName)
{
  std::vector<G4String> params;
  params.push_back( objectName );
  params.push_back( objectName );

  GmVFilter* filter = GmFilterMgr::GetInstance()->FindOrBuildFilter(params, false );
  GmVClassifier* classifier = 0;
  if( filter ) {
    theFilters.push_back(filter);
    theName += "_" + filter->GetName();
  } else {
    classifier = GmClassifierMgr::GetInstance()->FindOrBuildClassifier(params, false );
    if( classifier ) {
      if( theClassifier ) {
	G4Exception("GmUserAction::SetFilterOrClassifier ",
		    "More than one classifier, use GmCompoundClassifier",
		    FatalErrorInArgument,
		    G4String("Classifier= "+objectName+" to user action= " + theName).c_str());
      }
      theClassifier = classifier;
      theName += "_" + classifier->GetName();
    }
  }
  
  if( !filter && !classifier ) {
    G4Exception("GmUserAction::SetFilterOrClassifier ","No filter or classifier found",FatalErrorInArgument
		,G4String("Filter/Classifier= "+objectName+" to user action= " + theName).c_str());
  }

  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  bApplyFiltersToStepping = G4bool(paramMgr->GetNumericValue(theName+":ApplyFiltersToStepping", bApplyFiltersToStepping));
  bApplyFiltersToPreTracking = G4bool(paramMgr->GetNumericValue(theName+":ApplyFiltersToPreTracking", bApplyFiltersToPreTracking));
  bApplyFiltersToPostTracking = G4bool(paramMgr->GetNumericValue(theName+":ApplyFiltersToPostTracking", bApplyFiltersToPostTracking));
  bApplyFiltersToStacking = G4bool(paramMgr->GetNumericValue(theName+":ApplyFiltersToStacking", bApplyFiltersToStacking));


  bCheckAllFiltersAtStepping = G4bool(paramMgr->GetNumericValue(theName+":CheckAllFiltersAtStepping", bApplyFiltersToStepping));
  bCheckAllFiltersAtPreTracking = G4bool(paramMgr->GetNumericValue(theName+":CheckAllFiltersAtPreTracking", bApplyFiltersToPreTracking));
  bCheckAllFiltersAtPostTracking = G4bool(paramMgr->GetNumericValue(theName+":CheckAllFiltersAtPostTracking", bApplyFiltersToPostTracking));
  bCheckAllFiltersAtStacking = G4bool(paramMgr->GetNumericValue(theName+":CheckAllFiltersAtStacking", bApplyFiltersToStacking));

}
