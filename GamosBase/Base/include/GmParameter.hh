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
#ifndef GmParameter_hh
#define GmParameter_hh
//
//
//
#include<string>
#include<sstream>

class SimpleConfiguration;

/**
 */
template<class TYP>
class GmParameter 
{
private:
  typedef GmParameter<TYP> par;  
public:
  GmParameter() {} //?? NEEDED??
  GmParameter(const TYP& par) : theValue(par) {
    //  G4cout << " new GmParameter " << theValue << G4endl; 
  }

  ///
  GmParameter & operator=(const par& rh) {
    theValue=rh.theValue;
    return *this;
  }
  
  virtual ~GmParameter(){}

  operator const TYP&() const { return theValue;}
  TYP& operator()() { return theValue;}
  const TYP& operator()() const { return theValue;}

  TYP& value() { return theValue;}
  const TYP& value() const { return theValue;}

  /*
  std::string Type() const {
    return ClassName<T>::name();
  }
  */

  void Set(const TYP& val){ theValue = val; }
  void Set(const std::string & input);
  
  //  virtual std::string Get() const;
  
  
protected:
  
  TYP theValue;
  
  
};

#include<iomanip>
#include<algorithm>

/*
template<class T>
inline std::ostream & operator<<(std::ostream & out, const GmParameter<TYP>& c) {
  return out <<  c.value();
}
*/

template<class TYP>
inline void GmParameter<TYP>::Set(const std::string & input) {
  std::istringstream in(input.c_str());
  in >> theValue;
}  // for vectors???

/*
template<class TYP>
inline std::string GmParameter<TYP>::Get() const{
  std::ostringstream out;
  out << theValue;
#ifdef CMS_CHAR_STREAM
  out << ends;
#endif
  return out.str();
}
*/

#endif // GmParameter_hh
