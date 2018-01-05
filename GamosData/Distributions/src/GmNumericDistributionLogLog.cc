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
#include "GmNumericDistributionLogLog.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

//---------------------------------------------------------------------
GmNumericDistributionLogLog::GmNumericDistributionLogLog(G4String name)
  : GmVNumericDistribution(name)
{
}


//---------------------------------------------------------------------
G4double GmNumericDistributionLogLog::InterpolateValue(const G4double& indexVal, const G4double& indexDown, const G4double& indexUp, const G4double& valDown, const G4double& valUp )
{
  G4double logIndexDown = log(indexDown);
  G4double value = exp( log(valDown) + (log(indexVal)-logIndexDown)/(log(indexUp)-logIndexDown)*(log(valUp)-log(valDown)));
#ifndef GAMOS_NO_VERB
  if( DistVerb(debugVerb) ) G4cout << " GmNumericDistributionLogLog::InterpolateValue " << value 
				   << " valDown= " << valDown << " valUp= " << valUp 
				   << " logIndexDown= " << logIndexDown << " logIndexUp= " << log(indexUp) << G4endl;
#endif
  return value;

}

