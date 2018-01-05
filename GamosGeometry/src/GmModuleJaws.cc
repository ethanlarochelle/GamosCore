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
#include "GmModuleJaws.hh"
#include "G4tgrUtils.hh"
#include "G4tgrVolumeMgr.hh"
#include "G4UIcommand.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "GmGeomVerbosity.hh"
#include <sstream>

//------------------------------------------------------------------------
GmModuleJaws::GmModuleJaws( const std::vector<G4String>& par ) : GmVModule(par)
{
}

//------------------------------------------------------------------------
void GmModuleJaws::BuildObjects()
{
  G4int ip = 0;
  theWords["NAME"] = theParams[ip++];
  theWords["ORIENTATION"] = theParams[ip++];
  theWords["TYPE"] = theParams[ip++];
  G4cout << " GmModuleJaws::BuildObjects " << PrintW("NAME") << PrintW("ORIENTATION") << PrintW("TYPE") << G4endl;
  theWords["XDIM"] = theParams[ip++];
  theWords["YDIM"] = theParams[ip++];
  theWords["ZDIM"] = theParams[ip++];
  G4cout << " GmModuleJaws::BuildObjects " << PrintWVal("XDIM") << PrintWVal("YDIM") << PrintWVal("ZDIM") << G4endl;
  if( theWords["TYPE"] == "ROUND" ) {
    theWords["PROF_RADIUS"] = theParams[ip++];
    theWords["PROF_Z"] = "";
    theWords["HVL"] = theParams[ip++];
    G4cout << " GmModuleJaws::BuildObjects " << PrintWVal("PROF_RADIUS") << PrintWVal("PROF_Z") << PrintWVal("HVL") << G4endl;
  } else if( theWords["TYPE"] == "ROUND_DISP" ) {
    theWords["PROF_RADIUS"] = theParams[ip++];
    theWords["PROF_Z"] = theParams[ip++];
    theWords["HVL"] = theParams[ip++];
    G4cout << " GmModuleJaws::BuildObjects " << PrintWVal("PROF_RADIUS") << PrintWVal("PROF_Z") << PrintWVal("HVL") << G4endl;
  } else if( theWords["TYPE"] == "STRAIGHT" ) {
  } else {
    G4Exception("GmModuleJaws:BuildObjects",
		"MJ001",
		FatalException,
		("JAWS TYPE HAS TO BE STRAIGHT, ROUND OR ROUND_DISP, WHILE IT IS "+theWords["TYPE"]).c_str());
  }
  theWords["Z_FOCUS"] = theParams[ip++];
  theWords["RADIUS"] = theParams[ip++];
  theWords["Z_CENTRE"] = theParams[ip++];
  theWords["Z_PATIENT"] = theParams[ip++];
  G4cout << " GmModuleJaws::BuildObjects " << PrintWVal("Z_FOCUS") << PrintWVal("RADIUS") << PrintWVal("Z_CENTRE") << PrintWVal("Z_PATIENT") << G4endl;
  theWords["OPENING_NEG"] = theParams[ip++];
  theWords["OPENING_POS"] = theParams[ip++];
  G4cout << " GmModuleJaws::BuildObjects " << PrintWVal("OPENING_NEG") << PrintWVal("OPENING_POS") << G4endl;
  theWords["MATE"] = theParams[ip++];
  theWords["WORLD"] = theParams[ip++];
  G4cout << " GmModuleJaws::BuildObjects " << PrintW("MATE") << PrintW("WORLD") << G4endl;

  std::ostringstream fout;
  if( theWords["TYPE"] == "STRAIGHT" ) {
    fout << ":VOLU " << theWords["NAME"] << " BOX " 
	 << theWords["XDIM"] << " " 
	 << theWords["YDIM"] << " " 
	 << theWords["ZDIM"] << " " 
	 << theWords["MATE"];
    BuildObject( fout );
  } else if( theWords["TYPE"] == "ROUND" ||
	     theWords["TYPE"] == "ROUND_DISP" ) {
    
    fout << ":SOLID " << theWords["NAME"]+"_a" << " BOX " 
	 << theWords["XDIM"] << " " 
	 << theWords["YDIM"] << " " 
	 << theWords["ZDIM"];
    BuildObject( fout );
    
    if( theWords["ORIENTATION"] == "X" ) {
      fout << ":SOLID " << theWords["NAME"]+"_b" << " TUBE "
	   << "0. " << theWords["PROF_RADIUS"]
	   << " " << theWords["YDIM"];
      BuildObject( fout );
      fout << ":ROTM RM90X_JAWS 90. 0. 0. ";
      BuildObject( fout );
      fout << ":SOLID " << theWords["NAME"] << " INTERSECTION "
	   << theWords["NAME"]+"_a" << " "
	   << theWords["NAME"]+"_b" << " "
	   << "RM90X_JAWS " << theWords["PROF_RADIUS"] << "-" << theWords["XDIM"]
	   << " 0. "
	   << theWords["ZDIM"] << "-" << theWords["PROF_Z"];
      BuildObject( fout );
    } else if( theWords["ORIENTATION"] == "Y" ) {
      fout << ":SOLID " << theWords["NAME"]+"_b" << " TUBE "
	   << "0. " << theWords["PROF_RADIUS"]
	   << " " << theWords["YDIM"];
      BuildObject( fout );
      fout << ":ROTM RM90Y_JAWS 0. 90. 0. ";
      BuildObject( fout );
      fout << ":SOLID " << theWords["NAME"] << " INTERSECTION "
	   << theWords["NAME"]+"_a" << " "
	   << theWords["NAME"]+"_b" << " "
	   << "RM90Y_JAWS 0. "
	   << theWords["PROF_RADIUS"] << "-" << theWords["YDIM"] << " "
	   << theWords["ZDIM"] << "-" << theWords["PROF_Z"];
      BuildObject( fout );
    }
    
    fout << ":VOLU " << theWords["NAME"]
	 << " "  << theWords["NAME"]
	 << " "  << theWords["MATE"];
    BuildObject( fout );
  }

  G4double angle_1 = atan(G4tgrUtils::GetDouble(theWords["OPENING_NEG"]) / 
			  (G4tgrUtils::GetDouble(theWords["Z_PATIENT"]) - 
			   G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) ) );
  G4double angle_2 = atan(G4tgrUtils::GetDouble(theWords["OPENING_POS"]) / 
			   (G4tgrUtils::GetDouble(theWords["Z_PATIENT"]) - 
			    G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) ) );

  G4double radius = G4tgrUtils::GetDouble(theWords["RADIUS"]);
  G4double posZ = G4tgrUtils::GetDouble(theWords["Z_CENTRE"]);
  
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " RADIUS " << radius << " angle_1 " << angle_1 << " angle_2 " << angle_2 << " " << radius*sin(angle_1) << " " << radius*cos(angle_1) << " " << radius*sin(angle_2) << " " << radius*cos(angle_2) <<  G4endl;  
#endif
  G4String rotname = "RM_" + theWords["NAME"] ;
  
  if( theWords["ORIENTATION"] == "X" ) {
    G4double XDIM = G4tgrUtils::GetDouble(theWords["XDIM"]);
    G4double thetaX = asin(XDIM/radius);
    
    //--- Build first jaw    
    fout << ":ROTM " << rotname << "_1 " 
	 << " 0. " << -angle_1/CLHEP::deg << " 0. ";
    BuildObject( fout );
    
    fout << ":PLACE " << theWords["NAME"] << " 1 " 
	 << theWords["WORLD"] << " " 
 	 << rotname << "_1 "
	 << radius*sin(angle_1) - XDIM*cos(angle_1)
	 << " 0. " 
      //- 	 << G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) + radius/cos(thetaX)*cos(angle_1-thetaX);
 	 << posZ+(radius/cos(thetaX)*cos(angle_1-thetaX)-radius);
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) )  G4cout << " RADIUS POS Z " << G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) + radius*cos(angle_1) - XDIM*sin(angle_1) << " " 
				    << G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) << " " << radius*cos(angle_1) << " " << XDIM*sin(angle_1) << G4endl;
#endif

    BuildObject( fout );

    //--- Build second jaw    
    fout << ":ROTM " << rotname << "_2 " 
	 << " 0. " << -angle_2/CLHEP::deg << " 0. " ;
    BuildObject( fout );

    fout << ":PLACE " << theWords["NAME"] << " 2 " 
	 << theWords["WORLD"] << " " 
	 << rotname << "_2 "
	 << radius*sin(angle_2) + XDIM*cos(angle_2)
	 << " 0. " 
 	 << posZ+( radius/cos(thetaX)*cos(angle_2+thetaX) - radius);
    BuildObject( fout );
  } else 
  if( theWords["ORIENTATION"] == "Y" ) {
    G4double YDIM = G4tgrUtils::GetDouble(theWords["YDIM"]);
    G4double thetaY = asin(YDIM/radius);

    //--- Build first jaw    
    fout << ":ROTM " << rotname << "_1 " 
	 << angle_1/CLHEP::deg << " 0. 0. " ;
    BuildObject( fout );
    
    fout << ":PLACE " << theWords["NAME"] << " 1 " 
	 << theWords["WORLD"] << " " 
	 << rotname << "_1 "
	 << " 0. " 
	 << radius*sin(angle_1) - YDIM*cos(angle_1) << " " 
      // 	 << G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) + radius*cos(angle_1) + YDIM*tan(angle_1); 
 	 << posZ+ (radius/cos(thetaY)*cos(angle_1-thetaY) - radius);
    BuildObject( fout );

    //--- Build second jaw    
    fout << ":ROTM " << rotname << "_2 " 
	 << angle_2/CLHEP::deg-180. << " 0. 0. " ;
    BuildObject( fout );

    fout << ":PLACE " << theWords["NAME"] << " 2 " 
	 << theWords["WORLD"] << " " 
	 << rotname << "_2 "
	 << " 0. " 
	 << radius*sin(angle_2) + YDIM*cos(angle_2) << " " 	
 	 << posZ + (radius/cos(thetaY)*cos(angle_2+thetaY) - radius);
    BuildObject( fout );

  } else {
    G4Exception("GmModuleJaws::BuildObjects",
		"Wrong type of JAWS",
		FatalErrorInArgument,
		G4String("It is: "+theWords["ORIENTATION"]+" , while it should be X or Y").c_str());
  }

}

//------------------------------------------------------------------------
GmModuleJaws::~GmModuleJaws()
{
}
