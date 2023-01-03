/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___oahEarlyOptions___
#define ___oahEarlyOptions___

#include "exports.h"

#include "oahBasicTypes.h"

#include "mfLanguages.h"
#include "mfMultiGenerationOah.h"
#include "mfWaeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
// language
EXP extern const std::string K_LANGUAGE_OPTION_LONG_NAME;
EXP extern const std::string K_LANGUAGE_OPTION_SHORT_NAME;

// insider
EXP extern const std::string K_INSIDER_OPTION_LONG_NAME;
EXP extern const std::string K_INSIDER_OPTION_SHORT_NAME;
// EXP extern const std::string K_REGULAR_OPTION_LONG_NAME;
// EXP extern const std::string K_REGULAR_OPTION_SHORT_NAME;

// quiet mode
EXP extern const std::string K_QUIET_OPTION_LONG_NAME;
EXP extern const std::string K_QUIET_OPTION_SHORT_NAME;

// include
EXP extern const std::string K_INCLUDE_OPTION_LONG_NAME;
EXP extern const std::string K_INCLUDE_OPTION_SHORT_NAME;

#ifdef OAH_TRACING_IS_ENABLED

// trace early options
EXP extern const std::string K_TRACE_EARLY_OPTIONS_LONG_OPTION_NAME;
EXP extern const std::string K_TRACE_EARLY_OPTIONS_SHORT_OPTION_NAME;

// OAH verbose mode
EXP extern const std::string K_OAH_VERBOSE_MODE_LONG_OPTION_NAME;
EXP extern const std::string K_OAH_VERBOSE_MODE_SHORT_OPTION_NAME;

// trace OAH
EXP extern const std::string K_TRACE_OAH_LONG_OPTION_NAME;
EXP extern const std::string K_TRACE_OAH_SHORT_OPTION_NAME;

EXP extern const std::string K_TRACE_OAH_DETAILS_LONG_OPTION_NAME;
EXP extern const std::string K_TRACE_OAH_DETAILS_SHORT_OPTION_NAME;

// trace components
EXP extern const std::string K_TRACE_COMPONENTS_LONG_OPTION_NAME;
EXP extern const std::string K_TRACE_COMPONENTS_SHORT_OPTION_NAME;

// trace passes
EXP extern const std::string K_TRACE_PASSES_LONG_OPTION_NAME;
EXP extern const std::string K_TRACE_PASSES_SHORT_OPTION_NAME;

#endif

//______________________________________________________________________________
class EXP oahEarlyOptions
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          oahEarlyOptions ();

    virtual               ~oahEarlyOptions ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setEarlyLanguageKind (
                            mfLanguageKind languageKind);
    mfLanguageKind        getEarlyLanguageKind () const
                              { return fEarlyLanguageKind; }

    S_mfWaeHandler        getMfWaeHandler () const
                              { return fMfWaeHandler; }

    void                  setEarlyInsiderOption ();
    Bool                  getEarlyInsiderOption () const
                              { return fEarlyInsiderOption; }
//     void                  setEarlyRegularOption ();

    void                  setEarlyQuietOption ();
    Bool                  getEarlyQuietOption () const
                              { return fEarlyQuietOption; }

    void                  setEarlyMultiGenerationOutputKind (
                            mfMultiGenerationOutputKind value);
    mfMultiGenerationOutputKind
                          getEarlyMultiGenerationOutputKind () const
                              { return fEarlyMultiGenerationOutputKind; }

    const std::list<std::string>&
                          getEarlyIncludeFileNamesList () const
                              { return fEarlyIncludeFileNamesList; }

#ifdef OAH_TRACING_IS_ENABLED

    void                  setTraceEarlyOptions ();
    Bool                  getTraceEarlyOptions () const
                              { return fTraceEarlyOptions; }

    void                  setEarlyOahVerboseMode ();
    Bool                  getEarlyOahVerboseMode () const
                              { return fEarlyOahVerboseMode; }

    void                  setEarlyTracingOah ();
    Bool                  getEarlyTracingOah () const
                              { return fEarlyTracingOah; }

    void                  setEarlyTracingOahDetails ();
    Bool                  getEarlyTracingOahDetails () const
                              { return fEarlyTracingOahDetails; }

    void                  setEarlyTraceComponents ();
    Bool                  getEarlyTraceComponents () const
                              { return fEarlyTraceComponents; }


    void                  setEarlyTracePasses ();
    Bool                  getEarlyTracePasses () const
                              { return fEarlyTracePasses; }
#endif

  public:

    // public services
    // ------------------------------------------------------

    void                  applyEarlyOptionsIfPresentInArgcArgv (
                            int   argc,
                            char* argv[]);

    void                  applyEarlyOptionsIfPresentInOptionsAndArguments (
                            oahOptionsAndArguments& optionsAndArguments);

  public:

    // print
    // ------------------------------------------------------

    void                  print (std::ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

    void                  appendEarlyIncludeFileName (std::string includeFileName);

    Bool                  isEarlyOptionRecognized (
                            const std::string& theString,
                            const std::string& optionName);

    void                  applyEarlyOptionIfRelevant (
                            const std::string& argumentWithoutDashToBeUsed,
                            const std::string& optionValue);

  private:

    // fields
    // ------------------------------------------------------

    mfLanguageKind        fEarlyLanguageKind;
    S_mfWaeHandler        fMfWaeHandler;

    Bool                  fEarlyInsiderOption;
//     Bool                  fEarlyRegularOption;

    Bool                  fEarlyQuietOption;

    mfMultiGenerationOutputKind
                          fEarlyMultiGenerationOutputKind;

    std::list<std::string>
                          fEarlyIncludeFileNamesList;

#ifdef OAH_TRACING_IS_ENABLED

    Bool                  fTraceEarlyOptions;

    Bool                  fEarlyOahVerboseMode;

    Bool                  fEarlyTracingOah;
    Bool                  fEarlyTracingOahDetails;

    Bool                  fEarlyTraceComponents;

    Bool                  fEarlyTracePasses;

#endif
};
typedef SMARTP<oahEarlyOptions> S_oahEarlyOptions;
EXP std::ostream& operator << (std::ostream& os, const oahEarlyOptions& elt);

EXP extern oahEarlyOptions gGlobalOahEarlyOptions;


}


#endif
