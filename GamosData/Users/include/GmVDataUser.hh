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
#ifndef GmVDataUser_hh
#define GmVDataUser_hh

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"
class GmVFilter;
class GmVClassifier;

class GmVDataUser 
{
public:
  GmVDataUser();
  ~GmVDataUser(){};

  virtual void BuildData();
  virtual GmVData* BuildData2D( const G4String& dataName1, const G4String& dataName2 );
  virtual GmVData* BuildDataProf1D( const G4String& dataName1, const G4String& dataName2 );
  virtual GmVData* BuildDataProf2D( const G4String& dataName1, const G4String& dataName2, const G4String& dataName3 );

  GmDType GetDataType() const
  {
    return theDataType;
  }

  G4String GetDataTypeAsString() const;
  
protected:
  GmVData* Build1Data( const G4String& dataName );
  void SetUserName( G4String name ) {
    theUserName = name;
  }
  
protected:
  std::vector<GmVData*> theData;
  std::vector<GmVData*> theDoubleData;

  std::vector<G4String> theDataList;

  GmDType theDataType;

  G4bool bUseAtInitial;

  G4String theUserName; // same as User Action name
};

#endif
