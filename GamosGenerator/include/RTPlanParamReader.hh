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
#ifndef RTPlanParamReader_H
#define RTPlanParamReader_H
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include <map>

class RTPlanParamReader
{
public:
  RTPlanParamReader();
  ~RTPlanParamReader(){}

  void ReadParams( GmFileIn& fin);
  void CheckIfNewParameter( const std::vector<G4String>& wl,
			    G4bool bMustBeNew );
 
public:
  void SetParam(G4String parName, G4double val, G4bool bMustExist = false );
  void SetParamStr(G4String parName, G4String val, G4bool bMustExist = true );
  G4double GetParam(G4String parName, G4bool bMustExist = true) const;
  G4bool ParamExists(G4String parName) const;
  std::map<G4String,G4double> GetParams() const {
    return theParams;
  }
  std::map<G4String,G4String> GetParamStrs() const {
    return theParamStrs;
  }

protected:
  std::map<G4String,G4double> theParams;
  std::map<G4String,G4String> theParamStrs;

};

#endif // RTPlanParamReader_H
