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
#include "GmClassifierBy1Ancestor.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
//#include "G4RegNavHelper.hh"

//-------------------------------------------------------------
GmClassifierBy1Ancestor::GmClassifierBy1Ancestor(G4String name) : GmVClassifier( name )
{
} 

//-------------------------------------------------------------
void GmClassifierBy1Ancestor::SetParameters( std::vector<G4String>& params )
{
  if( params.size() != 1 ){
    G4Exception("GmClassifierBy1Ancestor::GmClassifierBy1Ancestor"," Number of parameters different than 1",FatalErrorInArgument," Only parameter is ancestor level");
  }

  theIndexDepth = GmGenUtils::GetInteger(params[0]);
  //  G4cout << this << " GmClassifierBy1Ancestor::SetParameters indexDepth " << theIndexDepth << G4endl;
}

//-------------------------------------------------------------
G4int GmClassifierBy1Ancestor::GetIndexFromStep(const G4Step* aStep)
{
  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4TouchableHistory* th = (G4TouchableHistory*)(preStep->GetTouchable());
  // G4cout << "GmClassifierBy1Ancestor::GetIndexFromStep "<< th->GetReplicaNumber(theIndexDepth) << " = " << aStep->GetTrack()->GetVolume()->GetCopyNo() << G4endl;
  //-  th = (G4TouchableHistory*)( aStep->GetPostStepPoint()->GetTouchable());
  /// G4cout << aStep->GetTrack()->GetPosition().z() << "  GmClassifierBy1Ancestor::GetIndexFromStep "<< th->GetReplicaNumber(theIndexDepth) << " = " << aStep->GetTrack()->GetNextVolume()->GetCopyNo() << G4endl;


    /// G4cout << aStep->GetTrack()->GetPosition().z() << "  GmClassifierBy1Ancestor::GetIndexFromStep "<< th->GetReplicaNumber(theIndexDepth) << " = " << aStep->GetTrack()->GetNextVolume()->GetCopyNo() << G4endl;
  //  G4cout << " GmClassifierBy1Ancestor::GetIndexFromStep( " <<  th->GetReplicaNumber(theIndexDepth) << " " << theIndexDepth << " touch " << th->GetVolume()->GetName() << G4endl;
  G4int index = th->GetReplicaNumber(theIndexDepth);//ts0
  if( theIndexMap.size() != 0 ) {
    std::map<G4int,G4int>::const_iterator ite = theIndexMap.find(index);
    if( ite == theIndexMap.end() ) {
      G4Exception("GmClassifierBy1Ancestor::GetIndexFromStep",
		  "Index not found",
		  FatalErrorInArgument,
		  G4String("Index = " + GmGenUtils::itoa(index)).c_str());
    } else {
      return (*ite).second;
    }
  }
  
  return index;

}

//-------------------------------------------------------------
G4int GmClassifierBy1Ancestor::GetIndexFromTrack( const G4Track* aTrack )
{
  const G4Step* aStep = aTrack->GetStep();
  if( !aStep ) return 0;

  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4TouchableHistory* th = (G4TouchableHistory*)(preStep->GetTouchable());
  // G4cout << "GmClassifierBy1Ancestor::GetIndex "<< th->GetReplicaNumber(theIndexDepth) << " = " << aStep->GetTrack()->GetVolume()->GetCopyNo() << G4endl;
  //-  th = (G4TouchableHistory*)( aStep->GetPostStepPoint()->GetTouchable());
  /// G4cout << aStep->GetTrack()->GetPosition().z() << "  GmClassifierBy1Ancestor::GetIndex "<< th->GetReplicaNumber(theIndexDepth) << " = " << aStep->GetTrack()->GetNextVolume()->GetCopyNo() << G4endl;


    /// G4cout << aStep->GetTrack()->GetPosition().z() << "  GmClassifierBy1Ancestor::GetIndex "<< th->GetReplicaNumber(theIndexDepth) << " = " << aStep->GetTrack()->GetNextVolume()->GetCopyNo() << G4endl;
  //  G4cout << " GmClassifierBy1Ancestor::GetIndex( " <<  th->GetReplicaNumber(theIndexDepth) << " " << theIndexDepth << " touch " << th->GetVolume()->GetName() << G4endl;
  G4int index = th->GetReplicaNumber(theIndexDepth);//ts0
  if( theIndexMap.size() != 0 ) {
    std::map<G4int,G4int>::const_iterator ite = theIndexMap.find(index);
    if( ite == theIndexMap.end() ) {
      G4Exception("GmClassifierBy1Ancestor::GetIndex",
		  "Index not found",
		  FatalErrorInArgument,
		  G4String("Index = " + GmGenUtils::itoa(index)).c_str());
    } else {
      return (*ite).second;
    }
  }
  
  return index;

}

//-------------------------------------------------------------
void GmClassifierBy1Ancestor::SetIndices( std::vector<G4String> wl )
{

  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
    G4int index = G4int(GmGenUtils::GetValue(wl[ii+1]));
    theIndexMap[G4int(GmGenUtils::GetValue(wl[ii]))] = index;
    if( theMaxIndex < index) theMaxIndex = index;    
  }

  theMaxIndex -= theIndexMap.size()+1;  
}
