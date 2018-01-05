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
#include "GmModuleMLC.hh"
#include "G4tgrUtils.hh"
#include "G4tgrVolumeMgr.hh"
#include "G4UIcommand.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "GmGeomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <math.h>

//#include <stdlib.h>
//#include <stdio.h>
//#include <string>

//------------------------------------------------------------------------
GmModuleMLC::GmModuleMLC( const std::vector<G4String>& par ) : GmVModule(par)
{
  //G4tgrUtils::CheckWLsize( par, 310, WLSIZE_EQ,
  //                         "GmModuleMLC::GmModuleMLC");

}

//------------------------------------------------------------------------
void GmModuleMLC::BuildObjects()
{
  G4int ip = 0;
  G4cout << "Starting to process MLC" << G4endl;

//---
  theWords["NAME"] = theParams[ip++];
  
  theWords["TYPE"]         = theParams[ip++];          // FOCUSED 
  theWords["ORIENTATION"]  = theParams[ip++];   // X or Y leaf move orientation
  theWords["ENDLEAFTYPE"] = theParams[ip++];  // ROUND, ROUND_DISP or STRAIGHT end leaf

  G4cout << " GmModuleMLC::BuildObjects " << PrintW("NAME") << PrintW("TYPE") << PrintW("ORIENTATION")  << PrintW("ENDLEAFTYPE") << G4endl;

  theWords["LONGDIM"]      = theParams[ip++];  G4double LONGDIM    = G4tgrUtils::GetDouble(theWords["LONGDIM"]);
  G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("LONGDIM") << G4endl;
  G4double ENDL_RADIUS;
  G4double ENDL_CIRCLE_Z;
  if( theWords["ENDLEAFTYPE"] == "ROUND" ){
    theWords["ENDL_RADIUS"] = theParams[ip++];
    ENDL_RADIUS  = G4tgrUtils::GetDouble(theWords["ENDL_RADIUS"]);
    G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("ENDL_RADIUS") << G4endl;
    theWords["HVL"]        = theParams[ip++];  G4double HVL      = G4tgrUtils::GetDouble(theWords["HVL"]);
    G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("HVL") << G4endl;
  }
  theWords["ENDL_CIRCLE_Z"] = "0";
  if( theWords["ENDLEAFTYPE"] == "ROUND_DISP" ){
    theWords["ENDL_RADIUS"] = theParams[ip++];
    ENDL_RADIUS  = G4tgrUtils::GetDouble(theWords["ENDL_RADIUS"]);
    G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("ENDL_RADIUS") << G4endl;
    theWords["ENDL_CIRCLE_Z"] = theParams[ip++];
    G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("ENDL_CIRCLE_Z") << G4endl;
    theWords["HVL"]        = theParams[ip++];  G4double HVL      = G4tgrUtils::GetDouble(theWords["HVL"]);
    G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("HVL") << G4endl;
  };

  theWords["Z_FOCUS"]      = theParams[ip++];  G4double Z_FOCUS    = G4tgrUtils::GetDouble(theWords["Z_FOCUS"]);
  theWords["C_FOCUS"]       = theParams[ip++]; G4double C_FOCUS        = G4tgrUtils::GetDouble(theWords["C_FOCUS"]);
  theWords["Z_ISOCENTER"]  = theParams[ip++];  G4double Z_ISOCENTER= G4tgrUtils::GetDouble(theWords["Z_ISOCENTER"]);
  G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("Z_FOCUS") << PrintWVal("C_FOCUS") << PrintWVal("Z_ISOCENTER") << G4endl;

  theWords["Z_TOP"]        = theParams[ip++];  G4double Z_TOP      = G4tgrUtils::GetDouble(theWords["Z_TOP"]);
  G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("Z_TOP") << G4endl;
  
  theWords["Z_GAP"]        = theParams[ip++];  G4double Z_GAP      = G4tgrUtils::GetDouble(theWords["Z_GAP"]);
  theWords["GAP"]        = theParams[ip++];  G4double GAP      = G4tgrUtils::GetDouble(theWords["GAP"]);
  theWords["CROSS_START_POINT"]        = theParams[ip++];  G4double CROSS_START_POINT      = G4tgrUtils::GetDouble(theWords["CROSS_START_POINT"]);
  G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("GAP") << PrintWVal("Z_GAP") << PrintWVal("CROSS_START_POINT") << G4endl;

  //  theWords["Z_PROFILE"]        = theParams[ip++];  G4double Z_PROFILE      = G4tgrUtils::GetDouble(theWords["Z_PROFILE"]);
  //  G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("Z_PROFILE") << G4endl;
    
  G4String aux_text;
  G4String aux_text_c;
  G4String aux_text_z; 
  
  theWords["N_LEAVES_CROSS_PROFILES"] = theParams[ip++]; G4double N_LEAVES_CROSS_PROFILES= G4tgrUtils::GetDouble(theWords["N_LEAVES_CROSS_PROFILES"]);
    G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("N_LEAVES_CROSS_PROFILES") << G4endl;

  for(int ii=1;ii<int(1+N_LEAVES_CROSS_PROFILES);ii++){
    
    theWords["LeafMode["+GmGenUtils::itoa(ii)+"]"] = theParams[ip++];
    theWords["NLeavesCrossPoints["+GmGenUtils::itoa(ii)+"]"] = theParams[ip++];
    int nLCPoints = int(G4tgrUtils::GetDouble(theWords["NLeavesCrossPoints["+GmGenUtils::itoa(ii)+"]"]));
    //    G4cout << " NLeavesCrossPoints " << nLCPoints << G4endl;
  G4cout << " GmModuleMLC::BuildObjects " << PrintW("LeafMode["+GmGenUtils::itoa(ii)+"]") << PrintWVal("NLeavesCrossPoints["+GmGenUtils::itoa(ii)+"]") << G4endl;
    for(int jj=1; jj<nLCPoints+1; jj++){	
      //      G4cout << " ***** READING PARAMETER ********* " ;
      theWords["z["+GmGenUtils::itoa(ii)+","+GmGenUtils::itoa(jj)+"]"] = theParams[ip++];
      theWords["c["+GmGenUtils::itoa(ii)+","+GmGenUtils::itoa(jj)+"]"] = theParams[ip++];
      
      G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("z["+GmGenUtils::itoa(ii)+","+GmGenUtils::itoa(jj)+"]") << PrintWVal("c["+GmGenUtils::itoa(ii)+","+GmGenUtils::itoa(jj)+"]") << G4endl;
    };
  };
  
  ///--- input lines to Leaf numbers 
  theWords["N_LEAVES"] = theParams[ip++]; G4double N_LEAVES = G4tgrUtils::GetDouble(theWords["N_LEAVES"]);
  G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("N_LEAVES") << G4endl;
  
  for(int ii=1;ii<int(1+N_LEAVES);ii++){
    
    ///--- input lines to leaf type and open position 
    theWords["LEAF_TYPE["+GmGenUtils::itoa(ii)+"]"]          = theParams[ip++];		
    theWords["OPEN_POSITION_NEG["+GmGenUtils::itoa(ii)+"]"]  = theParams[ip++]; 
    theWords["OPEN_POSITION_POS["+GmGenUtils::itoa(ii)+"]"]  = theParams[ip++];
    G4cout << " GmModuleMLC::BuildObjects " << PrintW("LEAF_TYPE["+GmGenUtils::itoa(ii)+"]") << PrintWVal("OPEN_POSITION_NEG["+GmGenUtils::itoa(ii)+"]") << PrintWVal("OPEN_POSITION_POS["+GmGenUtils::itoa(ii)+"]") << G4endl;
  };
  
  theWords["MATE"] = theParams[ip++];
  theWords["WORLD"]= theParams[ip++];
  G4cout << " GmModuleMLC::BuildObjects " << PrintW("MATE") << PrintW("WORLD") << G4endl;

  G4cout << "ALL MODULE READ OK" << G4endl << "*********____________*********" << G4endl << G4endl;

  //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  
  std::ostringstream fout;
  
  G4double cg_min=0; // min cross coordinate proyected in Z_GAP
  G4double cg_max=0; // max cross coordinate proyected in Z_GAP
  G4double cg_min_prev=0;
  G4double cg_max_prev=0;	
 
  G4double c_min=0;
  G4double c_max=0;
  G4double z_min=0;	 
  G4double z_max=0;	 
  
  G4double c1=0;
  G4double c2=0;
  G4double z1=0;
  G4double z2=0;
  G4double C_MIN_POS = CROSS_START_POINT;
  
  std::string  text_aux="";
  std::string  output_line_text="";

  //@@@@@  Build rotation matrices
  std::string R_90x = theWords["NAME"]+"_R_90x";
  std::string R_90y = theWords["NAME"]+"_R_90y";
  std::string R00   = theWords["NAME"]+"_R00";
  std::string R_270x_90z = theWords["NAME"]+"_R_270x_90z";
  std::string R_90y_180z = theWords["NAME"]+"_R_90y_180z";
  std::string R_aux;
  
  fout  << ":ROTM  "<<R_90x<<" 90  0 0 ";  BuildObject( fout );
  fout  << ":ROTM  "<<R_90y<<"  0 90 0 ";  BuildObject( fout );
  fout  << ":ROTM  "<<R00  <<"  0  0 0 ";  BuildObject( fout );
  fout  << ":ROTM  "<<R_270x_90z  <<"  0 0 -1 1  0 0 0 -1 0 ";  BuildObject( fout );
  fout  << ":ROTM  "<<R_90y_180z  <<"  0 0 -1 0 -1 0 -1 0 0 ";  BuildObject( fout );
  
  Z_GAP += Z_TOP;
  //@@@@@ Loop to leaves  
  for(int ii=1;ii<int(1+N_LEAVES);ii++){

    cg_min = DBL_MAX; // min/max leaf cross coordinates projected in Z_PROFILE
    //    cg_max = -DBL_MAX;
    c_min = DBL_MAX; // min/max leaf cross coordinates
    c_max = -DBL_MAX;
    z_min = DBL_MAX; // min/max leaf Z coordinates projected in Z_PROFILE
    z_max = -DBL_MAX;
    G4cout << G4endl << " *********************************************************************************************" << G4endl;
    G4cout << "Start to process leaf " << ii << " of " << theWords["N_LEAVES"]  << " Leaves ************ " << G4endl;	
    G4cout << " Number of Leaf Cross Points = " << theWords["NLeavesCrossPoints["+theWords["LEAF_TYPE["+GmGenUtils::itoa(ii)+"]"]+"]"] << G4endl;
    G4cout << "  LEAF TYPE =  " << theWords["LEAF_TYPE["+GmGenUtils::itoa(ii)+"]"]  <<   G4endl ;	
    //    G4cout << "    It's going to use LEAF_TYPE["+GmGenUtils::itoa(ii)+"] " <<  G4endl <<  G4endl ;
    
    aux_text="NLeavesCrossPoints["+theWords["LEAF_TYPE["+GmGenUtils::itoa(ii)+"]"]+"]";
    output_line_text.clear();
    
    //@@@ FIND intersection of Z_TOP intersection points of leaf profile lines
    int nLCPoints = int(G4tgrUtils::GetDouble(theWords["NLeavesCrossPoints["+theWords["LEAF_TYPE["+GmGenUtils::itoa(ii)+"]"]+"]"] ));
    for(int jj = 1;jj <= nLCPoints; jj++){
      int jj2 = jj+1;
      if( jj == nLCPoints ) jj2 = 1;
      //    for(int jj=1;jj<int(-1+1+G4tgrUtils::GetDouble(theWords[aux_text]));jj++){ 
      c1 = G4tgrUtils::GetDouble(theWords["c["+theWords["LEAF_TYPE["+GmGenUtils::itoa(ii)+"]"]+","+GmGenUtils::itoa(jj)+"]"]);
      c2 = G4tgrUtils::GetDouble(theWords["c["+theWords["LEAF_TYPE["+GmGenUtils::itoa(ii)+"]"]+","+GmGenUtils::itoa(jj2)+"]"]);
      z1 = G4tgrUtils::GetDouble(theWords["z["+theWords["LEAF_TYPE["+GmGenUtils::itoa(ii)+"]"]+","+GmGenUtils::itoa(jj)+"]"]);
      z2 = G4tgrUtils::GetDouble(theWords["z["+theWords["LEAF_TYPE["+GmGenUtils::itoa(ii)+"]"]+","+GmGenUtils::itoa(jj2)+"]"]);
      z1 = z1+Z_TOP;
      z2 = z2+Z_TOP;
      G4cout << jj << " points " << z1 << " " << c1 << " AND " << z2 << " " << c2 << G4endl; //GDEB
      //      if(  ((Z_PROFILE >= z1) && (Z_PROFILE < z2 ))  ||  ((Z_PROFILE <= z1) && (Z_PROFILE > z2 )) ){ 
      if( z1 != z2 ) {
	//-      c1 = (C_LEAF+c1-cg_min-C_FOCUS)*(z1-Z_FOCUS)/(Z_ISOCENTER-Z_FOCUS);

	G4double inters = c1+((c2-c1)/(z2-z1))*(Z_GAP-z1);
	cg_min = std::min(cg_min,inters);
	cg_max = std::max(cg_max,inters);
	//		v[kk]=c+((c_pi-c)/(z_pi-z))*(Z_PROFILE-z);
	G4cout << jj << " cg_min " << cg_min << " cg_max " << cg_max << G4endl;
      }
    }

    if( ii != 1 ) C_MIN_POS += GAP + cg_max_prev-cg_min_prev; 

    G4cout <<  ii << " +++++  STARTING TO CALCULATE DE FINAL C POSITION +++++++++++++++ " << C_MIN_POS << " + " << GAP << " + " << cg_max_prev << " - " << cg_min_prev << G4endl;
    for(int jj = 1;jj <= nLCPoints; jj++){
      c1 = G4tgrUtils::GetDouble(theWords["c["+theWords["LEAF_TYPE["+GmGenUtils::itoa(ii)+"]"]+","+GmGenUtils::itoa(jj)+"]"]);
      z1 = G4tgrUtils::GetDouble(theWords["z["+theWords["LEAF_TYPE["+GmGenUtils::itoa(ii)+"]"]+","+GmGenUtils::itoa(jj)+"]"]);
      //-      z1 += Z_TOP;
      c1 += C_MIN_POS - cg_min;
      G4cout << jj << " points " << z1 << " " << c1 << G4endl; //GDEB

      c1 = (c1-C_FOCUS)*(z1+Z_TOP-Z_FOCUS)/(Z_GAP-Z_FOCUS)+C_FOCUS;

      G4cout << jj << " c1 " << c1 << " " <<  C_FOCUS << " + " << (c1-C_FOCUS) << " / " << (z1+Z_TOP-Z_FOCUS) << " * " << (Z_GAP-Z_FOCUS) << G4endl;
        
    //@@@ Recalculate cross points projecting the profile so that the lines start at point (Z_FOCUS,C_FOCUS), and points are displaced by C_LEAF

      G4cout << ii << " " << jj << " FINAL LEAF POS " << z1 << " , " << c1 << " FROM " << G4tgrUtils::GetDouble(theWords["z["+theWords["LEAF_TYPE["+GmGenUtils::itoa(ii)+"]"]+","+GmGenUtils::itoa(jj)+"]"]) << " , " << G4tgrUtils::GetDouble(theWords["c["+theWords["LEAF_TYPE["+GmGenUtils::itoa(ii)+"]"]+","+GmGenUtils::itoa(jj)+"]"]) << G4endl;

      if( theWords["ENDLEAFTYPE"]=="ROUND" || theWords["ENDLEAFTYPE"]=="ROUND_DISP"){
	output_line_text+=" "+GmGenUtils::ftoa(z1)+" "+GmGenUtils::ftoa(c1)+" ";
      } else if (theWords["ENDLEAFTYPE"]=="STRAIGHT"){
	output_line_text+=" "+GmGenUtils::ftoa(z1)+" "+GmGenUtils::ftoa(c1)+" ";		
      };
      
      //      G4cout << "output_line_text == " << output_line_text  << G4endl;
      //fout 	<<  GmGenUtils::itoa(z) << " "
      //	<<  GmGenUtils::itoa(c) << " ";
      c_min = std::min( c_min, c1 );
      c_max = std::max( c_max, c1 );
      z_min = std::min( z_min, z1 );
      z_max = std::max( z_max, z1 );
    };
    
    cg_max_prev=cg_max;
    cg_min_prev=cg_min;
    G4cout << ii << " C_MIN " << c_min << " C_MAX " << c_max << " Z_MIN " << z_min << "Z_MAX " << z_max << G4endl;

    G4cout  << "@@@ PLACING " << ii << " LEAF " << G4endl;	  
    ///---- ROUNDER BORDER    
    if( theWords["ENDLEAFTYPE"]=="ROUND" || theWords["ENDLEAFTYPE"]=="ROUND_DISP"){    
      //@@@ UNION OF LEAF PROFILE AND TUBE FOR ROUNDED EDGE
      //@@ PROFILE:
      text_aux=":SOLID "+theWords["NAME"]+"_LeafNumber_"+GmGenUtils::itoa(ii)+"_BASE_POS  EXTRUDED "+theWords[aux_text]+" "+output_line_text +" 2 "
	+"-1*"+theWords["LONGDIM"]+"/2 0  0  1 "
	+theWords["LONGDIM"]+"/2 0  0  1 ";
      fout << text_aux;
      BuildObject( fout );
      
      text_aux=":SOLID "+theWords["NAME"]+"_LeafNumber_"+GmGenUtils::itoa(ii)+"_BASE_NEG  EXTRUDED "+theWords[aux_text]+" "+output_line_text+" 2 "
	+"-1*"+theWords["LONGDIM"]+"/2 0  0  1 "
	+theWords["LONGDIM"]+"/2 0  0  1 ";
      fout   << text_aux  ;
      BuildObject( fout );
      // G4cout << " EXTRUDED B LEAF END" << G4endl;

      //@@ TUBE:
      fout << ":SOLID "<<theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_TUBE  TUBE 0 "<<theWords["ENDL_RADIUS"]<<"  "<<GmGenUtils::ftoa((c_max-c_min)/2+20); 
      BuildObject( fout );

      G4String tubeSolidPos = "_TUBE";
      G4String tubeSolidNeg = "_TUBE";
      if( LONGDIM > ENDL_RADIUS ) {  // IF TUBE RADIUS IS SMALLER THAN LEAF LENGTH, IT NEEDS TO BE ADDED A BOX SO THAT THE INTERSECTION COVERS THE LEAF PROFILE
	//	fout << ":SOLID "<<theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_BOX   BOX    "<<" "<<theWords["ENDL_RADIUS"]<<" "<<GmGenUtils::ftoa(LONGDIM/2-ENDL_RADIUS/2)<<" "<<GmGenUtils::ftoa((c_max-c_min)/2+10);
	fout << ":SOLID "<<theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_BOX   BOX    "
	     <<" "<<theWords["ENDL_RADIUS"]<<" "
	     <<GmGenUtils::ftoa(LONGDIM/2-ENDL_RADIUS/2)<<" "
	     <<GmGenUtils::ftoa((c_max-c_min)/2+20);
	BuildObject( fout );  
      
	fout << ":SOLID "<<theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_TUBEBOX_POS UNION "<<theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_TUBE "<<theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_BOX "<< R00 <<" 0 "<<GmGenUtils::ftoa((LONGDIM/2-ENDL_RADIUS/2))<<" 0";
	BuildObject( fout );  
	
	fout << ":SOLID "<<theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_TUBEBOX_NEG UNION "<<theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_TUBE "<<theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_BOX "<< R00 <<" 0 -1*("<<GmGenUtils::ftoa((LONGDIM/2-ENDL_RADIUS/2))<<") 0";
	BuildObject( fout );  
	tubeSolidPos = "_TUBEBOX_POS";
	tubeSolidNeg = "_TUBEBOX_NEG";
      }
    
      //@@@ FINAL INTERSECTION
      fout << ":SOLID "  << theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_POS INTERSECTION  " 
	   << theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_BASE_POS " 
	   << " "
	   << theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<< tubeSolidPos << " "
	//<< R00 << " 0 0 0 "; 
	//	   << R_90x << " 0. " << (c_max+c_min)/2 << " " << GmGenUtils::ftoa(ENDL_RADIUS-LONGDIM/2) << " ";
	   << R_90x << " " << z_min+ENDL_CIRCLE_Z << " " << (c_max+c_min)/2 << " " << GmGenUtils::ftoa(-ENDL_RADIUS+LONGDIM/2) << " ";
      //	   << R_90x << " " << z_max/2 << " " << (c_max+c_min)/2+z_max/2.-ENDL_CIRCLE_Z << " " << GmGenUtils::ftoa(-ENDL_RADIUS+LONGDIM/2) << " ";
      //	   << rotmLeaf << " " << GmGenUtils::ftoa(Z_TOP+(z_max-Z_TOP)/2.0) << " " << (cg_max+cg_min)/2 << " " << GmGenUtils::ftoa(LONGDIM/2+ENDL_RADIUS/2) << " ";
      BuildObject( fout );

      fout << ":SOLID "  << theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_NEG INTERSECTION  " 
	   << theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_BASE_NEG " 
	   << " "
	   << theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<< tubeSolidNeg << " "
	//<< R00 << " 0 0 0 "; BuildObject( fout ); 
	   << R_90x << " " << z_min+ENDL_CIRCLE_Z << " " << (c_max+c_min)/2 <<" "<<GmGenUtils::ftoa(ENDL_RADIUS-LONGDIM/2) << " ";
      //	   << R_90x << " " << z_max/2 << " " << (c_max+c_min)/2+z_max/2.-ENDL_CIRCLE_Z <<" "<<GmGenUtils::ftoa(ENDL_RADIUS-LONGDIM/2) << " ";
      BuildObject( fout );

      //@@@  FINAL VOLUME DEFINITION
      
      fout << ":VOLU  "  << theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_POS "<<theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_POS "<<theWords["MATE"];
      BuildObject( fout );
      
      fout << ":VOLU  "  << theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_NEG "<<theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_NEG "<<theWords["MATE"];
      BuildObject( fout );
      
      G4cout << "Making the final volume" << G4endl;
      if(theWords["ORIENTATION"]=="x" || theWords["ORIENTATION"]=="X"){
	R_aux=R_90y;
      } else {
	R_aux=R_270x_90z;
      }
      
      G4double openingPos = G4tgrUtils::GetDouble(theWords["OPEN_POSITION_POS["+GmGenUtils::itoa(ii)+"]"]);
      
      G4double Z_CENTER = Z_TOP+(z_max-z_min)/2.;
      fout << ":PLACE " << theWords["NAME"] << "_LeafNumber_"<< GmGenUtils::itoa(ii) << "_POS 1 "
	   << theWords["WORLD"] << " "
	   << R_aux << " "
	   << LONGDIM/2.+openingPos*(Z_CENTER-Z_FOCUS)/(Z_ISOCENTER-Z_FOCUS)
	//<< GmGenUtils::ftoa(1.0*( ENDL_RADIUS+((-1.0)*A/Z_ISOCENTER)*( (Z_TOP+(z_max-Z_TOP)/2.0) - ENDL_RADIUS*(-1.0)*A/( sqrt(pow(Z_ISOCENTER,2)+pow(A,2)) ) )  -  sqrt(  pow(ENDL_RADIUS,2) -  pow(A*ENDL_RADIUS,2)/( (pow(ENDL_RADIUS,2)+pow(A,2)) ) ) ))
	   << " 0 "
	   << Z_TOP;
      BuildObject(fout );
      //      G4cout << " Z_TOP " << Z_TOP << " z_max " << z_max << " " <<  ((Z_TOP+z_max)/2.) << " " << GmGenUtils::ftoa((Z_TOP+z_max)/2.) <<G4endl;
      //      B= G4tgrUtils::GetDouble(theWords["OPEN_POSITION_POS["+GmGenUtils::itoa(ii)+"]"]);
      
      G4double openingNeg =G4tgrUtils::GetDouble(theWords["OPEN_POSITION_NEG["+GmGenUtils::itoa(ii)+"]"]);
      fout << ":PLACE " << theWords["NAME"] << "_LeafNumber_" <<GmGenUtils::itoa(ii) << "_NEG 1 "
	   << theWords["WORLD"] << " "
	   << R_aux << " "
	   << -LONGDIM/2.+openingNeg*(Z_CENTER-Z_FOCUS)/(Z_ISOCENTER-Z_FOCUS)
	//	   << GmGenUtils::ftoa(1.0*(-1.0*ENDL_RADIUS+(B/Z_ISOCENTER)*( (Z_TOP+(z_max-Z_TOP)/2.0)  +  ENDL_RADIUS*B/( sqrt(pow(Z_ISOCENTER,2)+pow(B,2)) ) ) + sqrt(  pow(ENDL_RADIUS,2) -  pow(B*ENDL_RADIUS,2)/( (pow(ENDL_RADIUS,2)+pow(B,2)) ) ) ))
	   << " 0 "
	   << Z_TOP;
      BuildObject(fout );
      
      //		system("sleep 2");
      G4cout << "DONE" << G4endl;
      
      ///---- ROUNDER STRAIGHT
    } else if (theWords["ENDLEAFTYPE"]=="STRAIGHT"){	      
      
      if(theWords["ORIENTATION"]=="x" || theWords["ORIENTATION"]=="X"){R_aux=R_90x;} else { R_aux=R_270x_90z;};
      
      ///--- LEAVES A
      fout<<":VOLU "<<theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_POS  POLYCONE "
	  <<" 270"<<"+("<<theWords["OPEN_POSITION_POS["+GmGenUtils::itoa(ii)+"]"] << ")*"
	  << GmGenUtils::ftoa(Z_TOP/Z_ISOCENTER) << " "
	  << GmGenUtils::ftoa((180/3.1416)*LONGDIM/(Z_TOP-Z_FOCUS))<<" "
	  << theWords[aux_text] << " "
	  << output_line_text << " "
	  << theWords["MATE"]; 
      BuildObject( fout );
      fout << ":PLACE " <<  theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_POS  1 " <<  theWords["WORLD"]<<" "<<R_aux 
	   << " 0 0 0 "; BuildObject(fout );
      

      ///--- LEAVES B
      fout<<":VOLU "<<theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_NEG  POLYCONE "
	  <<" 270"<<"+("<<theWords["OPEN_POSITION_NEG["+GmGenUtils::itoa(ii)+"]"]<< ")*"<<GmGenUtils::ftoa(Z_TOP/Z_ISOCENTER)<<"-"<<GmGenUtils::ftoa(1*(180/3.1416)*LONGDIM/(Z_TOP-Z_FOCUS))  <<" "      <<GmGenUtils::ftoa(1*(180/3.1416)*LONGDIM/(Z_TOP-Z_FOCUS))<<" "<<theWords[aux_text]<<" "
	  <<output_line_text<<" "<<theWords["MATE"]; 
      BuildObject( fout );
      fout << ":PLACE " <<  theWords["NAME"]<<"_LeafNumber_"<<GmGenUtils::itoa(ii)<<"_NEG 1 " <<  theWords["WORLD"]<<" "<<R_aux 
	   << " 0 0 0 "; BuildObject(fout );  
      
    }
    
  };

  return;

/*
  std::ostringstream fout;
  fout << ":VOLU " << theWords["NAME"] << " BOX " 
       << theWords["XDIM"] << " " 
       << theWords["YDIM"] << " " 
       << theWords["ZDIM"] << " " 
       << theWords["MATE"];
  BuildObject( fout );

  G4double angle_1 = atan(G4tgrUtils::GetDouble(theWords["PROJ_1"]) / 
			(G4tgrUtils::GetDouble(theWords["Z_PATIENT"]) - 
			 G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) ) );
  G4double angle_2 = -atan(G4tgrUtils::GetDouble(theWords["PROJ_2"]) / 
			(G4tgrUtils::GetDouble(theWords["Z_PATIENT"]) - 
			 G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) ) );
  G4double radius = G4tgrUtils::GetDouble(theWords["Z_TOP"]) - 
    G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) + 
    G4tgrUtils::GetDouble(theWords["ZDIM"]);

#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " RADIUS " << radius << " angle_1 " << angle_1 << " angle_2 " << angle_2 << " " << radius*sin(angle_1) << " " << radius*cos(angle_1) << " " << radius*sin(angle_2) << " " << radius*cos(angle_2) <<  G4endl;  
#endif
  G4String world = G4tgrVolumeMgr::GetInstance()->GetTopVolume()->GetName();
  G4String rotname = "RM_" + theWords["NAME"] ;
  
  if( theWords["TYPE"] == "X" ) {
    G4double XDIM = G4tgrUtils::GetDouble(theWords["XDIM"]);

    //--- Build first jaw    
    fout << ":ROTM " << rotname << "_1 " 
	 << " 0. " << -angle_1/deg << " 0. ";
    BuildObject( fout );
    
    fout << ":PLACE " << theWords["NAME"] << " 1 " 
	 << world << " " 
 	 << rotname << "_1 "
	 << radius*sin(angle_1) + XDIM*cos(angle_1)
	 << " 0. " 
	 << radius*cos(angle_1) - XDIM*sin(angle_1); 
    BuildObject( fout );

    //--- Build second jaw    
    fout << ":ROTM " << rotname << "_2 " 
	 << " 0. " << -angle_2/deg << " 0. " ;
    BuildObject( fout );

    fout << ":PLACE " << theWords["NAME"] << " 2 " 
	 << world << " " 
	 << rotname << "_2 "
	 << radius*sin(angle_2) - XDIM*cos(angle_2)
	 << " 0. " 
	 << radius*cos(angle_2) + XDIM*sin(angle_2);
    BuildObject( fout );
  } else 
  if( theWords["TYPE"] == "Y" ) {
    G4double YDIM = G4tgrUtils::GetDouble(theWords["YDIM"]);

    //--- Build first jaw    
    fout << ":ROTM " << rotname << "_1 " 
	 << angle_1/deg << " 0. 0. " ;
    BuildObject( fout );
    
    fout << ":PLACE " << theWords["NAME"] << " 1 " 
	 << world << " " 
	 << rotname << "_1 "
	 << " 0. " 
	 << radius*sin(angle_1) + YDIM*cos(angle_1) << " " 
	 << radius*cos(angle_1) - YDIM*sin(angle_1); 
    BuildObject( fout );

    //--- Build second jaw    
    fout << ":ROTM " << rotname << "_2 " 
	 << angle_2/deg << " 0. 0. " ;
    BuildObject( fout );

    fout << ":PLACE " << theWords["NAME"] << " 2 " 
	 << world << " " 
	 << rotname << "_2 "
	 << " 0. " 
	 << radius*sin(angle_2) - YDIM*cos(angle_2) << " " 
	 << radius*cos(angle_2) + YDIM*sin(angle_2); 
    BuildObject( fout );

  } else {
    G4Exception("GmModuleMLC::BuildObjects",
		"Wrong type of JAWS",
		FatalErrorInArgument,
		G4String("It is: "+theWords["TYPE"]+" , while it should be X or Y").c_str());
  }

*/

}

//------------------------------------------------------------------------
GmModuleMLC::~GmModuleMLC()
{

}


/*
std::string num2string(std::int i){
      std::string aux;
      std::string aux2=".";
      std::char caux[80]="";
       if (i>999){
           gcvt(i,10,caux);
        } else {
         gcvt(i,3,caux);
        };
       aux=caux;
            std::int longitud=aux.length();
            aux=aux.substr(longitud,1);
       std::int longitud=aux.size();
       if (aux[longitud-1]==aux2[0]){
                              aux=aux.substr(0,longitud-1);};
       return aux;
};
*/
