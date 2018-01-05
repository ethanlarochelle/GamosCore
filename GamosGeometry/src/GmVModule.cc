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
#include "GmVModule.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GmGeomVerbosity.hh"

//------------------------------------------------------------------------
GmVModule::GmVModule( const std::vector<G4String>& par )
{
  std::vector<G4String>::const_iterator ite = par.begin()+1;
  theName = *ite++;
  for( ; ite != par.end(); ite++ ){
    theParams.push_back( *ite );
  }

}


//------------------------------------------------------------------------
G4String GmVModule::GetParams( G4int ii )
{
  if( ii < 0 || ii >= G4int(theParams.size()) ) {
    G4Exception("GmVModule::GetParams",
		"Wrong argument index",
		FatalException,
		G4String("There are " + GmGenUtils::itoa(theParams.size()) + ", and you are asking for parameter number: " + GmGenUtils::itoa(ii)).c_str());
  }

  return theParams[ii];

}


//------------------------------------------------------------------------
GmVModule::~GmVModule()
{
}


//------------------------------------------------------------------------
void GmVModule::BuildObject( std::ostringstream& fout )
{
  G4String foutStr = fout.str();
  std::vector<G4String> lineWords = GmGenUtils::GetWordsInString(foutStr);
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) )  G4cout << " GmVModule::BuildObject FOUT" << fout.str() << "FF" << G4endl;
#endif
  theLineProcessor->ProcessLine( lineWords );

  fout.str("");
  G4cout << "@@BuildObject:  " << foutStr << G4endl;

}


//------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const GmVModule& mod)
{
  out << " GEOMETRY_MODULE: " << mod.GetName() << G4endl;
  std::vector<G4String>::const_iterator ite;
  std::vector<G4String> par = mod.GetParams();
  out << " PARAMS:";
  for( ite = par.begin(); ite != par.end(); ite++ ){
    out << " " << *ite;
  }
  out << G4endl;

  return out;
}


G4String GmVModule::PrintW(G4String name )
{
  return name + "= " + theWords[name] + " ";
}

G4String GmVModule::PrintWVal(G4String name )
{
  return name + "= " + GmGenUtils::ftoa(GmGenUtils::GetValue(theWords[name])) + " ";
}
