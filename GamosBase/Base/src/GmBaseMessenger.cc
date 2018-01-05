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
#include "GmBaseMessenger.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmClassifierMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmDistributionMgr.hh"

#include "G4UImanager.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

//---------------------------------------------------------------------
GmBaseMessenger::GmBaseMessenger() 
{
  filterCmd = new GmUIcmdWithAString("/gamos/filter",this);
  filterCmd->SetGuidance("Create a new filter assigning parameters to one of the plug-in filters: FILTER_NAME FILTER_CLASS");
  filterCmd->SetParameterName("choice",true);
  filterCmd->AvailableForStates(G4State_Idle);

  classifierCmd = new GmUIcmdWithAString("/gamos/classifier",this);
  classifierCmd->SetGuidance("Create a new classifier assigning parameters to one of the plug-in classifiers: CLASSIFIER_NAME CLASSIFIER_CLASS");
  classifierCmd->SetParameterName("choice",true);
  classifierCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  classifierIndicesCmd = new GmUIcmdWithAString("/gamos/classifier/setIndices",this);
  classifierIndicesCmd->SetGuidance("Set indices for a classifier: CLASSIFIER_NAME KEY_1 VALUE_1 KEY_2 VALUE_2 ...");
  classifierIndicesCmd->SetParameterName("choice",true);
  classifierIndicesCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  distributionCmd = new GmUIcmdWithAString("/gamos/distribution",this);
  distributionCmd->SetGuidance("Create a new distribution assigning parameters to one of the plug-in distributions: DISTRIBUTION_NAME DISTRIBUTION_CLASS");
  distributionCmd->SetParameterName("choice",true);
  distributionCmd->AvailableForStates(G4State_Idle);
}

//---------------------------------------------------------------------
GmBaseMessenger::~GmBaseMessenger() 
{
  if (filterCmd) delete filterCmd;
  if (classifierCmd) delete classifierCmd;
  if (classifierIndicesCmd) delete classifierIndicesCmd;
  if (distributionCmd) delete distributionCmd;
}

//---------------------------------------------------------------------
void GmBaseMessenger::SetNewValue(G4UIcommand * command, G4String newValues)    
{ 
  if (command == filterCmd) {
    GmFilterMgr::GetInstance()->FindOrBuildFilter( GmGenUtils::GetWordsInString(newValues) );
  } else if (command == classifierCmd) {
    GmClassifierMgr::GetInstance()->FindOrBuildClassifier( GmGenUtils::GetWordsInString(newValues) );
    //?    GmClassifierMgr::GetInstance()->CreateClassifier( GmGenUtils::GetWordsInString(newValues) );
  } else if (command == classifierIndicesCmd) {
    GmClassifierMgr::GetInstance()->SetClassifierIndices( GmGenUtils::GetWordsInString(newValues) );
  } else if (command == distributionCmd) {
    GmDistributionMgr::GetInstance()->FindOrBuildDistribution( GmGenUtils::GetWordsInString(newValues) );
  }

}

