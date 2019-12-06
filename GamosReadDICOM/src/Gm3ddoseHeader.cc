#include "Gm3ddoseHeader.hh"
#include "GmSqdoseHeader.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-----------------------------------------------------------------------
G4bool Gm3ddoseHeader::operator==(const Gm3ddoseHeader& right) const
{
  G4bool equ = 1;
  if( theNoVoxelX != right.GetNoVoxelX() ||
    theNoVoxelX != right.GetNoVoxelX() ||
    theNoVoxelX != right.GetNoVoxelX() ||
    theVoxelLimitsX != right.GetVoxelLimitsX() ||
    theVoxelLimitsX != right.GetVoxelLimitsX() ||
    theVoxelLimitsX != right.GetVoxelLimitsX() ) {
    equ = 0;
  }
  return equ;

}


//-----------------------------------------------------------------------
G4bool Gm3ddoseHeader::operator!=(const Gm3ddoseHeader& right) const
{ 
  return !( *this == right );
}

//-----------------------------------------------------------------------
Gm3ddoseHeader::Gm3ddoseHeader(const GmSqdoseHeader& sqdh)
{
  //  G4cout << " reading header file " << G4endl;
  theNoEvent = sqdh.GetNumberOfEvents();
  G4cout << " N events " << theNoEvent << G4endl;

  theNoVoxelX = sqdh.GetNoVoxelX();
  theNoVoxelY = sqdh.GetNoVoxelY();
  theNoVoxelZ = sqdh.GetNoVoxelZ();

  theVoxelLimitsX = sqdh.GetVoxelLimitsX();
  theVoxelLimitsY = sqdh.GetVoxelLimitsY();
  theVoxelLimitsZ = sqdh.GetVoxelLimitsZ();

}

//-----------------------------------------------------------------------
void Gm3ddoseHeader::Read( GmFileIn& fin )
{
  //  G4cout << " reading header file " << G4endl;
  std::vector<G4String> wl;
  /*t
  fin.GetWordsInLine(wl);
  if( wl.size() != 1 ){
    G4Exception("Gm3ddoseHeader::Read"," first line must have 1 words: number of events",FatalErrorInArgument,G4String("It has "+GmGenUtils::itoa(wl.size())).c_str());
  }
  theNoEvent = (unsigned long long)(GmGenUtils::GetValue( wl[0] ));
  G4cout << " N events " << theNoEvent << G4endl;
  */
  theNoEvent = 1E6;
  G4cout << " N events set to " << theNoEvent << G4endl;

  fin.GetWordsInLine(wl);
  if( wl.size() != 3 ){
    G4Exception("Gm3ddoseHeader::Read"," second line must have 3 words: number of voxels",FatalErrorInArgument,G4String("It has "+GmGenUtils::itoa(wl.size())).c_str());
  }

  theNoVoxelX = GmGenUtils::GetInteger( wl[0] );
  theNoVoxelY = GmGenUtils::GetInteger( wl[1] );
  theNoVoxelZ = GmGenUtils::GetInteger( wl[2] );

  fin.GetWordsInLine(wl);
  if( wl.size() != theNoVoxelX+1 ){
    G4Exception("Gm3ddoseHeader::Read",G4String(" third line must have number of words = " + GmGenUtils::itoa(theNoVoxelX)).c_str(),FatalErrorInArgument,G4String("It has "+GmGenUtils::itoa(wl.size())).c_str());
  }
  for( size_t ii = 0; ii < theNoVoxelX+1; ii++ ){    
    theVoxelLimitsX.push_back( GmGenUtils::GetValue(wl[ii]) );
  }

  fin.GetWordsInLine(wl);
  if( wl.size() != theNoVoxelY+1 ){
    G4Exception("Gm3ddoseHeader::Read",G4String(" fourth line must have number of words = " + GmGenUtils::itoa(theNoVoxelY)).c_str(),FatalErrorInArgument,G4String("It has "+GmGenUtils::itoa(wl.size())).c_str());
  } 
  for( size_t ii = 0; ii < theNoVoxelY+1; ii++ ){    
    theVoxelLimitsY.push_back( GmGenUtils::GetValue(wl[ii]) );
  }

  fin.GetWordsInLine(wl);
  if( wl.size() != theNoVoxelZ+1 ){
    G4Exception("Gm3ddoseHeader::Read",G4String(" fifth line must have number of words = " + GmGenUtils::itoa(theNoVoxelZ)).c_str(),FatalErrorInArgument,G4String("It has "+GmGenUtils::itoa(wl.size())).c_str());
  }
  for( size_t ii = 0; ii < theNoVoxelZ+1; ii++ ){    
    theVoxelLimitsZ.push_back( GmGenUtils::GetValue(wl[ii]) );
  }

}


//-----------------------------------------------------------------------
std::ostream& operator<<(std::ostream &out, 
			 const Gm3ddoseHeader &dh)
{

  out << dh.GetNumberOfEvents() << G4endl;
  //  G4cout << "DOSE N EV " << dh.GetNumberOfEvents() << G4endl;

  out << dh.GetNoVoxelX() << " " << dh.GetNoVoxelY() << " " << dh.GetNoVoxelZ() << G4endl;
  std::vector<float>::const_iterator ite;

  std::vector<float> vlim = dh.GetVoxelLimitsX();
  //  out << " size x " << dh.GetVoxelLimitsX().size() << G4endl;
  for( ite = vlim.begin(); ite != vlim.end(); ite++ ){
    out << *ite << " ";
  }
  out << G4endl;
 
  vlim = dh.GetVoxelLimitsY();
  for( ite = vlim.begin(); ite != vlim.end(); ite++ ){
    out << *ite << " ";
  }
  out << G4endl;
  
  vlim = dh.GetVoxelLimitsZ();
  for( ite = vlim.begin(); ite != vlim.end(); ite++ ){
    out << *ite << " ";
  }
  out << G4endl;

  return out;

}
