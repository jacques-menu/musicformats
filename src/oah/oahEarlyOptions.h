/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___oahEarlyOptions___
#define ___oahEarlyOptions___

#include "exports.h"

#include "oahBasicTypes.h"

#include "mfMultiGenerationOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
// insider
EXP extern const string K_INSIDER_OPTION_LONG_NAME;
EXP extern const string K_INSIDER_OPTION_SHORT_NAME;
// EXP extern const string K_REGULAR_OPTION_LONG_NAME;
// EXP extern const string K_REGULAR_OPTION_SHORT_NAME;

// quiet mode
EXP extern const string K_QUIET_OPTION_LONG_NAME;
EXP extern const string K_QUIET_OPTION_SHORT_NAME;

// trace early options
EXP extern const string K_TRACE_EARLY_OPTIONS_LONG_OPTION_NAME;
EXP extern const string K_TRACE_EARLY_OPTIONS_SHORT_OPTION_NAME;

// verbose mode
EXP extern const string K_OAH_VERBOSE_MODE_LONG_OPTION_NAME;
EXP extern const string K_OAH_VERBOSE_MODE_SHORT_OPTION_NAME;

// include
EXP extern const string K_INCLUDE_OPTION_LONG_NAME;
EXP extern const string K_INCLUDE_OPTION_SHORT_NAME;

#ifdef TRACING_IS_ENABLED

// trace OAH
EXP extern const string K_TRACE_OAH_LONG_OPTION_NAME;
EXP extern const string K_TRACE_OAH_SHORT_OPTION_NAME;

EXP extern const string K_TRACE_OAH_DETAILS_LONG_OPTION_NAME;
EXP extern const string K_TRACE_OAH_DETAILS_SHORT_OPTION_NAME;

// trace components
EXP extern const string K_TRACE_COMPONENTS_LONG_OPTION_NAME;
EXP extern const string K_TRACE_COMPONENTS_SHORT_OPTION_NAME;

// trace passes
EXP extern const string K_TRACE_PASSES_LONG_OPTION_NAME;
EXP extern const string K_TRACE_PASSES_SHORT_OPTION_NAME;

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

    const list<string>&   getEarlyIncludeFileNamesList () const
                              { return fEarlyIncludeFileNamesList; }

#ifdef TRACING_IS_ENABLED
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

    void                  print (ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

    void                  appendEarlyIncludeFileName (string includeFileName);

    Bool                  isEarlyOptionRecognized (
                            const string& theString,
                            const string& optionName);

    void                  applyEarlyOptionIfRelevant (
                            const string& argumentWithoutDashToBeUsed,
                            const string& optionValue);

  private:

    // fields
    // ------------------------------------------------------

    Bool                  fEarlyInsiderOption;
//     Bool                  fEarlyRegularOption;

    Bool                  fEarlyQuietOption;

    mfMultiGenerationOutputKind
                          fEarlyMultiGenerationOutputKind;

    list<string>          fEarlyIncludeFileNamesList;

#ifdef TRACING_IS_ENABLED
    Bool                  fTraceEarlyOptions;

    Bool                  fEarlyOahVerboseMode;

    Bool                  fEarlyTracingOah;
    Bool                  fEarlyTracingOahDetails;

    Bool                  fEarlyTraceComponents;

    Bool                  fEarlyTracePasses;
#endif
};
typedef SMARTP<oahEarlyOptions> S_oahEarlyOptions;
EXP ostream& operator<< (ostream& os, const oahEarlyOptions& elt);

EXP extern oahEarlyOptions gGlobalOahEarlyOptions;


}


#endif
