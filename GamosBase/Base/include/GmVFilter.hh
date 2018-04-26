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
#ifndef GmVFilter_h
#define GmVFilter_h 1

class G4Track;
#include "G4Step.hh"
#include "globals.hh"
#include <vector>

// class description:
//
//  This is the abstract base class of a filter

class GmVFilter
{
  
public: // with description
  GmVFilter(G4String name);
  
public:
  virtual ~GmVFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track*){ return TRUE; };
  virtual G4bool AcceptStep(const G4Step* aStep){ 
    return AcceptTrack(aStep->GetTrack()); }
  
  virtual void show(){}
  virtual void SetParameters( std::vector<G4String>& ){}

public:
  inline G4String GetName() const {
    return theFilterName; }
  inline G4String GetClass() const {
    return theClass; }
  inline void SetClass( G4String cl ) {
    theClass = cl;  }
  G4bool IsFutureFilter() const {
    return bFutureFilter;   
  }

protected:
  G4String theFilterName;
  G4String theClass;
  G4bool bFutureFilter;  
};

#endif
