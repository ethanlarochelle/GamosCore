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
#ifndef __GmPDSScoringTrackInfo_hh
#define __GmPDSScoringTrackInfo_hh

#include "GamosCore/GamosUtils/include/GmWeightTrackInfo.hh"
#include "G4Point3D.hh"
#include <map>

class GmPDSScoringTrackInfo : public GmWeightTrackInfo
{
public:
  GmPDSScoringTrackInfo(G4String& procName, G4Point3D& point ){
    theProcessName = procName; 
    thePoint = point;
  }
  ~GmPDSScoringTrackInfo(){}

  virtual void Print() const{};

  G4String GetProcessName() const {
    return theProcessName; }
  void SetProcessName( G4String procName ) {
    theProcessName = procName; }
  G4Point3D GetPoint() const { 
    return thePoint; }
  void SetPoint( G4Point3D& pt ) {
    thePoint = pt;
  }

private:
  G4String theProcessName;
  G4Point3D thePoint;
};

#endif
