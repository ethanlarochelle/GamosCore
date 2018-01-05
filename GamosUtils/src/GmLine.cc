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
#include "GamosCore/GamosUtils/include/GmLine.hh"

//--------------------------------------------------------------------
GmLine::GmLine(const G4ThreeVector& pnt, const G4ThreeVector& dir): thePoint(pnt)
{
  if( dir.mag() == 0. ) {
    G4Exception("GmLine::GmLine",
		"Wrong internal argument",
		FatalException,
		"trying to create a line with direction = (0.,0.,0.)");
  }
  theDir = dir.unit();
  // G4cout << " Created GmLine pnt " << thePoint << " dir " << theDir << G4endl;
}


//--------------------------------------------------------------------
G4double GmLine::GetDist( const G4ThreeVector& pt ) const
{
  G4ThreeVector vect = pt - thePoint;
  //  G4cout << " GmLine::GetDist enter " << " pt " << pt << " " << *this << G4endl;
  if( vect.mag() == 0. ) return 0.;
  G4double ang = acos( theDir * vect.unit() ); 
  //  G4cout << " GmLine::GetDist " << vect.mag() * sin( ang ) << " pt " << pt << " " << " ang " << ang << *this << G4endl;
  return vect.mag() * sin( ang );

}


//--------------------------------------------------------------------
G4ThreeVector GmLine::GetDistPoint( const G4ThreeVector& pt ) const
{
  G4ThreeVector vect = pt - thePoint;
  G4double ang = acos( theDir * vect.unit() ); 
  return thePoint + theDir * vect.mag() * cos( ang );
}


//--------------------------------------------------------------------
std::ostream& operator<<( std::ostream& os, const GmLine& lin )
{
  os << " GmLine: Point= " << lin.thePoint << "  Dir= " << lin.theDir;
  return os;

}
