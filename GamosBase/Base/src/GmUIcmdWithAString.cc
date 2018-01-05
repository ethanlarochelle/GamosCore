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
#include "GmUIcmdWithAString.hh"
#include "G4Tokenizer.hh"
#include "G4UIcommandStatus.hh"
#include "G4UImessenger.hh"

GmUIcmdWithAString::GmUIcmdWithAString
(const char * theCommandPath,G4UImessenger * theMessenger)
  :G4UIcmdWithAString(theCommandPath,theMessenger)
{
}


G4int GmUIcmdWithAString::DoIt(G4String parameterList)
{

  G4String correctParameters;
  G4int n_parameterEntry = parameter.size();
  if( n_parameterEntry != 0 )
    {
      G4String aToken;
      G4String correctToken;
      G4Tokenizer parameterToken( parameterList );
      for( G4int i_thParameter=0; i_thParameter<n_parameterEntry; i_thParameter++ )
	{
	  if(i_thParameter > 0)
	    {
	      correctParameters.append(" ");
	    }
	  aToken = parameterToken();
	  if( aToken.length()>0 && aToken(0)=='"' )
	    {
	      while( aToken(aToken.length()-1) != '"'
		     || ( aToken.length()==1 && aToken(0)=='"' ))
		{
		  G4String additionalToken = parameterToken();
		  if( additionalToken.isNull() )
		    { return fParameterUnreadable+i_thParameter; }
		  aToken += " ";
		  aToken += additionalToken;
		}
	    }
      
	  if(i_thParameter==n_parameterEntry-1 && parameter[i_thParameter]->GetParameterType()=='s')
	    {
	      G4String anotherToken;
	      while(!((anotherToken=parameterToken()).isNull()))
		{
		  G4int idxs = anotherToken.index("#");
		  if(idxs==G4int(std::string::npos))
		    {
		      aToken += " ";
		      aToken += anotherToken;
		    }
		  else if(idxs>0)
		    {
		      aToken += " ";
		      aToken += anotherToken(0,idxs);
		      break;
		    }
		  else
		    { break; }
		}
	    }

	  if( aToken.isNull() || aToken == "!" )
	    {

	      if(parameter[i_thParameter]->IsOmittable())
		{ 
		  if(parameter[i_thParameter]->GetCurrentAsDefault())
		    {
		      G4Tokenizer cvSt(messenger->GetCurrentValue(this));
		      G4String parVal;
		      for(G4int ii=0;ii<i_thParameter;ii++)
			{
			  parVal = cvSt();
			  if (parVal(0)=='"')
			    {
			      while( parVal(parVal.length()-1) != '"' )
				{
				  G4String additionalToken = cvSt();
				  if( additionalToken.isNull() )
				    { return fParameterUnreadable+i_thParameter; }
				  parVal += " ";
				  parVal += additionalToken;
				}
			    }
			}
		      G4String aCVToken = cvSt();
		      if (aCVToken(0)=='"')
			{
			  while( aCVToken(aCVToken.length()-1) != '"' )
			    {
			      G4String additionalToken = cvSt();
			      if( additionalToken.isNull() )
				{ return fParameterUnreadable+i_thParameter; }
			      aCVToken += " ";
			      aCVToken += additionalToken;
			    }
			  // aCVToken.strip(G4String::both,'"');
			}
		      correctParameters.append(aCVToken);
		    }
		  else
		    {
		      correctParameters.append(parameter[i_thParameter]->GetDefaultValue()); }
		}
	      else
		{ return fParameterUnreadable+i_thParameter; }
	    }
	  else
	    {
	      G4int stat = parameter[i_thParameter]->CheckNewValue( aToken );
	      if(stat) return stat+i_thParameter;
	      correctParameters.append(aToken);
	    }
	}
    }

  if(CheckNewValue( correctParameters ))
    { return fParameterOutOfRange+99; }


  messenger->SetNewValue( this, correctParameters );
  return 0;
}


