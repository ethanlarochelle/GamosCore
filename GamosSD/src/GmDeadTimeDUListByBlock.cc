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
#include "GamosCore/GamosSD/include/GmDeadTimeDUListByBlock.hh"
#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"


//----------------------------------------------------------------------
GmDeadTimeDUListByBlock::GmDeadTimeDUListByBlock(G4String sdtype)
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  theNShift = G4int(parmgr->GetNumericValue("SD:DeadTimeDUListByBlock:NShift:"+sdtype, 100));

  G4int NAncestors = G4int(parmgr->GetNumericValue("SD:DeadTimeDUListByBlock:NAncestors:"+sdtype, 2));
  theNShift = G4int(pow(double(theNShift),int(NAncestors-1)));
  G4cout << " DeadTimeDUListByBlock: NShift " << theNShift << G4endl;

}


//----------------------------------------------------------------------
void GmDeadTimeDUListByBlock::AddDetUnit( GmHit* hit )
{
  DTHitInfo hinfo;
  hinfo.time = hit->GetTime();
  hinfo.eventID = hit->GetEventID();
  (*this)[hit->GetDetUnitID()/theNShift] = hinfo;
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << "GmDeadTimeDUListByBlock::AddDetUnit in Block " << hit->GetDetUnitID()/theNShift << " time " <<  hit->GetTime() << " N DU= " << size() << G4endl;
#endif
}
