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
#include "GmStepNumberFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"

GmStepNumberFilter::GmStepNumberFilter(G4String name)
  :GmVFilter(name)
{
}

GmStepNumberFilter::~GmStepNumberFilter()
{ 
}

G4bool GmStepNumberFilter::AcceptStep(const G4Step* aStep)
{
  G4int nstep = aStep->GetTrack()->GetCurrentStepNumber();
  if( nstep >= theNStepMin && nstep <= theNStepMax ) {
    return TRUE;
  }
  return FALSE;
}

void GmStepNumberFilter::show()
{
  G4cout << "GmStepNumberFilter: step number MIN = " << theNStepMin << " MAX = " << theNStepMax << G4endl;
  G4cout << "-------------------------------------------"<<G4endl;
}

void GmStepNumberFilter::SetParameters( std::vector<G4String>& params)
{
  if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmStepNumberFilter::SetParameters","There should be two  parameters: min_step_number max_step_number",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
  }
  
  theNStepMin = G4int(GmGenUtils::GetValue( params[0] ));
  theNStepMax = G4int(GmGenUtils::GetValue( params[1] ));
  
}
