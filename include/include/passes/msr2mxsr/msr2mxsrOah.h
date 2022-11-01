/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2mxsrOah___
#define ___msr2mxsrOah___


#include "oahBasicTypes.h"

#include "lpsrBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msr2mxsrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2mxsrOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2mxsrOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2mxsrOahGroup ();

    virtual               ~msr2mxsrOahGroup ();

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

    // code generation
    Bool                  getXml2xmlInfos () const
                              { return fXml2xmlInfos; }

    Bool                  getMusicXMLComments () const
                              { return fMusicXMLComments; }

    // work and movement
    string                getWorkTitle () const
                              { return fWorkTitle; }
    string                getWorkNumber () const
                              { return fWorkNumber; }

    string                getMovementTitle () const
                              { return fMovementTitle; }
    string                getMovementNumber () const
                              { return fMovementNumber; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializMsr2mxsrTraceOptions ();
#endif

    void                  initializeCodeGenerationOptions ();
    void                  initializeMxsrGenerationOahGroup ();

    void                  initializeWorkAndMovementOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsr2mxsrOahHelp ();

    void                  printMsr2mxsrOahValues (int valueFieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // code generation
    Bool                  fXml2xmlInfos;

    Bool                  fMusicXMLComments;

    // work and movement
    string                fWorkTitle;
    string                fWorkNumber;

    string                fMovementTitle;
    string                fMovementNumber;
};
typedef SMARTP<msr2mxsrOahGroup> S_msr2mxsrOahGroup;
EXP ostream& operator << (ostream& os, const S_msr2mxsrOahGroup& elt);

EXP extern S_msr2mxsrOahGroup gGlobalMsr2mxsrOahGroup;

//______________________________________________________________________________
EXP S_msr2mxsrOahGroup createGlobalMsr2mxsrOahGroup ();


}


#endif
