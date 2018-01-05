cd Modules
ls  Geant4* | awk '{print "mv " $1 " " $1}' > kk
ema kk
mksr GEANT4 GAMOS .cmake
mksr Geant4 Gamos .cmake
mksr G4 Gm .cmake
mksr geant4 gamos .cmake
cd ../Templates
mksr GEANT4 GAMOS .cmake
mksr Geant4 Gamos .cmake
mksr G4 Gm .cmake
mksr g4 gm .cmake
mksr geant4 gamos .cmake
mksr GEANT4 GAMOS .in
mksr Geant4 Gamos .in
mksr G4 Gm .in
mksr geant4 gamos .in
mv Geant4Config.cmake.in GamosConfig.cmake.in
mv Geant4ConfigVersion.cmake.in GamosConfigVersion.cmake.in
mv UseGeant4.cmake UseGamos.cmake
mv UseGeant4_internal.cmake UseGamos_internal.cmake
mv geant4-config.in gamos-config.in
mv geant4-env-skeleton.in gamos-env-skeleton.in
mv geant4_validate_sources.cmake.in gamos_validate_sources.cmake.in
mv geant4make-skeleton.in gamosmake-skeleton.in
