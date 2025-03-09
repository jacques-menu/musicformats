/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrGenerationOah___
#define ___msrGenerationOah___

#include "mfPreprocessorSettings.h"

#include "msrGenerationBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrGeneratorsOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrGeneratorsOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsrGeneratorsOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrGeneratorsOahGroup ();

    virtual               ~msrGeneratorsOahGroup ();

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

    // fields
    // ------------------------------------------------------

    // generaton API kind

    void                  setGenerationAPIKind (msrGenerationAPIKind value)
                              { fGenerationAPIKind = value; }

    msrGenerationAPIKind  getGenerationAPIKind () const
                              { return fGenerationAPIKind;  }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
// JMI    void                  initializeMsrGeneratorsTraceOah ();
#endif // MF_TRACE_IS_ENABLED

    void                  initializeGenerationAPIOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsrGeneratorsOahHelp ();

    void                  displayMsrGeneratorsOahValues (
                            int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // generation API kind

    msrGenerationAPIKind  fGenerationAPIKind;
    S_msrGenerationAPIKindAtom
                          fGenerationAPIKindAtom;
};
typedef SMARTP<msrGeneratorsOahGroup> S_msrGeneratorsOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msrGeneratorsOahGroup& elt);

EXP extern S_msrGeneratorsOahGroup gGlobalMsrGeneratorsOahGroup;

//______________________________________________________________________________
EXP S_msrGeneratorsOahGroup createGlobalMsrGeneratorsOahGroup ();


}


#endif // ___msrGenerationOah___
