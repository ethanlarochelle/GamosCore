#ifndef GmClassifierByMaterial_hh
#define GmClassifierByMaterial_hh 1

#include <vector>
#include <map>
#include "globals.hh"
class G4Material;
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmClassifierByMaterial  : public GmVClassifier
{
public: // with description
  GmClassifierByMaterial(G4String);
  virtual ~GmClassifierByMaterial();
  
  virtual int64_t GetIndexFromStep(const G4Step*);
  virtual int64_t GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(int64_t);

  virtual void SetIndices( std::vector<G4String> wl );

private:
  std::map<G4Material*,int64_t> theIndexMap;
};

#endif
