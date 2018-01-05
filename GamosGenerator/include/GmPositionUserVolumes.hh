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
#ifndef GmPositionUserVolumes_HH
#define GmPositionUserVolumes_HH

#include <map>
class G4VSolid;
#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmVPositionOrigin.hh"

class GmPositionUserVolumes : public GmVPositionOrigin
{
public:
  GmPositionUserVolumes();
  ~GmPositionUserVolumes();

  virtual std::vector<GVSTouchableInfo*> AddTouchableInfos( const std::vector<G4String>& params ) const;
  void SetNewValue(G4UIcommand * command,G4String newValues);


private:
  void CheckNoSolidParams( const G4String& solidType, const unsigned int NoParamExpected, const unsigned int NoParam ) const; 
  GVSTouchableInfo* AddVolume( G4VSolid* solid, const std::vector<G4String>& params, unsigned int iSolid ) const;

};

#endif
