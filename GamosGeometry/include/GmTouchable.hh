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
#ifndef GmTouchable_h
#define GmTouchable_h

// Class that identifies a volume item ( = G4VTouchable with a format independent of G4 
// and some extra information)
// Pedro Arce
// 20.02.2001: Creation

#include <vector>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
class G4VPhysicalVolume;
class G4VSolid;
#include "G4VTouchable.hh"

class GmTouchable { 

 public:
  GmTouchable(){};
  GmTouchable( std::vector<G4VPhysicalVolume*> vpv, std::vector<G4int>& ancestorsCopyNo );
  GmTouchable( const G4VTouchable* g4touch );
  ~GmTouchable();

private:
  void BuildTouchable( std::vector<G4VPhysicalVolume*> vpv, std::vector<G4int>& ancestorsCopyNo );

// get methods
 public:
  friend std::ostream& operator<<(std::ostream&, const GmTouchable&);

  const G4String& GetName() const {
    return theName; }
  const G4String& GetShortName() const {
    return theName; }
  const G4String& GetLongName() const {
    return theLongName; }
  G4int GetCopyNo() const {
    return theCopyNo;} 
  const G4ThreeVector& GetGlobalPos() const {  //deprecated
    return GetGlobalPosition(); }
  const G4ThreeVector& GetGlobalPosition() const {
    return theGlobalPos;}
  const G4RotationMatrix& GetGlobalRotMat() const {  //deprecated
    return GetGlobalRotation(); } 
  const G4RotationMatrix& GetGlobalRotation() const { 
    return theGlobalRotMat;}
  const G4ThreeVector& GetLocalPos() const {  //deprecated
    return GetLocalPosition(); }
  const G4ThreeVector& GetLocalPosition() const {
    return theLocalPos;}
  const G4RotationMatrix& GetLocalRotMat() const {  //deprecated
    return GetLocalRotation(); } 
  const G4RotationMatrix& GetLocalRotation() const { 
    return theLocalRotMat;}
  //  VisCateg theVisCateg; 
  const G4String& GetMaterialName() const {
    return theMaterialName;}
  const G4VSolid* GetSolid() const {
    return theSolid;}

 private: 
  G4Transform3D CalculateTransformation( G4VPhysicalVolume* pv, G4int index );

 
private: 
  //! The name 
  G4String theName; 
  //! The long name: its name + copy No, plus those of its ancestors (e.g. /MODULE#1/NdcChamberRight#2/AirVoluForGas1#1 )
  G4String theLongName;
  //! The copy number: if the volume is created with a simple placement, it is the copy number you can find in the ASCII file; if it is created with a division or a parameterisation it is the number of the copy, the number in the ASCII file being irrelevant for the parameterisation (divisions are created without copy number in the ASCII file).
  G4int theCopyNo; 
  //! position in global reference frame 
  G4ThreeVector theGlobalPos; 
  //! rotation matrix in global reference frame
  G4RotationMatrix theGlobalRotMat;
  //! position in local reference frame
  G4ThreeVector theLocalPos;
  //! rotation matrix in local reference frame
  G4RotationMatrix theLocalRotMat;
  //! the name of the material
  G4String theMaterialName;
  //! the solid shape and parameters
  G4VSolid* theSolid; 

}; 

#endif
