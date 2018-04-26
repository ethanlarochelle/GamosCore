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
#include "GmBOptrForceCollisionTrackData.hh"
#include "GmBOptrForceCollision.hh"

GmBOptrForceCollisionTrackData::GmBOptrForceCollisionTrackData( const GmBOptrForceCollision* optr )
: G4VAuxiliaryTrackInformation(),
  fForceCollisionOperator( optr )
{
  fForceCollisionState = ForceCollisionState::free;
}

GmBOptrForceCollisionTrackData::~GmBOptrForceCollisionTrackData()
{
  if ( fForceCollisionState != ForceCollisionState::free )
    {
      G4ExceptionDescription ed;
      ed << "Track deleted while under GmBOptrForceCollision biasing scheme of operator `";
      if ( fForceCollisionOperator == nullptr ) ed << "(none)"; else ed << fForceCollisionOperator->GetName();
      ed <<"'. Will result in inconsistencies.";
      G4Exception(" GmBOptrForceCollisionTrackData::~GmBOptrForceCollisionTrackData()",
		  "BIAS.GEN.19",
		  JustWarning,
		  ed);
    }
}

void GmBOptrForceCollisionTrackData::Print() const
{
  G4cout << " GmBOptrForceCollisionTrackData object : " << this << G4endl;
  G4cout << "     Force collision operator : "; if ( fForceCollisionOperator == nullptr ) G4cout << "(none)"; else G4cout << fForceCollisionOperator->GetName(); G4cout << G4endl;
  G4cout << "     Force collision state    : ";
  switch ( fForceCollisionState )
    {
    case ForceCollisionState::free :
      G4cout << "free from biasing ";
      break;
    case ForceCollisionState::toBeCloned :
      G4cout << "to be cloned ";
      break;
    case ForceCollisionState::toBeForced :
      G4cout << "to be interaction forced ";
      break;
    case ForceCollisionState::toBeFreeFlight :
      G4cout << "to be free flight forced (under weight = 0) ";
      break;
    default:
      break;
    }
  G4cout << G4endl;
}
