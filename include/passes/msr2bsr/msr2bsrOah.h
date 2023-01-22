/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2bsrOah___
#define ___msr2bsrOah___

#include "bsrOah.h"


#include "oahBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msr2bsrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2bsrOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2bsrOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msr2bsrOahGroup ();

    virtual               ~msr2bsrOahGroup ();

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

    // miscellaneous
    Bool                  getNoBrailleHeadings () const
                              { return fNoBrailleHeadings; }

    Bool                  getNoTempos () const
                              { return fNoTempos; }

    Bool                  getNoPageNumbers () const
                              { return fNoPageNumbers; }
    Bool                  getNoLineNumbers () const
                              { return fNoLineNumbers; }
    Bool                  getNoMeasureNumbers () const
                              { return fNoMeasureNumbers; }

    Bool                  getNoBrailleLyrics () const
                              { return fNoBrailleLyrics; }

    Bool                  getBrailleRunDate () const
                              { return fBrailleRunDate; }

    bsrFacSimileKind      getFacSimileKind () const
                              { return fFacSimileKind; }

    Bool                  getIncludeClefs () const
                              { return fIncludeClefs; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef MF_TRACING_IS_ENABLED
// JMI    void                  initializeMsr2bsrtracingOahGroup ();
#endif

    void                  initializeMsr2bsrMiscellaneousOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsr2bsrOahHelp ();

    void                  displayMsr2bsrOahValues (int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // miscellaneous
    Bool                  fNoBrailleHeadings;

    Bool                  fNoTempos;

    Bool                  fNoPageNumbers;
    Bool                  fNoLineNumbers;
    Bool                  fNoMeasureNumbers;

    Bool                  fNoBrailleLyrics;

    Bool                  fBrailleRunDate;

    bsrFacSimileKind      fFacSimileKind;

    Bool                  fIncludeClefs;
};
typedef SMARTP<msr2bsrOahGroup> S_msr2bsrOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msr2bsrOahGroup& elt);

EXP extern S_msr2bsrOahGroup gGlobalMsr2bsrOahGroup;

//______________________________________________________________________________
EXP S_msr2bsrOahGroup createGlobalMsr2bsrOahGroup ();


}


#endif
