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
#include "G4ProcessAttribute.hh"
#include "G4ProcessManager.hh"

#include "GmVarianceReductionMessenger.hh"
#include "GmImportanceSamplingProcess.hh"
#include "GmWrapperProcess.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmDistributionMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVDistribution.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4ParticleTable.hh"

//-----------------------------------------------------------------------
GmVarianceReductionMessenger::GmVarianceReductionMessenger() 
{
  importanceSamplingCmd = new GmUIcmdWithAString("/gamos/physics/VR/importanceSampling",this);
  importanceSamplingCmd->SetGuidance("Sets on importance sampling");
  importanceSamplingCmd->SetParameterName("choice",true);
  importanceSamplingCmd->AvailableForStates(G4State_Idle);
  /*
  XSBiasingCmd = new GmUIcmdWithAString("/gamos/physics/VR/XSBiasing",this);
  XSBiasingCmd->SetGuidance("Multiplies cross section by a factor for given processes");
  XSBiasingCmd->SetParameterName("choice",true);
  XSBiasingCmd->AvailableForStates(G4State_Idle);*/
}

//-----------------------------------------------------------------------
GmVarianceReductionMessenger::~GmVarianceReductionMessenger() 
{
  if (importanceSamplingCmd) delete importanceSamplingCmd;
  //  if (XSBiasingCmd) delete XSBiasingCmd;
}

//-----------------------------------------------------------------------
void GmVarianceReductionMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{ 
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
  
  if (command == importanceSamplingCmd) {
    SetImportanceSampling( wl );
    //  } else if (command == XSBiasingCmd) {
    //    XSBiasing( wl );
  }
}


//-----------------------------------------------------------------------
void GmVarianceReductionMessenger::SetImportanceSampling(std::vector<G4String> params )
{
  if( params.size() < 3 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmVarianceReductionMessenger::SetImportanceSampling",
		"At least three arguments must be supplied in command '/gamos/physics/VR/importanceSampling' : NAME DISTRIBUTION_CLASS PARTICLE_NAME_1 PARTICLE_NAME_2 ...",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }

  G4String imporSampName = params[0];

  params.erase(params.begin());
  GmVDistribution* distribution = GmDistributionMgr::GetInstance()->FindOrBuildDistribution(params);

  GmImportanceSamplingProcess* ipproc = new GmImportanceSamplingProcess(imporSampName,distribution);

  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  std::vector<G4ParticleDefinition*>::const_iterator ite;
  for( unsigned int ii = 1; ii < params.size(); ii++ ){
    std::vector<G4ParticleDefinition*> parts = GmGetParticleMgr::GetInstance()->GetG4ParticleList( params[ii] );
    if( parts.size() == 0 ) {
      G4Exception("GmVarianceReductionMessenger::SetImportanceSampling",
		  "particle not found ", 
		  FatalErrorInArgument, 
		  params[ii]);
    }
    for( ite = parts.begin(); ite != parts.end(); ite++ ){
      G4ParticleDefinition* part = partTable->FindParticle( *ite);
      if( part == 0 ) {
	G4Exception("GmVarianceReductionMessenger::SetImportanceSampling","particle not found ", FatalErrorInArgument, params[ii]);
      } else {
	//	G4ProcessManager* procMgr = partTable->FindParticle( *ite )->GetProcessManager();
	G4ProcessManager* procMgr = part->GetProcessManager();
	procMgr->AddDiscreteProcess(ipproc,1);
      }
    }
  }
}

