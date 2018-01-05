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
#include "GmVClassifier.hh"
#include "G4UIcommand.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//---------------------------------------------------------
GmVClassifier::GmVClassifier(G4String name ) : theName( name )
{
  theMaxIndex = 0;
  bIsCompound = FALSE;

  bOnSecondary = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(name+":OnSecondary",0));

}

//---------------------------------------------------------
G4String GmVClassifier::GetIndexName(G4int index )
{
  return G4UIcommand::ConvertToString( G4int(index) );
}

//---------------------------------------------------------
G4int GmVClassifier::GetIndexFromTrack(const G4Track*)
{
  return 0;
}

#include "G4Track.hh" //GDEB
//---------------------------------------------------------
G4int GmVClassifier::GetIndexFromSecoTrack(const G4Track*, const G4Track* aTrack2)
{
  return GetIndexFromTrack(aTrack2);
}

//---------------------------------------------------------
void GmVClassifier::CheckNumberOfIndices( std::vector<G4String> wl )
{
  if( wl.size()%2 == 1 ) {
    G4Exception("GmVClassifier::SetIndices",
		("Wrong number of parameters in classifier " + theName).c_str(),
		FatalErrorInArgument,
		"There should be an even number of parameters after classifier name in command /gamos/base/setClassifierIndices : KEY_1 INDEX_2 KEY_2 INDEX_2 ...");
  }
}
