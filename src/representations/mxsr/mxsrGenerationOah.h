/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsrGenerationOah___
#define ___mxsrGenerationOah___


#include "oahBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP mxsrGenerationOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxsrGenerationOahGroup> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mxsrGenerationOahGroup ();

    virtual               ~mxsrGenerationOahGroup ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMxsrGenerationOahGroup ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

    Bool                  getXml2xmlInfos () const
                              { return fXml2xmlInfos; }

    Bool                  getMusicXMLComments () const
                              { return fMusicXMLComments; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
// JMI    void                  initializeMsr2mxsrtracingOah ();
#endif

    void                  initializeCodeGenerationOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMxsrGenerationOahHelp ();

    void                  printMxsrGenerationOahValues (int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // code generation
    // --------------------------------------

    Bool                  fXml2xmlInfos;

    Bool                  fMusicXMLComments;
};
typedef SMARTP<mxsrGenerationOahGroup> S_mxsrGenerationOahGroup;
EXP ostream& operator<< (ostream& os, const S_mxsrGenerationOahGroup& elt);

EXP extern S_mxsrGenerationOahGroup gGlobalMxsrGenerationOahGroup;

//______________________________________________________________________________
EXP S_mxsrGenerationOahGroup createGlobalMxsrGenerationOahGroup ();


}


#endif
