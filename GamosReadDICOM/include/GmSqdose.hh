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
#ifndef GmSqdose__HH
#define GmSqdose__HH

#include <vector>
#include "GmSqdoseHeader.hh"
class Gm3ddose;
enum SqdoseType{ SqTALL, SqTFILLED, SqTCROSS_PHANTOM };

class GmSqdose
{
public:
  GmSqdose();
  GmSqdose(const Gm3ddose& dold);
  ~GmSqdose();

  GmSqdose operator+=( const GmSqdose& dose );
  GmSqdose operator*=( double factor );

  void Read( G4String fileName );
  void Read( FILE* fin);
  void Print( FILE* fout );
  void CalculateErrors();

public:

  GmSqdoseHeader* GetHeader() const {
    return theHeader; }
  std::vector<float> GetDoses() const {
    return theDoses; }
  std::vector<float> GetDoseSqs() const {
    return theDoseSqs; }
  float GetDose(G4int ii ) const {
    return theDoses[ii]; }
  float GetDoseSq(G4int ii ) const {
    return theDoseSqs[ii]; }
  std::vector<float> GetDoseErrors() const {
    return theDoseErrors; }
  float GetDoseError(G4int ii) const {
    return theDoseErrors[ii]; }

  void SetHeader( GmSqdoseHeader* header ){
    theHeader = header; }
  void SetDoses( std::vector<float>& doses ) {
    theDoses = doses; }
  void SetDoseSqs( std::vector<float>& dosesqs ) {
    theDoseSqs = dosesqs; }

  void SetSqdoseType( SqdoseType typ ) {
    theType = typ; }

  SqdoseType GetType() const {
    return theType; }

private:
  GmSqdoseHeader* theHeader;
  std::vector<float> theDoses;
  std::vector<float> theDoseSqs;
  std::vector<float> theDoseErrors;

  SqdoseType theType;
};


#endif
