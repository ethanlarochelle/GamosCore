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
#ifndef GmVolumeFilters_hh
#define GmVolumeFilters_hh
#include "GmTouchablePhantomStructure.hh"
#include "GamosCore/GamosBase/Filters/include/GmVVolumeConditionFilter.hh"
#include "GamosCore/GamosBase/Filters/include/GmGeomConditionEnter.hh"
#include "GamosCore/GamosBase/Filters/include/GmGeomConditionExit.hh"
#include "GamosCore/GamosBase/Filters/include/GmGeomConditionStart.hh"
#include "GamosCore/GamosBase/Filters/include/GmGeomConditionEnd.hh"
#include "GamosCore/GamosBase/Filters/include/GmGeomConditionIn.hh"
#include "GamosCore/GamosBase/Filters/include/GmGeomConditionTraverse.hh"

class GmEnterPhantomStructureFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchablePhantomStructure>
{
public:
  GmEnterPhantomStructureFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchablePhantomStructure>(name) {}
};

class GmExitPhantomStructureFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchablePhantomStructure>
{
public:
  GmExitPhantomStructureFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchablePhantomStructure>(name) {}
};

class GmStartPhantomStructureFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchablePhantomStructure>
{
public:
  GmStartPhantomStructureFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchablePhantomStructure>(name) {}
};

class GmEndPhantomStructureFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchablePhantomStructure>
{
public:
  GmEndPhantomStructureFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchablePhantomStructure>(name) {}
};

class GmTraversePhantomStructureFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchablePhantomStructure>
{
public:
  GmTraversePhantomStructureFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchablePhantomStructure>(name) {}
};
class GmInPhantomStructureFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchablePhantomStructure>
{
public:
  GmInPhantomStructureFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchablePhantomStructure>(name) {}
};

#endif
