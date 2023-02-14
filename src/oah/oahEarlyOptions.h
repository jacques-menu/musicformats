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

#include "mfStaticSettings.h"

#include "oahBasicTypes.h"

#include "languages.h"
#include "mfMultiGenerationOah.h"
#include "languages.h"


namespace MusicFormats
{

//______________________________________________________________________________
// language
EXP extern const std::string K_LANGUAGE_OPTION_LONG_NAME;
EXP extern const std::string K_LANGUAGE_OPTION_SHORT_NAME;

// insider
EXP extern const std::string K_INSIDER_OPTION_LONG_NAME;
EXP extern const std::string K_INSIDER_OPTION_SHORT_NAME;

// quiet mode
EXP extern const std::string K_QUIET_OPTION_LONG_NAME;
EXP extern const std::string K_QUIET_OPTION_SHORT_NAME;

// include
EXP extern const std::string K_INCLUDE_OPTION_LONG_NAME;
EXP extern const std::string K_INCLUDE_OPTION_SHORT_NAME;

#ifdef MF_TRACE_IS_ENABLED

// trace early options
EXP extern const std::string K_TRACE_EARLY_OPTIONS_OPTION_LONG_NAME;
EXP extern const std::string K_TRACE_EARLY_OPTIONS_OPTION_SHORT_NAME;

// OAH verbose mode
EXP extern const std::string K_OAH_VERBOSE_MODE_OPTION_LONG_NAME;
EXP extern const std::string K_OAH_VERBOSE_MODE_OPTION_SHORT_NAME;

// trace OAH
EXP extern const std::string K_TRACE_OAH_OPTION_LONG_NAME;
EXP extern const std::string K_TRACE_OAH_OPTION_SHORT_NAME;

EXP extern const std::string K_TRACE_OAH_DETAILS_OPTION_LONG_NAME;
EXP extern const std::string K_TRACE_OAH_DETAILS_OPTION_SHORT_NAME;

// trace components
EXP extern const std::string K_TRACE_COMPONENTS_OPTION_LONG_NAME;
EXP extern const std::string K_TRACE_COMPONENTS_OPTION_SHORT_NAME;

// trace passes
EXP extern const std::string K_TRACE_PASSES_OPTION_LONG_NAME;
EXP extern const std::string K_TRACE_PASSES_OPTION_SHORT_NAME;

EXP extern const std::string K_TRACE_ONLY_PASS_OPTION_LONG_NAME;
EXP extern const std::string K_TRACE_ONLY_PASS_OPTION_SHORT_NAME;

#endif // MF_TRACE_IS_ENABLED

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
    mfLanguageKind&       getEarlyLanguageKindRef ()
                              { return fEarlyLanguageKind; }

    void                  setEarlyInsiderOption ();
    Bool                  getEarlyInsiderOption () const
                              { return fEarlyInsiderOption; }
    Bool&                 getEarlyInsiderOptionRef ()
                              { return fEarlyInsiderOption; }

    void                  setEarlyQuietOption ();
    Bool                  getEarlyQuietOption () const
                              { return fEarlyQuietOption; }
    Bool&                 getEarlyQuietOptionRef ()
                              { return fEarlyQuietOption; }

    void                  setEarlyMultiGenerationOutputKind (
                            mfMultiGenerationOutputKind value);
    mfMultiGenerationOutputKind
                          getEarlyMultiGenerationOutputKind () const
                              { return fEarlyMultiGenerationOutputKind; }
    mfMultiGenerationOutputKind&
                          getEarlyMultiGenerationOutputKindRef ()
                              { return fEarlyMultiGenerationOutputKind; }

    const std::list<std::string>&
                          getEarlyIncludeFileNamesList () const
                              { return fEarlyIncludeFileNamesList; }
    const std::list<std::string>&
                          getEarlyIncludeFileNamesListRef ()
                              { return fEarlyIncludeFileNamesList; }

#ifdef MF_TRACE_IS_ENABLED

    void                  setTraceEarlyOptions ();
    Bool                  getTraceEarlyOptions () const
                              { return fTraceEarlyOptions; }
    Bool&                 getTraceEarlyOptionsRef ()
                              { return fTraceEarlyOptions; }

    void                  setEarlyOahVerboseMode ();
    Bool                  getEarlyOahVerboseMode () const
                              { return fEarlyOahVerboseMode; }
    Bool&                 getEarlyOahVerboseModeRef ()
                              { return fEarlyOahVerboseMode; }

    void                  setEarlyTraceOah ();
    Bool                  getEarlyTraceOah () const
                              { return fEarlyTraceOah; }
    Bool&                 getEarlyTraceOahRef ()
                              { return fEarlyTraceOah; }

    void                  setEarlyTraceOahDetails ();
    Bool                  getEarlyTraceOahDetails () const
                              { return fEarlyTraceOahDetails; }
    Bool&                 getEarlyTraceOahDetailsRef ()
                              { return fEarlyTraceOahDetails; }

    void                  setEarlyTraceComponents ();
    Bool                  getEarlyTraceComponents () const
                              { return fEarlyTraceComponents; }
    Bool&                  getEarlyTraceComponentsRef ()
                              { return fEarlyTraceComponents; }


    void                  setEarlyTracePasses ();
    Bool                  getEarlyTracePasses () const
                              { return fEarlyTracePasses; }
    Bool&                 getEarlyTracePassesRef ()
                              { return fEarlyTracePasses; }

    void                  setEarlyTraceOnlyPass (mfPassIDKind passIDKind);
    mfPassIDKind          getEarlyTraceOnlyPass () const
                              { return fEarlyTraceOnlyPass; }
    mfPassIDKind&         getEarlyTraceOnlyPassRef ()
                              { return fEarlyTraceOnlyPass; }

#endif // MF_TRACE_IS_ENABLED

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

    void                  initializeEarlyOptions ();

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

    Bool                  fEarlyInsiderOption;
//     Bool                  fEarlyRegularOption;

    Bool                  fEarlyQuietOption;

    mfMultiGenerationOutputKind
                          fEarlyMultiGenerationOutputKind;

    std::list<std::string>
                          fEarlyIncludeFileNamesList;

#ifdef MF_TRACE_IS_ENABLED

    Bool                  fTraceEarlyOptions;

    Bool                  fEarlyOahVerboseMode;

    Bool                  fEarlyTraceOah;
    Bool                  fEarlyTraceOahDetails;

    Bool                  fEarlyTraceComponents;

    Bool                  fEarlyTracePasses;
    mfPassIDKind          fEarlyTraceOnlyPass;

#endif // MF_TRACE_IS_ENABLED
};
typedef SMARTP<oahEarlyOptions> S_oahEarlyOptions;
EXP std::ostream& operator << (std::ostream& os, const oahEarlyOptions& elt);

//______________________________________________________________________________
// hidden global OAH early options variable
EXP oahEarlyOptions& getGlobalOahEarlyOptions ();

// a handy shortcut
#define gEarlyOptions getGlobalOahEarlyOptions ()


}


#endif // ___oahEarlyOptions___
