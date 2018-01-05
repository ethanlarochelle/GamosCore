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
#include "GmStringDistribution.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

//---------------------------------------------------------------------
GmStringDistribution::GmStringDistribution(G4String name)
  : GmVStringDistribution(name)
{
  BuildData();
}

//---------------------------------------------------------------------
G4double GmStringDistribution::GetValueFromStep(const G4Step* aStep)
{

  if( !theData ) { 
    G4Exception("GmVStringDistribution::GetValueFromStep",
		"Data type is not defined",
		FatalException,
		G4String("Use command /gamos/setParam " + theName + ":Data").c_str());
  } 

  G4String indexVal = theData->GetStringValueFromStep( aStep );

  return GetStringValueFromIndex( indexVal );
}

