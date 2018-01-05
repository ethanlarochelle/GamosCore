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
#ifndef Gm3ddose__HH
#define Gm3ddose__HH

#include <vector>
#include "Gm3ddoseHeader.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
class GmSqdose;

class Gm3ddose
{
public:
  Gm3ddose();
  Gm3ddose(const GmSqdose& sqd);
  ~Gm3ddose(){};

  Gm3ddose operator+=( const Gm3ddose& dose );

  void Read( G4String& fileName );
  void Read(GmFileIn& fin);
  //  void Write3ddose(std::ofstream& fout);

public:

  Gm3ddoseHeader* GetHeader() const {
    return theHeader; }
  std::vector<float> GetDoses() const {
    return theDoses; }
  std::vector<float> GetDoseErrors() const {
    return theDoseErrors; }

private:
  Gm3ddoseHeader* theHeader;
  std::vector<float> theDoses;
  std::vector<float> theDoseErrors;

};

std::ostream& operator<<(std::ostream &out, 
                           const Gm3ddose &dose);


#endif
