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
#ifndef GmG4PhysicsLists_hh
#define GmG4PhysicsLists_hh

#include "FTFP_BERT.hh"
class G4FTFP_BERT : public  FTFP_BERT 
{
public:
  G4FTFP_BERT() {}
};
 
#include "FTFP_BERT_HP.hh"
class G4FTFP_BERT_HP : public  FTFP_BERT_HP 
{
public:
  G4FTFP_BERT_HP() {}
};
 
#include "FTFP_BERT_TRV.hh"
class G4FTFP_BERT_TRV : public  FTFP_BERT_TRV 
{
public:
  G4FTFP_BERT_TRV() {}
};

#include "FTFP_INCLXX.hh"
class G4FTFP_INCLXX : public  FTFP_INCLXX 
{
public:
  G4FTFP_INCLXX() {}
};
 
#include "FTFP_INCLXX_HP.hh"
class G4FTFP_INCLXX_HP : public  FTFP_INCLXX_HP 
{
public:
  G4FTFP_INCLXX_HP() {}
};
 
#include "FTF_BIC.hh"
class G4FTF_BIC : public  FTF_BIC 
{
public:
  G4FTF_BIC() {}
};

#ifndef WIN32
#include "G4GenericPhysicsList.hh"
class G4G4GenericPhysicsList : public  G4GenericPhysicsList 
{
public:
  G4G4GenericPhysicsList() {}
};
#endif

#include "LBE.hh"
class G4LBE : public  LBE 
{
public:
  G4LBE() {}
};
 
#include "QBBC.hh"
class G4QBBC : public  QBBC 
{
public:
  G4QBBC() {}
};
 
#include "QGSP_BERT.hh"
class G4QGSP_BERT : public  QGSP_BERT 
{
public:
  G4QGSP_BERT() {}
};
 
#include "QGSP_BERT_HP.hh"
class G4QGSP_BERT_HP : public  QGSP_BERT_HP 
{
public:
  G4QGSP_BERT_HP() {}
};
 
#include "QGSP_BIC.hh"
class G4QGSP_BIC : public  QGSP_BIC 
{
public:
  G4QGSP_BIC() {}
};
 
#include "QGSP_BIC_HP.hh"
class G4QGSP_BIC_HP : public  QGSP_BIC_HP 
{
public:
  G4QGSP_BIC_HP() {}
};
 
#include "QGSP_FTFP_BERT.hh"
class G4QGSP_FTFP_BERT : public  QGSP_FTFP_BERT 
{
public:
  G4QGSP_FTFP_BERT() {}
};
 
#include "QGSP_INCLXX.hh"
class G4QGSP_INCLXX : public  QGSP_INCLXX 
{
public:
  G4QGSP_INCLXX() {}
};
 
#include "QGSP_INCLXX_HP.hh"
class G4QGSP_INCLXX_HP : public  QGSP_INCLXX_HP 
{
public:
  G4QGSP_INCLXX_HP() {}
};
 
#include "QGS_BIC.hh"
class G4QGS_BIC : public  QGS_BIC 
{
public:
  G4QGS_BIC() {}
};
 
#include "Shielding.hh"
class G4Shielding : public  Shielding 
{
public:
  G4Shielding() {}
};

#include "QGSP_BIC_AllHP.hh"
class G4QGSP_BIC_AllHP : public  QGSP_BIC_AllHP
{
public:
  G4QGSP_BIC_AllHP() {}
};

#endif
