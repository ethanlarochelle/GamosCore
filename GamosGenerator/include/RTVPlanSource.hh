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
#ifndef RTVPlanSource_H
#define RTVPlanSource_H
//
#include "GmParticleSource.hh"
#include "G4ParticleDefinition.hh"
#include "RTIonPlanControlPoint.hh"
#include "RTPlanParamReader.hh"
#include "G4RotationMatrix.hh"

class RTVPlanBeam;
class G4EventManager;
class G4RunManager;
class G4VPhysicalVolume;
class GmVModule;
class GmGeometryUtils;
class GmModuleMgr;
class GmVGenerDistPosition2D;

class RTVPlanSource : public GmParticleSource, public RTPlanParamReader
{
public:
  RTVPlanSource( const G4String& name);
  ~RTVPlanSource(){}

  virtual G4PrimaryVertex* GenerateVertex( G4double time );

  virtual G4String GetParticleName() const {
    return theParticleDef->GetParticleName(); }

  virtual void AssociatePlanVolumes( std::vector<G4String> wl );
  virtual void DefineAcceleratorVolume( std::vector<G4String> wl );

  size_t GetNoBeams() const {
    return theBeams.size();
  }
  RTVPlanBeam* GetBeam( G4int ii );


protected:
  virtual G4double GetSpotSize( ScanSpotData spData ) = 0;
  virtual void SetSpotSize( GmVGenerDistPosition2D* psitionDistribution2D, const ScanSpotData& spdata ) = 0;
  virtual void InitializeDistributions( const G4String& partName);
  virtual void ReadPlanData() = 0;
  virtual void CalculateMetersetEvts();
  virtual void PlotScanSpots(){};

  void BuildRTPlan2G4Associations();
  void MoveAccelerator(const ScanSpotData& spdata);
  void MovePhantom(const ScanSpotData& spdata);

  G4String thePlanFileName;
  G4String theBeamFileName;
  G4String theCPFileName;
  G4ParticleDefinition* theParticleDef;

  std::vector<RTVPlanBeam*> theBeams;
  std::vector<G4double> theMetersetEvt;

  G4int iCurrentBeam;
  G4double theNEventsPerMSUnit;
  G4double theMSUnitsToSkip;
  G4int theNEventsToSkip;
  G4int theCurrentMetersetEvtID;

  G4double theRotAngleX;
  G4double theRotAngleY;
  G4double theRotAngleZ;

  G4bool bEGauss;  
  G4double theEnergySigma;

  G4bool bDiscGaussian;
  G4double theDiscRadius;
  G4double theDiscSigma;

  G4bool bDirAngle;
  G4bool bDirAngle2D;
  G4double theOpeningAngle;
  G4double theOpeningAngleX;
  G4double theOpeningAngleY;
  
  G4EventManager* theEvtMgr;

  GmGeometryUtils* theGeomUtils;
  GmModuleMgr* theModuleMgr;
  G4RunManager* theG4RunManager;
  
  G4int theEventID;
  G4bool bInitialized;
  
  std::map<G4String,GmVModule*> theRTPlanGmModuleAssoc; 
  std::map<G4String,G4String> theRTPlanVoluAssoc;
  std::map<G4String,std::vector<G4VPhysicalVolume*> > theRTPlanG4PVsAssoc;
  G4String theAcceleratorVolume;
  G4ThreeVector theOrigPhantomTranslation;
  G4RotationMatrix theOrigPhantomRotMat;

  G4bool bCoordSystemPatient;
  GmVGenerDistPosition2D* thePositionDistribution2D;
};


#endif // RTVPlanSource_H
