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
#include "GmNumberOfSecondariesFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

GmNumberOfSecondariesFilter::GmNumberOfSecondariesFilter(G4String name)
  :GmVFilter(name)
{
}

GmNumberOfSecondariesFilter::~GmNumberOfSecondariesFilter()
{ 
}

G4bool GmNumberOfSecondariesFilter::AcceptStep(const G4Step* )
{
  G4int nseco = GmG4Utils::GetSecondariesOfCurrentStep().size();
  if( nseco >= theNSecoMin && nseco <= theNSecoMax ) {
    return TRUE;
  }
  return FALSE;
}

void GmNumberOfSecondariesFilter::show()
{
  G4cout << "GmNumberOfSecondariesFilter: seco number MIN = " << theNSecoMin << " MAX = " << theNSecoMax << G4endl;
  G4cout << "-------------------------------------------"<<G4endl;
}

void GmNumberOfSecondariesFilter::SetParameters( std::vector<G4String>& params)
{
  if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmNumberOfSecondariesFilter::SetParameters","There should be two  parameters: min_seco_number max_seco_number",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
  }
  
  theNSecoMin = G4int(GmGenUtils::GetValue( params[0] ));
  theNSecoMax = G4int(GmGenUtils::GetValue( params[1] ));
  
}
