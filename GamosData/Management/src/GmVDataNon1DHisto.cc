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
#include "GmVDataNon1DHisto.hh"
#include "GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmVDataNon1DHisto::GmVDataNon1DHisto() : GmVData()
{
}

//----------------------------------------------------------------
GmVDataNon1DHisto::~GmVDataNon1DHisto()
{
}


//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteText( const G4Step* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());

}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteText( const G4Track* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteText( const G4Track* , const G4Track* , G4bool  )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteText( const G4Event* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());

}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteText( const G4Run* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());

}


//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteBin( const G4Step* )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteBin( const G4Track* )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteBin( const G4Track* , const G4Track* )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteBin( const G4Event* )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteBin( const G4Run* )
{
  G4Exception("GmVDataNon1DHisto::WriteText",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}


//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteCout( const G4Step* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteCout",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());

}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteCout( const G4Track* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteCout",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteCout( const G4Track* , const G4Track* , G4bool  )
{
  G4Exception("GmVDataNon1DHisto::WriteCout",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());
}

//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteCout( const G4Event* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteCout",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());

}
//----------------------------------------------------------------
void GmVDataNon1DHisto::WriteCout( const G4Run* , G4bool )
{
  G4Exception("GmVDataNon1DHisto::WriteCout",
	      "A DataVsData cannot be written into a file, only serves to make 2D histograms",
	      FatalErrorInArgument,
	      G4String(" DATA= " + theName).c_str());

}

