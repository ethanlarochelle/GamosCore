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
#include "Reflex/PluginService.h"

#include "GmDummyPhysics.hh"
PLUGINSVC_FACTORY(GmDummyPhysics,G4VUserPhysicsList*())

#include "GmDNAPhysics.hh"
PLUGINSVC_FACTORY(GmDNAPhysics,G4VUserPhysicsList*())

#include "GmG4PhysicsLists.hh"
PLUGINSVC_FACTORY(G4FTFP_BERT,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_BERT_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_BERT_TRV,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_INCLXX,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_INCLXX_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTF_BIC,G4VUserPhysicsList*())
#ifndef WIN32
PLUGINSVC_FACTORY(G4G4GenericPhysicsList,G4VUserPhysicsList*())
#endif
PLUGINSVC_FACTORY(G4LBE,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QBBC,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BERT,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BERT_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BIC,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BIC_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_FTFP_BERT,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_INCLXX,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_INCLXX_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGS_BIC,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4Shielding,G4VUserPhysicsList*())

PLUGINSVC_FACTORY(G4QGSP_BIC_AllHP,G4VUserPhysicsList*())

#include "GmQGSP_BIC_HP_EMopt3.hh"
PLUGINSVC_FACTORY(GmQGSP_BIC_HP_EMopt3,G4VUserPhysicsList*())
 
#include "PhysListEmStandardSS.hh"
PLUGINSVC_FACTORY(PhysListEmStandardSS,G4VUserPhysicsList*())

