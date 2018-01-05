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
// #define  GmVerbosity_CC
#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include <iostream>
#include <fstream>
#include <algorithm>

GmVerbosity::GmVerbosity(int il) : level_(testVerb) 
{
  filter_ = il;
  verbose_=true;
}

const FilterLevel * GmVerbosity::levels() {
  static const FilterLevel local[nFilterLevel+1] = {silentVerb,errorVerb,warningVerb,infoVerb,debugVerb,testVerb};
  return local;
}

const G4String * GmVerbosity::levelNames() {
  static const G4String local[nFilterLevel+1] = {"silent", "error", "warning", "info", "debug", "test"};
  return local;
}
const FilterLevel & GmVerbosity::level(const G4String & name) {
  const G4String * p = std::find(levelNames(),levelNames()+nFilterLevel,name);
  return levels()[p-levelNames()];
}

const G4String & GmVerbosity::levelName(int il) {
  const FilterLevel * p = std::lower_bound(levels(),levels()+nFilterLevel,il);
  return levelNames()[p-levels()];
}
 
static std::ofstream devnull("/dev/null");

std::ostream & GmVerbosity::out = devnull;

void GmVerbosity::Verbose() {verbose_=true;}

void GmVerbosity::Silent() {verbose_=false;}


void GmVerbosity::SetFilterLevel(int il) { filter_=il;}

int GmVerbosity::GetFilterLevel() const { return filter_;}


//GmVerbosity infoV, testV, debugV;

//----------------------------------------------------------------------------
