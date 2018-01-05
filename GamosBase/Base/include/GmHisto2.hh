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
#ifndef GmHisto2_hh
#define GmHisto2_hh
#ifdef GAMOS_NO_ROOT

#include <string>
#include <map>
#include <iostream>
#include <math.h>

class GmHisto2
{
public:
  GmHisto2( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY );
  GmHisto2( const std::string& name, const std::string& title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY );
  ~GmHisto2(){};

  void Fill( double valueX, double valueY, double weight = 1. );

  std::string GetName() const { return theName; }
  int GetNbinsX() const { return theNBinsX; }
  int GetNbinsY() const { return theNBinsY; }
  double GetLowerEdgeX() const { return theLowerEdgeX; }
  double GetLowerEdgeY() const { return theLowerEdgeY; }
  double GetUpperEdgeX() const { return theUpperEdgeX; }
  double GetUpperEdgeY() const { return theUpperEdgeY; }
  double GetBinWidthX() const { return theBinWidthX; }
  double GetBinWidthY() const { return theBinWidthY; }
  double GetBinWidthX(int ) const { return theBinWidthX; }
  double GetBinWidthY(int ) const { return theBinWidthY; }
  double GetBinLowEdgeX( int ibin ) const;
  double GetBinUpEdgeX( int ibin ) const;
  double GetBinLowEdgeY( int ibin ) const;
  double GetBinUpEdgeY( int ibin ) const;

  double GetBinContent(int ibinX, int ibinY) const;
  double GetBinError(int ibinX, int ibinY) const;

  double GetSumW() const { return theSumW;}
  double GetSumW2() const { return theSumW2;}
  double GetSumWX() const { return theSumWX;}
  double GetSumWX2() const { return theSumWX2;}
  double GetSumWY() const { return theSumWY;}
  double GetSumWY2() const { return theSumWY2;}

  int GetEntries() const;
  void SetEntries( int ent ) { theEntries = ent;}

  double GmGenUtils::GetIntegral() const;
  int GetBinX(float binx ) const;
  int GetBinY(float biny ) const;

  double GetBinSumW(int ibin) const { 
    std::map<int,double>::const_iterator ite = theBinSumW.find(ibin);
    return (*ite).first;}
  double GetBinSumW2(int ibin) const {
    std::map<int,double>::const_iterator ite = theBinSumW2.find(ibin);
    return (*ite).first;}
  
  //  virtual Float_t GetKurtosis(Int_t axis=1) const;
  //    virtual Float_t GetSkewness(Int_t axis=1) const;
  //  virtual Float_t GetMaximum(Float_t maxval=FLT_MAX) const;
  //   virtual Int_t    GetMaximumBin() const;
  //   virtual Float_t GetMinimum(Float_t minval=-FLT_MAX) const;
  //   virtual Int_t    GetMinimumBin() const;
  double GetEffectiveEntries() const;

  void SetBinContent(int ibinX, int ibinY, double val);
  void SetBinError(int ibinX, int ibinY, double val);

  double GetMean( int axis ) const;
  double GetRMS( int axis ) const;
  double GetMeanError( int axis ) const;
  double GetRMSError( int axis ) const;

protected:
  std::string theName;
  int theNBinsX;
  double theLowerEdgeX;
  double theUpperEdgeX;
  int theNBinsY;
  double theLowerEdgeY;
  double theUpperEdgeY;
  double theBinWidthX;
  double theBinWidthY;

  int theEntries;
  std::map<int,int> theBinN;
  std::map<int,double> theBinSumW;
  std::map<int,double> theBinSumW2;

  double theSumW;
  double theSumW2;
  double theSumWX;
  double theSumWX2;
  double theSumWY;
  double theSumWY2;

  friend GmHisto2 operator+(const GmHisto2 &h1, const GmHisto2 &h2);
  
};

#endif

#endif
