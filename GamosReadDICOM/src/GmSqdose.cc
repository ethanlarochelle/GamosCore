#include "GmSqdose.hh"
#include "Gm3ddose.hh"
#include "GmReadDICOMVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include <limits>

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

  bBelowFloatPrecisionSetTo0 = GmParameterMgr::GetInstance()->GetNumericValue("GmSqdose:BelowFloatPrecisionSetTo0",1);
  //   G4cout << this << " bBelowFloatPrecisionSetTo0 " << bBelowFloatPrecisionSetTo0 << G4endl;  //GDEB
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

#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(warningVerb) ) G4cout << " TYPE " << theType << G4endl;
#endif
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
      //      if( ii > 131121 && ii < 113150 )
      //	G4cout << ii << " WRITE " << dose << " " << theDoseSqs[ii] << G4endl; //GDEB
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
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(warningVerb) ) if( ii%1000000 == 0 ) G4cout << " GmSqdose Print DOSE " << ii <<" " << dose << " " << dose2 << G4endl;
#endif
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
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(warningVerb) ) G4cout << " GmSqdose::Read type " << theType << G4endl;
#endif

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

  //  G4double valMax = 0.;
  if( theType == SqTALL || theType == SqTCROSS_PHANTOM  ) {
    for( size_t iv = 0; iv < nv; iv++ ){
      float ftmp;
      if( fread(&ftmp, sizeof(float), 1, fin) != 1) {
	G4Exception("GmSqdose::Read()",
		    "Problem reading dose ",
		    FatalErrorInArgument,
		    G4String("Reading voxel number "+GmGenUtils::itoa(iv)).c_str());
      }
      /* int ix = iv%theHeader->GetNoVoxelX(); //GDEB
      int iy = (iv/theHeader->GetNoVoxelX())%theHeader->GetNoVoxelY();
      int iz = int(iv/theHeader->GetNoVoxelX()/theHeader->GetNoVoxelY());
           if( ftmp > valMax ) {
	valMax = ftmp;
	G4cout << valMax << " MAXVAL " << iv << " : " << ix << " " << iy << " " << iz << " : " << theHeader->GetMinX()+(ix+0.5)*2.*theHeader->GetVoxelHalfX() << " " << theHeader->GetMinY()+(iy+0.5)*2.*theHeader->GetVoxelHalfY() << " "<< theHeader->GetMinZ()+(iz+0.5)*2.*theHeader->GetVoxelHalfZ() << G4endl; //GDEB
      }
      if( ftmp > 59 ) G4cout << ftmp << " VALBIG " << iv << " : " << ix << " " << iy << " " << iz << " : " << theHeader->GetMinX()+(ix+0.5)*2.*theHeader->GetVoxelHalfX() << " " << theHeader->GetMinY()+(iy+0.5)*2.*theHeader->GetVoxelHalfY() << " "<< theHeader->GetMinZ()+(iz+0.5)*2.*theHeader->GetVoxelHalfZ() << G4endl; //GDEB
       if( iz <= 34 && iz >= 32 & ftmp != 0 ) G4cout << ftmp << " VALBIG " << iv << " : " << ix << " " << iy << " " << iz << " : " << theHeader->GetMinX()+(ix+0.5)*2.*theHeader->GetVoxelHalfX() << " " << theHeader->GetMinY()+(iy+0.5)*2.*theHeader->GetVoxelHalfY() << " "<< theHeader->GetMinZ()+(iz+0.5)*2.*theHeader->GetVoxelHalfZ() << G4endl; //GDEB
      //      if( theDoses.size()%1 == 0 ) G4cout << iv << " 1READ dose " << theDoses.size() << " = " << theDoses[theDoses.size()-1] << G4endl; //GDEB
      */
      //      G4cout << theDoses.size() << " READ " << ftmp << G4endl; //GDEB
      theDoses.push_back( ftmp );
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
  G4float nev1 = theHeader->GetNumberOfEvents();
  G4float nev2 = gmdose.GetHeader()->GetNumberOfEvents();
  G4float nev1Sq = nev1*nev1;
  G4float nev2Sq = nev2*nev2;
  G4float nevT = nev1+nev2;
  G4float nevTSq = nevT*nevT;
  theHeader->SetNumberOfEvents(nevT);
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
      G4bool bError0 = 0; // if both errors are 0, set DosesSq so that new error is 0
      G4double errd1 = theDoses[ii]==0 ? 0 : theDoseSqs[ii]*nev1 - theDoses[ii]*theDoses[ii];
      //      G4cout << ii << " errd1 " << errd1 << " = " << theDoseSqs[ii] << " * " << nev1<< "  - " <<theDoses[ii]*theDoses[ii] <<  " : " << theDoses[ii]  << " :: " <<  sqrt(fabs(errd1))/theDoses[ii] << G4endl; //GDEB
      
      if( theDoses[ii] == 0 || fabs(errd1)/theDoses[ii]/theDoses[ii] < 5.e-07 ) {
	//	G4double errd2 = doses[ii]==0 ? 0 : doses2[ii]*nev2 - doses[ii]*doses[ii];      
	G4double errd2 = doses2[ii]*nev2 - doses[ii]*doses[ii];      
	//	G4cout << ii << " errd2 " << errd2 << " = " << doses2[ii] << " * " << nev2<< "  - " <<doses[ii]*doses[ii] << " : " << doses[ii] << " :: " << sqrt(fabs(errd2))/doses[ii] << " ::: " << fabs(errd2)/doses[ii]/doses[ii] << G4endl; //GDEB
	if( doses[ii] == 0 || fabs(errd2)/doses[ii]/doses[ii] < 5.e-07 ) {
	  bError0 = 1;
	}
      }
      theDoses[ii] = (theDoses[ii]*nev1 + doses[ii]*nev2)/nevT;
      if( bError0 ) {
	theDoseSqs[ii] = theDoses[ii]*theDoses[ii]/nevT;
	//	G4cout << ii << " NEW ERROR " << theDoseSqs[ii] << " " << nevT << " " << theDoseSqs[ii]*theHeader->GetNumberOfEvents()- theDoses[ii]*theDoses[ii] << " : " << theDoses[ii] << G4endl; //GDEB
      } else {
	theDoseSqs[ii] = (theDoseSqs[ii]*nev1Sq + doses2[ii]*nev2Sq)/nevTSq;
      }
      // if error=0, make error of sum = 0

    }
    //    if( theDoses[ii] != 0 )  G4cout << ii << "summed dose1 " << theDoses[ii] << " dose2 " << doses[ii] << G4endl;
    // if( ii == 0)  G4cout << "summed doseSq1 " << theDoseSqs[ii] << " dose2 " << doses2[ii] << G4endl;
  }

  //  G4cout << "dose value sum " << theDoses[0] << G4endl;

  return *this;
}

//131145 write 2.5653e-08 = 0.00506488 err 3.75219e-13   

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
    //    if(error != 0 ) G4cout << ii << " CalculateErrors error " << error << " = " << theDoseSqs[ii] << " * " << nEvents << " - " << theDoses[ii]*theDoses[ii] << " : " << theDoses[ii]<<  G4endl; // GDEB
    //    G4cerr << " FLOAT_MIN " << std::numeric_limits<float>::min() << G4endl;1.17549e-38 ?? 1.4013e-45
    //-- float precision problem: correct setting error_REL = 1 (assume dose value is very small, so error is 100%)
    if( theDoseSqs[ii] == 0. && theDoses[ii] ) {
      error = theDoses[ii]*theDoses[ii];
    }  // after this, no negative error should happen becasue of this
    // THIS HAPPENS IF THE DOSE HAS BEEN MULTIPLIED BY A NUMBER IF THE DOSE WAS VERY SMALL THAT DOSESQ WAS APPROXIMATED TO 0, AND DOSE*DOSE WAS ALSO APPROXIMATED TO 0, BUT AFTER MULTIPLICATION DOSE*DOSE <> 0  !!WARNING GmSqdose::CalculateErrors negative error, set to 0. 4384 : 0 = 0 * 3.2e+07 - 0 / 3.2e+07 : 8.80046e-26

    if( sqrt(fabs(error))/theDoses[ii] < 1.44129e-05 ) {
      //    if( bBelowFloatPrecisionSetTo0 && sqrt(fabs(error))/theDoses[ii] < 1.44129e-05 ) {
      error = 0.; // no negative error should happen
      //    G4cout << this << " " << ii << " SET ERROT TO 0 " << bBelowFloatPrecisionSetTo0 << G4endl;
    }
    //    if( theDoses[ii] != 0 )
    //-    if( error < 1.44129ee-05 ) G4cerr << (theDoseSqs[ii]*nEvents - theDoses[ii]*theDoses[ii])  << " !!WARNING GmSqdose::CalculateErrors error below float precision, should have been set to 0. " << ii << " : " << error << " = " << theDoseSqs[ii] << " * " << nEvents << " - " << theDoses[ii]*theDoses[ii] << " / " << (nEvents-1) << " : " <<  theDoses[ii] << " REL " << sqrt(fabs(error))/theDoses[ii] << " " << (sqrt(fabs(error))/theDoses[ii] < 1.-05 ) << G4endl; //GDEB
   
    //    if( ii%1000 == 1 ) G4cout << ii << " CalculateError " << error << " " << theDoses[ii] << " = " << theDoseSqs[ii] << G4endl; //GDEB
    if( error < 0. ) { // probably form DICOM2G4n when image had error =0
      //      if( error < -1.E-30 ) G4cerr << " !!WARNING GmSqdose::CalculateErrors negative error, set to 0. " << error << G4endl;
      //-- send a warning only if relative error > 1e-4
      if( theDoses[ii] == 0 || sqrt(fabs(error))/theDoses[ii] > 1.e-4 ) G4cerr << " !!WARNING GmSqdose::CalculateErrors negative error, set to 0. " << ii << " : " << error << " = " << theDoseSqs[ii] << " * " << nEvents << " - " << theDoses[ii]*theDoses[ii] << " / " << (nEvents-1) << " : " <<  theDoses[ii]  << " REL " << sqrt(fabs(error))/theDoses[ii] << G4endl;
      error = 0.;
    } else {
      error = std::sqrt(error);
    }
    
    //     if(error != 0 ) G4cout << ii << " CalculateErrors error " << error << " = " << theDoseSqs[ii] << " * " << nEvents << " - " << theDoses[ii]*theDoses[ii] << " : " << theDoses[ii]<<  G4endl; // GDEB
 
    theDoseErrors.push_back( error );
    //    if( ii%100000 == 1 ) G4cout << " DOSE GetError " << ii << " " << theDoses[ii] << " +- " << error << " " << theDoseSqs[ii] << " nev " << nEvents << G4endl;
    /*    if( ii >1 && ii < 1000  ) {
      G4cout << ii << " SQ FINAL ERROR " << theDoseErrors[ii]<< G4endl;  //GDEB
      } */
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
    theDoseSqs.push_back( (theDoseErrors[jj]*theDoses[jj]*theDoseErrors[jj]*theDoses[jj]*nEvents*nEvents*(nEvents-1)+theDoses[jj]*theDoses[jj])/nEvents/1.E3 );
    //?    theDoses[jj] *= nEvents/1.E3;
    theDoses[jj] *= nEvents;
    //    G4double error = (theDoseSqs[jj]*nEvents - theDoses[jj]*theDoses[jj]) / (nEvents-1);
    //    if(jj%1000 == 0 ) G4cout << " GmSqdose DOSE " << jj << " " << theDoses[jj] << " +- " << theDoseErrors[jj] << " D2 " << theDoseSqs[jj] << G4endl; //GDEB
   }

}

G4String GmSqdose::GetTypeStr() const
{
  switch (theType) {
  case SqTALL:
    return "SqTALL";
  case SqTFILLED:
    return SqTFILLED;
  case SqTCROSS_PHANTOM:
    return SqTCROSS_PHANTOM;
  };
  return "NOTYPE";

}
