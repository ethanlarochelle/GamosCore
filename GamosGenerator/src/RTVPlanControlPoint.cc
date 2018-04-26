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
#include "RTVPlanControlPoint.hh"
#include "GmGenerVerbosity.hh"

//-----------------------------------------------------------------------
G4bool RTVPlanControlPoint::operator==( const RTVPlanControlPoint& right ) const 
{
  if( GetParam("DoseRateSet") == right.GetParam("DoseRateSet") &&
      GetParam("GantryAngle") == right.GetParam("GantryAngle") &&
      GetParam("IsocenterPosition_X") == right.GetParam("IsocenterPosition_X") &&
      GetParam("IsocenterPosition_Y") == right.GetParam("IsocenterPosition_Y") &&
      GetParam("IsocenterPosition_Z") == right.GetParam("IsocenterPosition_Z") &&
      GetParam("LimitingDeviceAngle") == right.GetParam("LimitingDeviceAngle") &&
      GetParam("NominalBeamEnergy") == right.GetParam("NominalBeamEnergy") &&
      GetParam("PatientSupportAngle") == right.GetParam("PatientSupportAngle") &&
      GetParam("SourceToSurfaceDistance") == right.GetParam("SourceToSurfaceDistance") &&
      GetParam("TableTopEccentricAngle") == right.GetParam("TableTopEccentricAngle") ) {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) G4cout << " RTVPlanControlPoint::operator== RETURNS TRUE " << G4endl;
#endif
    
    return true;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) {
      G4cout << " RTVPlanControlPoint::operator== RETURNS FALSE " << G4endl; 
      G4cout << " FIRST CONTROL POINT: " << *this << G4endl; 
      G4cout << " SECOND CONTROL POINT: " << right << G4endl;
    }
#endif
    return false;
  }
}

std::ostream& operator<<(std::ostream& out, const RTVPlanControlPoint& cp)
{
  std::map<G4String,G4String>::const_iterator ites;
  std::map<G4String,G4String> paramStrs = cp.GetParamStrs();
  out << " N PARAMS str " << paramStrs.size() << G4endl;
  for(ites = paramStrs.begin(); ites != paramStrs.end(); ites++ ) {
    out << (*ites).first << " " << (*ites).second << G4endl;
  }

  std::map<G4String,G4double>::const_iterator ited;
  std::map<G4String,G4double> params = cp.GetParams();
  out << " N PARAMS " << params.size() << G4endl;
  for(ited = params.begin(); ited != params.end(); ited++ ) {
    out << (*ited).first << " " << (*ited).second << G4endl;
  }
  out << " ENDED " << G4endl;

  return out;
}
