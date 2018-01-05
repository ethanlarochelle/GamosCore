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
#ifndef GmGeneratorChangeEnergyAndMaterial_h
#define GmGeneratorChangeEnergyAndMaterial_h

#include "GmGenerator.hh"
class G4PVPlacement;
class G4Material;
class G4LogicalVolume;
#include <map>
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosBase/Base/include/GmChangeEnergies.hh"

class GmGeneratorChangeEnergyAndMaterial : public GmGenerator,
					   public GmUserEventAction,
					   public GmChangeEnergies
{

public:
  GmGeneratorChangeEnergyAndMaterial();
  ~GmGeneratorChangeEnergyAndMaterial(){};
  void InitialiseRun();
  virtual void GeneratePrimaries(G4Event* evt);

  virtual void BeginOfEventAction( const G4Event* evt );
  virtual void EndOfEventAction( const G4Event* evt );

  std::vector<G4Material*> GetMaterials() const {
    return theMaterials;
  }

private:
  void BuildMaterialsFromGeom();
  void BuildMaterialsFromNameList();
  void BuildSimpleGeometry();
  void BuildLogicalVolumeList();
  void BuildSources();
  virtual void ChangeMaterialOrSource();
  virtual void ChangeMaterial(G4bool bChangeSource = true);
  virtual void ChangeSource();

protected:
  G4bool bInitialised;
  G4int theCurrentMaterial;
  G4int theCurrentSource;
  unsigned int theNEventsPerRun;

  std::vector<G4Material*> theMaterials;
  std::vector<G4String> theMaterialNames;
  G4bool bMaterialsFromGeom;
  std::vector<G4String> theLogicalVolumeNames;
  std::vector<G4LogicalVolume*> theLogicalVolumes;
  G4bool bBuildSimpleGeometry;

  std::map<G4double,G4String> theSourceCommands;
  G4bool bTooManyEvents;
  G4String thePrimaryParticleName;

};
#endif
