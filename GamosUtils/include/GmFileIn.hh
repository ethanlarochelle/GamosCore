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
#ifndef GmFileIn_HH
#define GmFileIn_HH

#include "globals.hh"
#include <vector>


class GmFileIn 
{
 public:
  GmFileIn();
  ~GmFileIn(){}
 private:
   GmFileIn( const G4String& name ): theName(name){}

 public:
  // Get the only instance opening the file
  static GmFileIn& GetInstance( const G4String& name ); 

  // Get the only instance when file should be already opened
  static GmFileIn& GetInstanceOpened( const G4String& name ); 

  // Read a line and transform it to a vector of words 
  // return 0 if EOF found, else 1
  G4int GetWordsInLine( std::vector<G4String>& wl );

  // Read a line and transform it to a vector of doubles
  // return 0 if EOF found, else 1
  G4int GetWordsInLineDouble( std::vector<G4double>& wl );

  // Print out an error message indicating the line being read
  void ErrorInLine();

 // Access data members
  G4int Nline() 
  { 
    return theLineNo[theCurrentFile]; 
  }

  const G4String& GetName() 
  {
    return theName; 
  }

  G4bool eof();
  void Close();
  void Rewind();
  void DumpException( const G4String& sent );

  void SetSeparator( char sep ) 
  {
    theSeparator = sep;
  }
  void SetSuppressQuotes( G4bool sq )
  {
    bSuppressQuotes = sq;
  }
  void SetCheckQuotes( G4bool sq )
  {
    bCheckQuotes = sq;
  }
  std::ifstream* GetIfstream() const 
  {
    return theifstream;
  }

 private:
  void OpenNewFile( const char* filename );

 private:
  std::vector< std::ifstream* > theFiles;
  std::ifstream* theifstream;
  // Number of line being read
  std::vector<G4int> theLineNo;
  std::vector<G4String> theNames;
  int theCurrentFile;  // index of file being read in theFiles

 // private DATA MEMEBERS
  // Vector of class instances (each one identified by its name)
  static std::vector<GmFileIn*> theInstances;
 
  /// Name of file
  G4String theName;

  char theSeparator;
  G4bool bSuppressQuotes;
  G4bool bCheckQuotes;
};

#endif 
