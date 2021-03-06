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
#ifndef GmProdCutOutsideVoxelFilter_h
#define GmProdCutOutsideVoxelFilter_h 1

#include "globals.hh"
#include "GmVProdCutOutsideVoxelFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts particles that would be cut by a production cut (and their daughters) and only if they are in the voxel different than the voxel where it would be produced (for daughters it is counted the voxel of the ancestor)
// The cut is only checked for particles created by ionisationor bremsstrahlung processes
// 
///////////////////////////////////////////////////////////////////////////////

#include <set>

class GmProdCutOutsideVoxelFilter : public GmVProdCutOutsideVoxelFilter
{

public: // with description
  GmProdCutOutsideVoxelFilter(G4String name);
  virtual ~GmProdCutOutsideVoxelFilter();

public: // with description
  virtual G4bool CheckProcess(const G4VProcess*);

};

#endif
