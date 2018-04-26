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
#include "GmSqdose.hh"
#include "Gm3ddose.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//-----------------------------------------------------------------------
GmSqdose::GmSqdose()
{
  theHeader = 0;
  G4String stype = GmParameterMgr::GetInstance()->GetStringValue("GmSqdose:FileType","ALL");
  for(size_t ii = 0; ii < stype.length(); ii++) {
    stype[ii] = toupper(stype[ii]);
  }
  if( stype == "ALL" ) {
    theType = SqTALL;
  } else if( stype == "FILLED" ) {
    theType = SqTFILLED;
  } else if( stype == "CROSS_PHANTOM" || stype == "CROSSPHANTOM" ) {
    theType = SqTCROSS_PHANTOM;
  } else {
    G4Exception("GmSqdose::GmSqdose",
		"Wrong FileType",
		FatalErrorInArgument,
		G4String("FileType can be ALL, FILLED or CROSS_PHANTOM, it is " + stype ).c_str());
  }
  
}

//-----------------------------------------------------------------------
GmSqdose::~GmSqdose()
{
  // delete theHeader;
}

//-----------------------------------------------------------------------
void GmSqdose::Print( FILE* out )
{
  theHeader->Print(out);

  G4cout << " TYPE " << theType << G4endl; 
  if(fwrite(&theType,
	    sizeof(size_t),1,out)!=1){
    G4Exception("GmSqdose::Print",
		"Error",
		FatalException,
		"Error writing type");
  }

  size_t siz = 0;
  if( theType == SqTALL ) {
    siz = theHeader->GetNoVoxelX()*
      theHeader->GetNoVoxelY()*
      theHeader->GetNoVoxelZ();
  } else if ( theType == SqTCROSS_PHANTOM ) {
    siz = theDoses.size();
  }
  //  G4cout << " N VOXELS WRITE " << siz << G4endl;  //GDEB
  if( theType == SqTALL || theType == SqTCROSS_PHANTOM ) {
    //----- Loop to all voxels and print dose
    for( size_t ii = 0; ii < siz; ii++ ) {
      float dose = theDoses[ii];
      if(fwrite((float *) &dose,
		sizeof(float),1,out)!=1) {
	G4Exception("GmSqdose::Print",
		    "Error",
		    FatalException,
		    "Error writing dose");
      }
    }
    
    for( size_t ii = 0; ii < siz; ii++ ) {
      float dose2 = theDoseSqs[ii];
      if(fwrite((float *) &dose2,
		sizeof(float),1,out)!=1){
	G4Exception("GmSqdose::Print",
		    "Error",
		    FatalException,
		    "Error writing doseSq");
      }
    }
  } else if( theType == SqTFILLED ) {
    siz = theDoses.size();
    for( size_t ii = 0; ii < siz; ii++ ) {
      float dose = theDoses[ii];
      float dose2 = theDoseSqs[ii];
      if( ii%1000 == 0 ) G4cout << " GmSqdose Print DOSE " << ii <<" " << dose << " " << dose2 << G4endl;
      if( dose != 0. ) {
	if(fwrite(&ii,
		  sizeof(size_t),1,out)!=1){
	  G4Exception("GmSqdose::Print",
		      "Error",
		      FatalException,
		      "Error writing index");
	}
	if(fwrite((float *) &dose,
		  sizeof(float),1,out)!=1){
	  G4Exception("GmSqdose::Print",
		    "Error",
		    FatalException,
		      "Error writing dose");
	}
	if(fwrite((float *) &dose2,
		  sizeof(float),1,out)!=1){
	  G4Exception("GmSqdose::Print",
		    "Error",
		    FatalException,
		      "Error writing doseSq");
	} 
	//	G4cout << " WRITING DOSE " << ii << " " << dose << " " << dose2 << G4endl;     
      }
    }
    
  }

}

//-----------------------------------------------------------------------
void GmSqdose::Read( G4String fileName )
{
  FILE * fin = fopen(fileName,"rb");
    
  if( !fin ) {
    G4Exception("GmSqdose::Read",
		"Error",
		FatalException,
		("File not found: "+ fileName).c_str());
  }

  Read(fin);
}


//-----------------------------------------------------------------------
void GmSqdose::Read( FILE* fin )
{
  if( !fin ) {
    G4Exception("GmSqdose::Read",
		"Error",
		FatalException,
		"File not found");
  }

    if( theHeader != 0 ){
    G4Exception("GmSqdose::Read",
		    "Error",
		    FatalException,
		"This dose does have a header already");
  }
  theHeader = new GmSqdoseHeader;
  theHeader->Read( fin );

  if( fread(&theType, sizeof(size_t), 1, fin) != 1) {
    G4Exception("GmSqdose::Read",
		    "Error",
		    FatalException,
		"Problem reading type ");
  }
  G4cout << " GmSqdose::Read type " << theType << G4endl;

  size_t nv = 0;
  if( theType == SqTALL ) {
    nv = theHeader->GetNoVoxelX()*
      theHeader->GetNoVoxelY()*
      theHeader->GetNoVoxelZ();
  } else if ( theType == SqTCROSS_PHANTOM ) {
    nv = (theHeader->GetNoVoxelX()+theHeader->GetNoVoxelY()-1)*
      theHeader->GetNoVoxelZ();theDoses.size();
  }
  //  G4cout << " READ nv " << nv << G4endl; //GDEB  

  if( theType == SqTALL || theType == SqTCROSS_PHANTOM  ) {
    for( size_t iv = 0; iv < nv; iv++ ){
      float ftmp;
      if( fread(&ftmp, sizeof(float), 1, fin) != 1) {
	G4Exception("GmSqdose::Read()",
		    "Problem reading dose ",
		    FatalErrorInArgument,
		    G4String("Reading voxel number "+GmGenUtils::itoa(iv)).c_str());
      }
      theDoses.push_back( ftmp );
      //      if( theDoses.size()%1 == 0 ) G4cout << iv << " 1READ dose " << theDoses.size() << " = " << theDoses[theDoses.size()-1] << G4endl; //GDEB
    }
    
    for( size_t iv = 0; iv < nv; iv++ ){
      float ftmp;
      if( fread(&ftmp, sizeof(float), 1, fin) != 1) {
	G4Exception("GmSqdose::Read()",
		    "Problem reading dose ",
		    FatalErrorInArgument,
		    G4String("Reading voxel number "+GmGenUtils::itoa(iv)).c_str());
      }
      theDoseSqs.push_back( ftmp );
      //      if( theDoseSqs.size()%1000 == 1 ) G4cout << iv << " READ dose2 " << theDoseSqs.size() << " = " << theDoseSqs[theDoseSqs.size()-1] << G4endl;//GDEB
    }
  } else if( theType == SqTFILLED ){
    G4int idPrev = -1;
    size_t iv;
    for( iv = 0; iv < nv; iv++ ){
      size_t idNow;
      if( fread(&idNow, sizeof(size_t), 1, fin) != 1) {
	if(feof(fin)) {
	  break;
	} else {
	  G4Exception("GmSqdose::Read()","Problem reading voxel ID ",FatalErrorInArgument,G4String("Reading voxel number "+GmGenUtils::itoa(idNow)).c_str());
	}
      }
      float ftmp;
      if( fread(&ftmp, sizeof(float),  1, fin) != 1) {
	G4Exception("GmSqdose::Read()","Problem reading dose ",FatalErrorInArgument,G4String("Reading voxel number "+GmGenUtils::itoa(idNow)).c_str());
      }
      for( size_t ii = 0; ii < idNow-idPrev-1; ii++ ){
	theDoses.push_back( 0. );
      }
      theDoses.push_back( ftmp );
      //      if( idNow%1000 == 0 ) G4cout << " 2READ dose " << theDoses.size() << " = " << theDoses[theDoses.size()-1] << G4endl;//GDEB

      if( fread(&ftmp, sizeof(float),  1, fin) != 1) {
	G4Exception("GmSqdose::Read()","Problem reading dose ",FatalErrorInArgument,G4String("Reading voxel number "+GmGenUtils::itoa(idNow)).c_str());
      }
      for( size_t ii = 0; ii < idNow-idPrev-1; ii++ ){
	theDoseSqs.push_back( 0. );
      }
      theDoseSqs.push_back( ftmp );
      //      if( theDoseSqs.size()%1000 == 1 ) G4cout << iv << " READ dose2 " << theDoseSqs.size() << " = " << theDoseSqs[theDoseSqs.size()-1] << G4endl;//GDEB

      //      G4cout << iv << " READ DOSE " << idNow << " prev " << idPrev << " dose " << ftmp1 << " dose2 " << ftmp << G4endl;

      idPrev = idNow;
      //    if( theDoseSqs.size()%100000 == 1 ) G4cout << iv << " READ dose2 " << theDoseSqs.size() << " = " << theDoseSqs[theDoseSqs.size()-1] << G4endl;
    }
    G4cout << " GmSqdose Number of voxels read " << iv << " out of " << nv << G4endl;
  }
}

//-----------------------------------------------------------------------
GmSqdose GmSqdose::operator+=( const GmSqdose& gmdose )
{
  theHeader->SetNumberOfEvents(theHeader->GetNumberOfEvents()+gmdose.GetHeader()->GetNumberOfEvents());
  
  std::vector<float> doses = gmdose.GetDoses();
  std::vector<float> doses2 = gmdose.GetDoseSqs();

  size_t siz = theDoses.size();
  if( doses.size() > siz ) {
    for( size_t ii = siz; ii < doses.size(); ii++ ) {
      theDoses[ii] = 0.;
    }
  }
  
  //  G4double nevA = theHeader->GetNumberOfEvents();
  //  G4double nevB = gmdose.GetHeader()->GetNumberOfEvents();
  // G4double nevSUM = nevA + nevB;
  // G4cout << " nevSUM " << nevSUM  << " nevA  " <<  nevA << " nevB " << nevB << G4endl;
  for( size_t ii = 0; ii < siz; ii++ ) {
    //    if( ii == 0 ) G4cout << this << " dose1 " << theDoses[ii] << " " << &gmdose << " dose2 " << dose2[ii] << G4endl;
    //    theDoses[ii] = (theDoses[ii]*nevA + doses[ii]*nevB) / nevSUM;
    //   theDoseSqs[ii] = (theDoseSqs[ii]*nevA*nevA + doses2[ii]*nevB*nevB)/(nevSUM*nevSUM);
    if( ii < doses.size() ) {
      theDoses[ii] = theDoses[ii] + doses[ii];
      theDoseSqs[ii] = theDoseSqs[ii] + doses2[ii];
    }

    //    if( theDoses[ii] != 0 )  G4cout << ii << "summed dose1 " << theDoses[ii] << " dose2 " << doses[ii] << G4endl;
    // if( ii == 0)  G4cout << "summed doseSq1 " << theDoseSqs[ii] << " dose2 " << doses2[ii] << G4endl;
  }

  //  G4cout << "dose value sum " << theDoses[0] << G4endl;

  return *this;
}


//-----------------------------------------------------------------------
GmSqdose GmSqdose::operator*=( G4double factor )
{
  G4double factor2 = factor*factor;

  size_t siz = theDoses.size();
  
  for( size_t ii = 0; ii < siz; ii++ ) {
    //    if( ii == 0 ) G4cout << this << " dose1 " << theDoses[ii] << " " << &gmdose << " dose2 " << dose2[ii] << G4endl;
    //    theDoses[ii] = (theDoses[ii]*nevA + doses[ii]*nevB) / nevSUM;
    //   theDoseSqs[ii] = (theDoseSqs[ii]*nevA*nevA + doses2[ii]*nevB*nevB)/(nevSUM*nevSUM);
    theDoses[ii] *= factor;
    theDoseSqs[ii] *= factor2;

    // if( ii == 0 )  G4cout << "summed dose1 " << theDoses[ii] << " dose2 " << doses[ii] << G4endl;
    // if( ii == 0)  G4cout << "summed doseSq1 " << theDoseSqs[ii] << " dose2 " << doses2[ii] << G4endl;
  }

  //  G4cout << "dose value sum " << theDoses[0] << G4endl;

  return *this;
}


//--------------------------------------------------------------------
void GmSqdose::CalculateErrors()
{
  size_t nvox = theDoses.size();
  G4double nEvents = theHeader->GetNumberOfEvents();
  
  for( size_t ii = 0; ii < nvox; ii++ ){
    G4double error = (theDoseSqs[ii]*nEvents - theDoses[ii]*theDoses[ii]) / (nEvents-1);
    //    if( ii%1000 == 1 ) {
    /*    if( ii >1 && ii < 1000  ) {
      G4double doseSq = (theDoseErrors[ii]*theDoseErrors[ii]*nEvents*nEvents*(nEvents-1)+theDoses[ii]*theDoses[ii])/nEvents;
      G4cout << ii << " SQ FROM ERROR " << theDoseErrors[ii]<< " " << theDoses[ii] << " " << theDoseSqs[ii] << G4endl;  //GDEB
      } */

    //    if( ii%1000 == 1 ) G4cout << ii << " CalculateError " << error << " " << theDoses[ii] << " = " << theDoseSqs[ii] << G4endl; //GDEB
    if( error < 0. ) {
      if( error < -1.E-30 ) G4cerr << " !!WARNING GmSqdose::CalculateErrors negative error, set to 0. " << error << G4endl;
      error = 0.;
    } else {
      error = std::sqrt(error);
    }

    theDoseErrors.push_back( error );
    /*    if( ii >1 && ii < 1000  ) {
      G4cout << ii << " SQ FINAL ERROR " << theDoseErrors[ii]<< G4endl;  //GDEB
      } */
    
    //     G4cout << " GetError " << ii q<< " e= " << error << " S1 " << theDoseSqs[ii]*nEvents << " s2 " << theDoses[ii]*theDoses[ii] << " S3 " << (nEvents*nEvents*(nEvents-1)) << G4endl;
  }
}

//-----------------------------------------------------------------------
GmSqdose::GmSqdose(const Gm3ddose& dose3d)
{
  theType = SqTALL;

  theHeader = new GmSqdoseHeader(*(dose3d.GetHeader()));

  //  G4cout << " GmSqdose(const Gm3ddose& NV " << theHeader->GetNoVoxelX()<< " " << theHeader->GetNoVoxelY() << " " << theHeader->GetNoVoxelZ() << G4endl; //GDEB
 
  theDoses = dose3d.GetDoses();
  theDoseErrors = dose3d.GetDoseErrors();

  G4double nEvents = theHeader->GetNumberOfEvents();
  size_t nv = theDoses.size();
  for( size_t jj = 0; jj < nv; jj++ ){
    theDoseSqs.push_back( (theDoseErrors[jj]*theDoses[jj]*theDoseErrors[jj]*theDoses[jj]*nEvents*nEvents*(nEvents-1)+theDoses[jj]*theDoses[jj])/nEvents );
    theDoses[jj] *= nEvents;
    //    G4double error = (theDoseSqs[jj]*nEvents - theDoses[jj]*theDoses[jj]) / (nEvents-1);
    //    if(jj%1000 == 0 ) G4cout << " GmSqdose DOSE " << jj << " " << theDoses[jj] << " +- " << theDoseErrors[jj] << " D2 " << theDoseSqs[jj] << G4endl; //GDEB
   }

}

