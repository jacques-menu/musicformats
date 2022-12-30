/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdl2msrOah___
#define ___msdl2msrOah___


#include "msrPitchesNames.h"

#include "oahBasicTypes.h"

#include "msdlKeywords.h"

#include "msdlInputOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msdl2msrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2msrOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2msrGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2msrOahGroup ();

    virtual               ~msdl2msrOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

#ifdef OAH_TRACING_IS_ENABLED
    Bool                  getTraceTokens () const
                              { return fTraceTokens; }
    Bool                  getTraceTokensDetails () const
                              { return fTraceTokensDetails; }

    Bool                  getTraceSyntax () const
                              { return fTraceSyntax; }
    Bool                  getTraceSyntaxDetails () const
                              { return fTraceSyntaxDetails; }

    Bool                  getTraceSyntaxErrorRecovery () const
                              { return fTraceSyntaxErrorRecovery; }
    Bool                  getTraceSyntaxErrorRecoveryDetails () const
                              { return fTraceSyntaxErrorRecoveryDetails; }
#endif

    msdlUserLanguageKind  getMsdlUserLanguageKind () const
                              { return fMsdlUserLanguageKind; }

    msdlKeywordsLanguageKind
                          getMsdlKeywordsInputLanguageKind () const
                              { return fMsdlKeywordsInputLanguageKind; }

    msrQuarterTonesPitchesLanguageKind
                          getMsdlQuarterTonesPitchesLanguageKind () const
                              { return fMsdlQuarterTonesPitchesLanguageKind; }

    Bool                  getLexicalAnalysisOnly () const
                              { return fLexicalAnalysisOnly; }

    void                  setIgnoreSeparatorTokens (Bool value)
                              { fIgnoreSeparatorTokens = value; }

    Bool                  getIgnoreSeparatorTokens () const
                              { return fIgnoreSeparatorTokens; }

    msdlKeywordsLanguageKind
                          getMsdlKeywordsConversionLanguageKind () const
                              { return fMsdlKeywordsConversionLanguageKind; }

    msdlCommentsTypeKind  getMsdlCommentsTypeKind () const
                              { return fMsdlCommentsTypeKind; }

    Bool                  getQuitOnLexicalErrors () const
                              { return fQuitOnLexicalErrors; }

  public:

    // public services
    // ------------------------------------------------------

    // languages
    Bool                  setMsdlQuarterTonesPitchesLanguage (std::string language);

    Bool                  setMsdlKeywordsLanguage (std::string language);

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  private:

    // private services
    // ------------------------------------------------------

#ifdef OAH_TRACING_IS_ENABLED
    void                  initializeMsdl2msrtracingOah ();
#endif

    void                  initializeMsdlLanguagesOptions ();

    void                  initializeMsdlWholeInputScansOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsdl2msrHelp ();

    void                  printMsdl2msrValues (int fieldWidth);

  private:

    // fields
    // ------------------------------------------------------

    // trace
#ifdef OAH_TRACING_IS_ENABLED
    // MSDL tokens and syntax
    Bool                  fTraceTokens;
    Bool                  fTraceTokensDetails;

    Bool                  fTraceSyntax;
    Bool                  fTraceSyntaxDetails;

    Bool                  fTraceSyntaxErrorRecovery;
    Bool                  fTraceSyntaxErrorRecoveryDetails;
#endif

    // languages
    // --------------------------------------

    msdlUserLanguageKind  fMsdlUserLanguageKind;

    msdlKeywordsLanguageKind
                          fMsdlKeywordsInputLanguageKind;

    msrQuarterTonesPitchesLanguageKind
                          fMsdlQuarterTonesPitchesLanguageKind;

    // whole input scan
    // --------------------------------------

    Bool                  fLexicalAnalysisOnly;

    Bool                  fIgnoreSeparatorTokens;

    msdlKeywordsLanguageKind
                          fMsdlKeywordsConversionLanguageKind;

    msdlCommentsTypeKind  fMsdlCommentsTypeKind;

    // quit
    // --------------------------------------

    Bool                  fQuitOnLexicalErrors;
};
typedef SMARTP<msdl2msrOahGroup> S_msdl2msrOahGroup;
EXP std::ostream& operator << (std::ostream& os, const S_msdl2msrOahGroup& elt);

EXP extern S_msdl2msrOahGroup gGlobalMsdl2msrOahGroup;

//______________________________________________________________________________
EXP S_msdl2msrOahGroup createGlobalMsdl2msrOahGroup ();


}


#endif
