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
#ifndef GmHistoProfile1_hh
#define GmHistoProfile1_hh
#ifdef GAMOS_NO_ROOT

#include "GmHisto1.hh"

class GmHistoProfile1 : public GmHisto1
{
public:
  GmHistoProfile1( const std::string& name, int nBins, double lowerEdge, double upperEdge );
  GmHistoProfile1( const std::string& name, const std::string& title, int nBins, double lowerEdge, double upperEdge );
  ~GmHistoProfile1(){};

  void Fill( double valueX, double valueY, double weight = 1. );

  double GetBinContent(int ibin) const;
  double GetBinError(int ibin) const;

private:
  friend  GmHistoProfile1 operator+(const GmHistoProfile1 &h1, const GmHistoProfile1 &h2);
  
};

#endif

#endif
