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
///////////////////////////////////////////////////////////////////////////////
// File: GmAnalysisMessenger.h
// Date: 27/03/2002 P.Arce
// Description: Adds commands for listing, enabling and disabling user actions
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmAnalysisMessenger_h
#define GmAnalysisMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "GmUIcmdWithAString.hh"

class GmAnalysisMessenger: public G4UImessenger {
public:
  GmAnalysisMessenger();
  ~GmAnalysisMessenger();
  
  virtual void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:
  GmUIcmdWithAString*  setFormatCmd;
  GmUIcmdWithAString*  addFormatCmd;
  GmUIcmdWithAString*  histo1NBinsCmd;
  GmUIcmdWithAString*  histo1MinCmd;
  GmUIcmdWithAString*  histo1MaxCmd;
  GmUIcmdWithAString*  histo2NBinsXCmd;
  GmUIcmdWithAString*  histo2MinXCmd;
  GmUIcmdWithAString*  histo2MaxXCmd;
  GmUIcmdWithAString*  histo2NBinsYCmd;
  GmUIcmdWithAString*  histo2MinYCmd;
  GmUIcmdWithAString*  histo2MaxYCmd;
  GmUIcmdWithAString*  histoPrintCmd;

};

#endif
