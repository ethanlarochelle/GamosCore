# - Define datasets known and used by Gamos
# We keep this separate from the GamosInstallData module for conveniance
# when updating and patching because datasets may change more rapidly.
# It allows us to decouple the dataset definitions from how they are
# checked/installed/configured
#

# - NDL
gamos_add_dataset(
  NAME      GmNDL
  VERSION   4.2
  FILENAME  GmNDL
  EXTENSION tar.gz
  ENVVAR    GmNEUTRONHPDATA
  MD5SUM    2302d7ffeef1a450955302f027ee1cf4
  )

# - Low energy electromagnetics
gamos_add_dataset(
  NAME      GmEMLOW
  VERSION   6.32
  FILENAME  GmEMLOW
  EXTENSION tar.gz
  ENVVAR    GmLEDATA
  MD5SUM    9d3302072ba694b1d4505c330ed89d89
  )

# - Photon evaporation
gamos_add_dataset(
  NAME      PhotonEvaporation
  VERSION   2.3
  FILENAME  GmPhotonEvaporation
  EXTENSION tar.gz
  ENVVAR    GmLEVELGAMMADATA
  MD5SUM    08848ebdd536280a0629d802040b70be
  )

# - Radioisotopes
gamos_add_dataset(
  NAME      RadioactiveDecay
  VERSION   3.6
  FILENAME  GmRadioactiveDecay
  EXTENSION tar.gz
  ENVVAR    GmRADIOACTIVEDATA
  MD5SUM    9566ff1614bea7455343328cb66abd6f
  )

# - Neutron XS
gamos_add_dataset(
  NAME      GmNEUTRONXS
  VERSION   1.2
  FILENAME  GmNEUTRONXS
  EXTENSION tar.gz
  ENVVAR    GmNEUTRONXSDATA
  MD5SUM    092634b9258c7bc387cb83557ff1df81
  )

# - PII
gamos_add_dataset(
  NAME      GmPII
  VERSION   1.3
  FILENAME  GmPII
  EXTENSION tar.gz
  ENVVAR    GmPIIDATA
  MD5SUM    05f2471dbcdf1a2b17cbff84e8e83b37
  )

# - Optical Surfaces
gamos_add_dataset(
  NAME      RealSurface
  VERSION   1.0
  FILENAME  RealSurface
  EXTENSION tar.gz
  ENVVAR    GmREALSURFACEDATA
  MD5SUM    0dde95e00fcd3bcd745804f870bb6884
  )

# - SAID
gamos_add_dataset(
  NAME      GmSAIDDATA
  VERSION   1.1
  FILENAME  GmSAIDDATA
  EXTENSION tar.gz
  ENVVAR    GmSAIDXSDATA
  MD5SUM    d88a31218fdf28455e5c5a3609f7216f
  )

