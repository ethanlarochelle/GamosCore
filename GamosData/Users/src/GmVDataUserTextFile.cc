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
#include "GmVDataUserTextFile.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4RunManager.hh"

#include <fstream>

//----------------------------------------------------------------
GmVDataUserTextFile::GmVDataUserTextFile()
{
}

//----------------------------------------------------------------
void GmVDataUserTextFile::WriteHeaderText()
{

  bWriteHeaderData = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theOutputType+":WriteHeaderData",0));
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) )  G4cout << " bWriteHeaderData " << bWriteHeaderData << " " << theOutputType+":WriteHeaderData" << G4endl;
#endif

  if( !bWriteHeaderData ) return;

  unsigned int ndata = theData.size();
  *theFileOut << "HEADER: ";
  *theFileOut << ndata << ",";
  for( unsigned int ii = 0; ii < theData.size(); ii++) {
    G4String dataName = theData[ii]->GetName();
    *theFileOut << "\"" << dataName << "\"";
    if( ii != ndata-1) *theFileOut << ",";
  }

  *theFileOut << G4endl;
  
}
