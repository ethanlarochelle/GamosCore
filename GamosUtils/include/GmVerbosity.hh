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
#ifndef VERBOSE_H
#define VERBOSE_H

#include <iosfwd>

#include "globals.hh"

enum FilterLevel {silentVerb = -1, errorVerb = 0, warningVerb = 1, infoVerb=2, debugVerb=3, testVerb=4}; 

class GmVerbosity {
  
public:
  
  enum { nFilterLevel=6};  

  static const FilterLevel * levels();
  
  static const G4String * levelNames();
  
  static const FilterLevel & level(const G4String & name);
  
  static const G4String & levelName(int il);
  
public:
	
#ifdef WIN32
  #if defined GmUtils_ALLOC_EXPORT 
    G4DLLEXPORT static std::ostream & out;
  #else 
    G4DLLIMPORT static std::ostream & out;
  #endif
#else
  static std::ostream & out;
#endif

  ///
  explicit GmVerbosity(int il=warningVerb);

  ///
  GmVerbosity & operator()(int il=warningVerb) { 
	  level_=il;
	  return *this;}

  GmVerbosity & operator()(const G4String& il) { level_=level(il); return *this;}
  
  ///
  operator bool() const { return verbose_ && (level_ <= filter_);}

  ///
  void Verbose();

  ///
  void Silent();

  ///
  void SetFilterLevel(int il);
  ///
  int GetFilterLevel() const;

  // int level() const { return level_;}
  

private:

  bool verbose_;
  int filter_;
  int level_;
};

#include <ostream>

inline std::ostream & operator<<(std::ostream& o, const GmVerbosity& v) {
  
  return v ? o : GmVerbosity::out;
  
}


#endif // VERBOSE_H
