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
#ifndef GmVDataEquivalentDose_hh
#define GmVDataEquivalentDose_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include <map>
class G4Step;

struct Flux2Dose {
  G4double Hstar;
  G4double Hp0;
  G4double Hp15;
  G4double Hp30;
  G4double Hp45;
  G4double Hp60;
  G4double Hp75;
};
enum equivDose {edHstar, edHp0, edHp15, edHp30, edHp45, edHp60, edHp75 };

class GmVDataEquivalentDose : public GmVData
{
public:
  GmVDataEquivalentDose();
  ~GmVDataEquivalentDose();

protected:
  G4double DoseFromEnergy( G4double ener, const G4Step* aStep );
  G4double EnergyToDoseFactor( G4double ener );
  std::map<G4double,Flux2Dose>* ReadEnergyBinsForNeutrons();
  std::map<G4double,Flux2Dose>* ReadEnergyBinsForGammas();
  void SetEquivDoseType( const G4String& doseName );

protected:
  std::map<G4double,Flux2Dose>* theFlux2Dose;
  equivDose theEquivDose;

};
#endif
