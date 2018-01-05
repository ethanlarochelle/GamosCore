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

#include "GmEventClassifierByGammaInteraction.hh"
#include "GmTrajPoint.hh"
#include "GmTrajStep.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

//---------------------------------------------------------------
GmEventClassifierByGammaInteraction::GmEventClassifierByGammaInteraction()
{
  bProcessNamesBuilt = FALSE;
}

//---------------------------------------------------------------
GmEventClassifierByGammaInteraction::~GmEventClassifierByGammaInteraction()
{
}

//---------------------------------------------------------------
int GmEventClassifierByGammaInteraction::Classify( const std::vector<GmTrajPoint*>& interactionList )
{
  //  G4cout << " GmEventClassifierByGammaInteraction::Classify " << interactionList.size() << G4endl;
  int type = 0;

  std::vector<GmTrajPoint*>::const_iterator ite;
  for( ite = interactionList.begin(); ite != interactionList.end(); ite++ ){ 
    type += Classify1Interaction( (*ite)->GetProcessName() );
  }

  return type;

}

//---------------------------------------------------------------
void GmEventClassifierByGammaInteraction::BuildProcessNames()
{
  /*  thePhotoElecName = GmGetParticleMgr::GetInstance()->GetG4ProcessName("gamma","Phot");
  theComptonName = GmGetParticleMgr::GetInstance()->GetG4ProcessName("gamma","Compt");
  theRayleighName = GmGetParticleMgr::GetInstance()->GetG4ProcessName("gamma","Rayl");
  theConversionName = GmGetParticleMgr::GetInstance()->GetG4ProcessName("gamma","Conv");*/
  thePhotoElecName = "phot"; 
  theComptonName = "compt";
  theRayleighName = "Rayl";
  theConversionName = "conv";
  bProcessNamesBuilt = TRUE;
}

//---------------------------------------------------------------
int GmEventClassifierByGammaInteraction::Classify( const std::vector<GmTrajStep*>& interactionList )
{
  //  G4cout << " GmEventClassifierByGammaInteraction::Classify " << interactionList.size() << G4endl;
  if( !bProcessNamesBuilt ) {
    BuildProcessNames();
  }

  int type = 0;

  std::vector<GmTrajStep*>::const_iterator ite;
  for( ite = interactionList.begin(); ite != interactionList.end(); ite++ ){ 
    type += Classify1Interaction( (*ite)->GetProcessName() );
  }

  return type;

}

//---------------------------------------------------------------
int GmEventClassifierByGammaInteraction::Classify1Interaction( const G4String& processName )
{
  int type = 0;

  if( processName == thePhotoElecName ){ 
    type = 10000;

  }else if( processName == theComptonName ){
    type = 100;

    if( type >= 10000 ) {
      G4Exception("GmEventClassifierByGammaInteraction::Classify1Interaction",
		  "Error",
		  FatalException,
		  "More than 100 LowEnergyGompton  interactions " );
    }
    
  }else if( processName == theRayleighName ){ 
    type = 1;
    
  }else if( processName == theConversionName ){ 

  }else if( processName == "Transportation" ){ 

  }else {
    if( processName != "Rayl" ) G4Exception( "GmEventClassifierByGammaInteraction::Classify1Interaction",
					     "Warning",
					     JustWarning,
					     G4String(" Process not found" + processName).c_str() );
  }
   
  return type;
}

//---------------------------------------------------------------
G4String GmEventClassifierByGammaInteraction::GetClassificationName( int ic ) 
{

  G4String name = "Evt:";

  int iPE = ic/10000;
  int iCOMP= (ic/100)%100;
  int iRAYL = ic%100;

  //  G4cout << " Class name " << " iCOMP  " << iCOMP << " iPE " << iPE << G4endl;
  name += "PE";
  name += GmGenUtils::itoa(iPE);

  name += "COMP";
  name += GmGenUtils::itoa(iCOMP);

  if( iRAYL > 0 ){
    name += "RAYL";
    name += GmGenUtils::itoa(iRAYL);
  }

  return name;
}
