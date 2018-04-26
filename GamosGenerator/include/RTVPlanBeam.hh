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
#ifndef RTVPlanBeam_H
#define RTVPlanBeam_H
#include "RTVPlanControlPoint.hh"
#include "RTPlanParamReader.hh"

class RTVPlanBeam : public RTPlanParamReader
{
public: 
  RTVPlanBeam(){}
  ~RTVPlanBeam(){}
  std::vector<RTVPlanControlPoint*> GetControlPoints() const {
    return theControlPoints;
  }
  virtual ScanSpotData NextScanSpot( G4int& incp );
  G4int GetIndex() const {
    return theIndex;
  }

  G4int GetNoControlPoints() const {
    return theControlPoints.size();
  }
  RTVPlanControlPoint* GetControlPoint( G4int ii );
  G4String GetFileName() const {
    return theFileName;
  }
  void SetFileName( G4String fn ) {
    theFileName = fn;
  }

protected:
  virtual void ReorderControlPoints() = 0;

  std::vector<RTVPlanControlPoint*> theControlPoints;

  G4int theIndex;
  G4int iCurrentControlPoint;
  G4String theFileName;
};

#endif // RTVPlanBeam_H
