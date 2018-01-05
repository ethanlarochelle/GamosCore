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
#include "GmFilterFromClassifier.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosBase/Base/include/GmClassifierMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Step.hh"

//----------------------------------------------------------------------
GmFilterFromClassifier::GmFilterFromClassifier(G4String name)
  :GmVFilter(name)
{
}


//----------------------------------------------------------------------
GmFilterFromClassifier::~GmFilterFromClassifier()
{ 
}


//----------------------------------------------------------------------
G4bool GmFilterFromClassifier::AcceptStep(const G4Step* aStep)
{
  //  G4cout << "  GmFilterFromClassifier::AcceptStep " << theClassifier->GetIndexFromStep( const_cast<G4Step*>(aStep) ) << " =?= " << theIndex << G4endl;
  if( theClassifier->GetIndexFromStep( const_cast<G4Step*>(aStep) ) == theIndex ) {
    return TRUE;
  } else {
    return FALSE;
  }
}


//----------------------------------------------------------------------
void GmFilterFromClassifier::show()
{
  G4cout << "---- GmFilterFromClassifier "<< theClassifier->GetName() << G4endl;
}


//----------------------------------------------------------------------
void GmFilterFromClassifier::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 2  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmFilterFromClassifier::SetParameters",
		"There should be at least two parameters: CLASSIFIER_NAME INDEX",
		FatalErrorInArgument,
		G4String("The current parameters are: "+parastr).c_str());
  }

  params.insert(params.begin(), params[0] );
  theClassifier = GmClassifierMgr::GetInstance()->FindOrBuildClassifier(params);

  theIndex = atoi(params[2]);
   
}
