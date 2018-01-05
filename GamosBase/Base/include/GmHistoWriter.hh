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
#ifndef GmHistoWriter_h
#define GmHistoWriter_h 1

#ifndef GAMOS_NO_ROOT
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TProfile2D.h"
#else
class GmHisto1;
class GmHisto2;
class GmHistoProfile1;
class GmHistoProfile2;
#endif

#include "globals.hh"

class GmHistoWriter 
{
public:
  GmHistoWriter(){};
  virtual ~GmHistoWriter(){};
#ifndef GAMOS_NO_ROOT
  virtual void SaveHisto1( const TH1F* his ) = 0;
  virtual void SaveHisto2( const TH2F* his ) = 0;
  virtual void SaveHistoProf1( const TProfile* his ) = 0;
  virtual void SaveHistoProf2( const TProfile2D* his ) = 0;
#else
  virtual void SaveHisto1( const GmHisto1* his ) = 0;
  virtual void SaveHisto2( const GmHisto2* his ) = 0;
  virtual void SaveHistoProf1( const GmHistoProfile1* his ) = 0;
  virtual void SaveHistoProf2( const GmHistoProfile2* his ) = 0;
#endif

 private:

};


#endif
