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
#include "GmGamosVerboseByEventUA.hh"
#include "G4Track.hh"
#include "G4Event.hh"

#include "GmGamosVerboseByEventUA.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmVerbosity.hh"

//---------------------------------------------------------------------------
GmGamosVerboseByEventUA::GmGamosVerboseByEventUA()
{

}

//----------------------------------------------------------------
GmGamosVerboseByEventUA::~GmGamosVerboseByEventUA() 
{
  if (verbCmd) delete verbCmd;
}

//----------------------------------------------------------------
void GmGamosVerboseByEventUA::BeginOfEventAction( const G4Event* anEvent )
{
  //----------- Set /tracking/verbose for this event 
  mmis::const_iterator ite;
  for( ite = theVerbs.begin(); ite != theVerbs.end(); ite++ ){
    if( (*ite).first == anEvent->GetEventID() ) {
      G4cout << " GmGamosVerboseByEventUA::BeginOfEventAction " << (*ite).second.first << " = " << (*ite).second.second << G4endl;
      GmVVerbosity::SetVerbosityLevel( (*ite).second.first, (*ite).second.second );
    }
    
  }
}

//----------------------------------------------------------------
void GmGamosVerboseByEventUA::SetNewValue(G4UIcommand * command,G4String newValues)
{
  if (command == verbCmd) {
    GmGenUtils::CheckNWords(newValues,4,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 4 arguments: verbosity_class verbosity_value event_min event_max"); 
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );

    G4String verbName = wl[0];
    G4String verbValue = wl[1];
    G4int verbVal;
    if( GmGenUtils::IsNumber( verbValue ) ) {
      verbVal = atoi( verbValue );
    } else {
      if( verbValue == "silent" ) {
	verbVal = silentVerb;
      } else if( verbValue == "error" ) {
	verbVal = errorVerb;
      } else if( verbValue == "warning" ) {
	verbVal = warningVerb;
      } else if( verbValue == "info" ) {
	verbVal = infoVerb;
      } else if( verbValue == "debug" ) {
	verbVal = debugVerb;
      } else if( verbValue == "test" ) {
	verbVal = testVerb;
      } else {
	G4Exception("GmGamosVerboseByEventUA::SetNewValue",
		    "Error",
		    FatalException,
		    G4String("Invalid value = " + verbValue).c_str());
      }
    }

    G4int eventMin = G4int(GmGenUtils::GetValue(wl[2]));
    if( eventMin < 0 ) {
      G4Exception("GmGamosVerboseByEventUA::SetNewValue",
		  "Minimum event number must be >= 0",
		  FatalErrorInArgument,
		  (G4String("Value is = ") + wl[2]).c_str());
    }
    G4int eventMax = G4int(GmGenUtils::GetValue(wl[3]));
    if( eventMax <= eventMin ) {
      G4Exception("GmGamosVerboseByEventUA::SetNewValue",
		  "Maximum event number must be > minimum event number",
		  FatalErrorInArgument,
		  (G4String("Value is = ") + wl[3]).c_str());
    }

    theVerbs.insert( mmis::value_type( eventMin, std::pair<G4String,G4int>(verbName,verbVal) ));
    theVerbs.insert( mmis::value_type( eventMax , std::pair<G4String,G4int>(verbName,silentVerb)));

  }
}

