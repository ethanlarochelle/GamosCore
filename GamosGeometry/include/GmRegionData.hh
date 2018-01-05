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
#ifndef GmRegionData_h
#define GmRegionData_h

/*---------------------------------------------------------------------------   
ClassName:   GmRegionData    
Author:      P. Arce
Changes:      
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 

#include <vector>
#include "globals.hh"

//------------------------------------------------------------------------
class GmRegionData 
{ 
 public:
  GmRegionData( const std::vector<G4String>& rd );

  ~GmRegionData();

  void SetCutsData( const std::vector<G4String>& cu );

  //--- Get methods
  G4String GetRegionName() const {
    return theRegionName; }

  std::vector<G4String> GetLVNames(){
    return theLVNames; }

  G4double GetGammaCut() const {
    return theGammaCut; }
  G4double GetElectronCut() const {
    return theElectronCut; }
  G4double GetPositronCut() const {
    return thePositronCut; }
  G4bool CutsAreSet() const {
    return bCutsSet; }

private:
  G4String theRegionName;

  std::vector<G4String> theLVNames;
  G4double theGammaCut, theElectronCut, thePositronCut;
  G4bool bCutsSet;
};

#endif
