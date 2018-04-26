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
#include "Gm3ddose.hh"
#include "GmSqdose.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-----------------------------------------------------------------------
Gm3ddose::Gm3ddose()
{
  theHeader = 0;
}

//-----------------------------------------------------------------------
std::ostream& operator<<(std::ostream &out, 
			 const Gm3ddose &gmdose)
{
  out << *(gmdose.GetHeader());
  size_t siz = gmdose.GetDoses().size();
  size_t nz = gmdose.GetHeader()->GetNoVoxelZ();
  std::vector<float> dose = gmdose.GetDoses();
  std::vector<float> doseError = gmdose.GetDoseErrors();

  //  G4cout << &gmdose << " writing dose " << dose[0] << " " << dose[1] << G4endl;
  for( size_t ii = 0; ii < siz; ii++ ) {
    out << dose[ii];
    if( ii%nz == nz-1) {
      out << G4endl;
    } else {
      out << " ";
    }
  }

  for( size_t ii = 0; ii < siz; ii++ ) {
    out << doseError[ii];
    if( ii%nz == nz-1) {
      out << G4endl;
    } else {
      out << " ";
    }
  }

  return out;

}

//-----------------------------------------------------------------------
void Gm3ddose::Read( G4String& fileName )
{
  GmFileIn fin = GmFileIn::GetInstance( fileName );
  Read(fin);
}

//-----------------------------------------------------------------------
Gm3ddose::Gm3ddose(const GmSqdose& sqd)
{
  theHeader = new Gm3ddoseHeader(*(sqd.GetHeader()));

  theDoses = sqd.GetDoses();
  std::vector<float> doseSqs = sqd.GetDoseSqs();

  float nev = theHeader->GetNumberOfEvents();
  float nev1= nev - 1;
  size_t nv = theDoses.size();
  for( size_t jj = 0; jj < nv; jj++ ){
    theDoseErrors.push_back( (doseSqs[jj]*nev - theDoses[jj]*theDoses[jj] ) / nev1);
  }

}


//-----------------------------------------------------------------------
void Gm3ddose::Read( GmFileIn& fin )
{
  G4cout << " Gm3ddose::Read " << G4endl; //GDEB
  if( theHeader != 0 ){
    G4Exception("Gm3ddose::Read",
		"Wrong argument",
		FatalErrorInArgument, 
		"This dose does have a header already");
  }
  theHeader = new Gm3ddoseHeader;

  theHeader->Read( fin );

  size_t nv = theHeader->GetNoVoxelX() *
  theHeader->GetNoVoxelY() * 
	       theHeader->GetNoVoxelZ();
  std::vector<G4String> wl;
  size_t iv = 0;
  G4double dose;
  std::ifstream* fins = fin.GetIfstream();
  for( iv = 0; iv < nv; iv++ ){
    *fins >> dose;
    theDoses.push_back( dose );
  }

  iv = 0;
  for( iv = 0; iv < nv; iv++ ){
    *fins >> dose;
    theDoseErrors.push_back( dose );
  }
 
  if( theDoses.size() != theDoseErrors.size() ){
    G4Exception("Gm3ddose::Read",
		"Wrong format of dose file",FatalErrorInArgument,G4String(" number of dose values = " + GmGenUtils::itoa(theDoses.size())+ "must be equal to number of error values"+GmGenUtils::itoa(theDoseErrors.size())).c_str());
  }

  //  G4cout << this <<" dose value " << theDoses[0] << G4endl;
}

//-----------------------------------------------------------------------
Gm3ddose Gm3ddose::operator+=( const Gm3ddose& gmdose )
{
  size_t siz = theDoses.size();
  std::vector<float> doseB = gmdose.GetDoses();
  std::vector<float> doseErrorB = gmdose.GetDoseErrors();

  //  G4double nevA = theHeader->GetNumberOfEvents();
  // G4double nevB = gmdose.GetHeader()->GetNumberOfEvents();
  //  G4double nevSUM = nevA + nevB;
  for( size_t ii = 0; ii < siz; ii++ ) {
    //    if( ii == 0 ) G4cout << this << " dose1 " << theDoses[ii] << " " << &gmdose << " doseB " << doseB[ii] << G4endl;
    theDoses[ii] = theDoses[ii] + doseB[ii];
    theDoseErrors[ii] = sqrt(sqr(theDoseErrors[ii]) + sqr(doseErrorB[ii]));
    //    theDoses[ii] = (theDoses[ii]*nevA + doseB[ii]*nevB) / nevSUM;
    //    theDoseErrors[ii] = sqrt(sqr(theDoseErrors[ii]*nevA) + sqr(doseErrorB[ii]*nevB)/sqr(nevSUM*nevSUM));
    //    if( ii == 0 ) G4cout << "summed dose1 " << theDoses[ii] << " doseB " << doseB[ii] << G4endl;
  }

  //  G4cout << "dose value sum " << theDoses[0] << G4endl;

  return *this;
}
