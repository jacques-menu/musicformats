/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___LilyPondIssue34Oah___
#define ___LilyPondIssue34Oah___

#include "mfPreprocessorSettings.h"

#include "mfMultiGenerationOah.h"

#include "msrGenerationBasicTypes.h"

#include "brailleGenerationOah.h"
#include "brailleGeneration.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP LilyPondIssue34OahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<LilyPondIssue34OahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLilyPondIssue34OahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          LilyPondIssue34OahGroup ();

    virtual               ~LilyPondIssue34OahGroup ();

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

    void                  setGenerationAPIKind (
                            msrGenerationAPIKind value)
                              { fGenerationAPIKind = value; }

    msrGenerationAPIKind  getGenerationAPIKind () const
                              { return fGenerationAPIKind;  }

    // multi-generation output kind

    void                  setMultiGenerationOutputKind (
                            mfMultiGenerationOutputKind value)
                              { fMultiGenerationOutputKind = value; }

    mfMultiGenerationOutputKind
                          getMultiGenerationOutputKind () const
                              { return fMultiGenerationOutputKind;  }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
// JMI    void                  initializeLilyPondIssue34TraceOah ();
#endif // MF_TRACE_IS_ENABLED

    void                  initializeGenerationAPIOptions ();

    void                  initializeMultiGenerationOutputOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printLilyPondIssue34OahHelp ();

    void                  printLilyPondIssue34OahValues (
                            int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // generation API kind

    msrGenerationAPIKind  fGenerationAPIKind;
    S_msrGenerationAPIKindAtom
                          fGenerationAPIKindAtom;

    // multi-generation output kind

    mfMultiGenerationOutputKind
                          fMultiGenerationOutputKind;
    S_mfMultiGenerationOutputKindAtom
                          fMultiGenerationOutputKindAtom;
};
typedef SMARTP<LilyPondIssue34OahGroup> S_LilyPondIssue34OahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_LilyPondIssue34OahGroup& elt);

EXP extern S_LilyPondIssue34OahGroup gGlobalLilyPondIssue34OahGroup;

//______________________________________________________________________________
EXP S_LilyPondIssue34OahGroup createGlobalLilyPondIssue34OahGroup ();


}


#endif // ___LilyPondIssue34Oah___
