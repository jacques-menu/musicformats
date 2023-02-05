/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfStringsHandling.h"

#include "mfStaticSettings.h"

#include "mfWaeHandlersDutch.h"
#include "mfWaeHandlersEnglish.h"
#include "mfWaeHandlersFrench.h"
#include "mfWaeHandlersGerman.h"
#include "mfWaeHandlersItalian.h"
#include "mfWaeHandlersSpanish.h"

#include "oahWae.h"
#include "oahEarlyOptions.h"

#include "waeOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
//#define DEBUG_EARLY_OPTIONS

//______________________________________________________________________________
// global variable
oahEarlyOptions gGlobalOahEarlyOptions;

//_______________________________________________________________________________
oahEarlyOptions::oahEarlyOptions ()
{
  // cannot use method setEarlyLanguageKind() in constructor

  // english is the default language
  fEarlyLanguageKind =
    mfLanguageKind::kMusicFormatsLanguageEnglish;
//     mfLanguageKind::kMusicFormatsLanguageFrench;

  fMfWaeHandler =
    mfWaeHandlerEnglish::create ();
//     mfWaeHandlerFrench::create ();

#ifdef DEBUG_EARLY_OPTIONS
  std::cerr << // JMI
    "Enforcing fTraceEarlyOptions" <<
    std::endl;
#endif

  initializeEarlyOptions ();
}

oahEarlyOptions::~oahEarlyOptions ()
{}

void oahEarlyOptions::initializeEarlyOptions ()
{
#ifdef MF_TRACING_IS_ENABLED
  fEarlyTraceOnlyPass =
    mfPassIDKind::kMfPassID_ALL; // default value for passes tracing
#endif
}

//_______________________________________________________________________________
const std::string K_LANGUAGE_OPTION_LONG_NAME  = "language";
const std::string K_LANGUAGE_OPTION_SHORT_NAME = "lang";

void oahEarlyOptions::setEarlyLanguageKind (
  mfLanguageKind languageKind)
{
#ifdef MF_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
		std::stringstream ss;

    ss <<
      "Setting fEarlyLanguageKind to " <<
      languageKind <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  fEarlyLanguageKind = languageKind;

  switch (fEarlyLanguageKind) {
    case mfLanguageKind::kMusicFormatsLanguage_UNKNOWN:
      {
        std::stringstream ss;

        ss <<
          "Attempt at setting fEarlyLanguageKind to kMusicFormatsLanguage_UNKNOWN" <<
          std::endl;

        oahError (ss.str ());
      }
      break;
    case mfLanguageKind::kMusicFormatsLanguageEnglish: // default value
      fMfWaeHandler = mfWaeHandlerEnglish::create ();
      break;
    case mfLanguageKind::kMusicFormatsLanguageFrench:
      fMfWaeHandler = mfWaeHandlerFrench::create ();
      break;
    case mfLanguageKind::kMusicFormatsLanguageItalian:
      fMfWaeHandler = mfWaeHandlerItalian::create ();
      break;
    case mfLanguageKind::kMusicFormatsLanguageGerman:
      fMfWaeHandler = mfWaeHandlerGerman::create ();
      break;
    case mfLanguageKind::kMusicFormatsLanguageSpanish:
      fMfWaeHandler = mfWaeHandlerSpanish::create ();
      break;
    case mfLanguageKind::kMusicFormatsLanguageDutch:
      fMfWaeHandler = mfWaeHandlerDutch::create ();
      break;
  } // switch
}

//_______________________________________________________________________________
const std::string K_INSIDER_OPTION_LONG_NAME  = "insider";
const std::string K_INSIDER_OPTION_SHORT_NAME = "ins";

void oahEarlyOptions::setEarlyInsiderOption ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
		std::stringstream ss;

    ss <<
      "Setting fEarlyInsiderOption" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  fEarlyInsiderOption = true;
}

// const std::string K_REGULAR_OPTION_LONG_NAME  = "regular";
// const std::string K_REGULAR_OPTION_SHORT_NAME = "reg";
//
// void oahEarlyOptions::setEarlyRegularOption ()
// {
// #ifdef MF_TRACING_IS_ENABLED
//   if (fTraceEarlyOptions) {
//     gLogStream <<
//       "Setting fEarlyRegularOption" <<
//       std::endl;
//   }
// #endif
//
//   fEarlyRegularOption = true;
// }
//
// Bool oahEarlyOptions::getEarlyRegularOption ()
// {
//   return fEarlyRegularOption;
// }

//_______________________________________________________________________________
const std::string K_QUIET_OPTION_LONG_NAME  = "quiet";
const std::string K_QUIET_OPTION_SHORT_NAME = "q";

void oahEarlyOptions::setEarlyQuietOption ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
		std::stringstream ss;

    ss <<
      "Setting fEarlyQuietOption" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  fEarlyQuietOption = true;
}

//______________________________________________________________________________
void oahEarlyOptions::setEarlyMultiGenerationOutputKind (
  mfMultiGenerationOutputKind value)
{
#ifdef MF_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
		std::stringstream ss;

    ss <<
      "Setting fEarlyMultiGenerationOutputKind" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  fEarlyMultiGenerationOutputKind = value;
}

//______________________________________________________________________________
const std::string K_INCLUDE_OPTION_LONG_NAME  = "include";
const std::string K_INCLUDE_OPTION_SHORT_NAME = "inc";

void oahEarlyOptions::appendEarlyIncludeFileName (std::string includeFileName)
{
#ifdef MF_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
		std::stringstream ss;

    ss <<
      "Appending fEarlyIncludeFileName [" <<
      includeFileName <<
      ']' <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  fEarlyIncludeFileNamesList.push_back (includeFileName);
}

#ifdef MF_TRACING_IS_ENABLED

//_______________________________________________________________________________
const std::string K_TRACE_EARLY_OPTIONS_OPTION_LONG_NAME  ("trace-early-options");
const std::string K_TRACE_EARLY_OPTIONS_OPTION_SHORT_NAME = "teo";

void oahEarlyOptions::setTraceEarlyOptions ()
{
#ifdef DEBUG_EARLY_OPTIONS
  gLogStream <<
    "Setting fTraceEarlyOptions" <<
    std::endl;
#endif

  fTraceEarlyOptions = true;
}

//_______________________________________________________________________________
const std::string K_OAH_VERBOSE_MODE_OPTION_LONG_NAME  = "oah-verbose-mode";
const std::string K_OAH_VERBOSE_MODE_OPTION_SHORT_NAME = "ovm";

void oahEarlyOptions::setEarlyOahVerboseMode ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
		std::stringstream ss;

    ss <<
      "Setting fEarlyOahVerboseMode" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  fEarlyOahVerboseMode = true;
}

//_______________________________________________________________________________
const std::string K_TRACE_OAH_OPTION_LONG_NAME  = "trace-oah";
const std::string K_TRACE_OAH_OPTION_SHORT_NAME = "toah";

void oahEarlyOptions::setEarlyTracingOah ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
		std::stringstream ss;

    ss <<
      "Setting fEarlyTracingOah" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  fEarlyTracingOah = true;
}

//_______________________________________________________________________________
const std::string K_TRACE_OAH_DETAILS_OPTION_LONG_NAME  = "trace-oah-details";
const std::string K_TRACE_OAH_DETAILS_OPTION_SHORT_NAME = "toahd";

void oahEarlyOptions::setEarlyTracingOahDetails ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
		std::stringstream ss;

    ss <<
      "Setting fEarlyTracingOahDetails" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  fEarlyTracingOahDetails = true;
}

//_______________________________________________________________________________
const std::string K_TRACE_COMPONENTS_OPTION_LONG_NAME  = "trace-components";
const std::string K_TRACE_COMPONENTS_OPTION_SHORT_NAME = "tcomps";

void oahEarlyOptions::setEarlyTraceComponents ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
		std::stringstream ss;

    ss <<
      "Setting fEarlyTraceComponents" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  fEarlyTraceComponents = true;
}

//_______________________________________________________________________________
const std::string K_TRACE_PASSES_OPTION_LONG_NAME  = "trace-passes";
const std::string K_TRACE_PASSES_OPTION_SHORT_NAME = "tpasses";

void oahEarlyOptions::setEarlyTracePasses ()
{
  if (fTraceEarlyOptions) {
		std::stringstream ss;

    ss <<
      "Setting fEarlyTracePasses" <<
      std::endl;
  }

  fEarlyTracePasses = true;
}

const std::string K_TRACE_ONLY_PASS_OPTION_LONG_NAME  = "trace-only-pass";
const std::string K_TRACE_ONLY_PASS_OPTION_SHORT_NAME = "topass";

void oahEarlyOptions::setEarlyTraceOnlyPass (mfPassIDKind passIDKind)
{
  if (fTraceEarlyOptions) {
		std::stringstream ss;

    ss <<
      "Setting fEarlyTraceOnlyPass to " << passIDKind <<
      std::endl;
  }

  fEarlyTraceOnlyPass = passIDKind;
}

#endif

//______________________________________________________________________________
Bool oahEarlyOptions::isEarlyOptionRecognized (
  const std::string& theString,
  const std::string& optionName)
{
  Bool result;

//   if (fTraceEarlyOptions) { JMI
//     gLogStream <<
//       "isEarlyOptionRecognized()" <<
//       ", theString: " << theString <<
//       ", optionName: " << optionName <<
//       std::endl;
//   }

  if (theString == optionName) {
#ifdef MF_TRACING_IS_ENABLED
    if (fTraceEarlyOptions) {
      std::stringstream ss;

      ss <<
        "Option '-" <<
        optionName <<
        "' has been recognized early" <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif

    result = true;
  }

  return result;
}

void oahEarlyOptions::applyEarlyOptionIfRelevant (
  const std::string& argumentWithoutDashToBeUsed,
  const std::string& optionValue)
{
  // this is OAH handling pass 1
  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_LANGUAGE_OPTION_LONG_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_LANGUAGE_OPTION_SHORT_NAME)
  ) {
    setEarlyLanguageKind (
      mfLanguageKindFromString (optionValue));
  }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_INSIDER_OPTION_LONG_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_INSIDER_OPTION_SHORT_NAME)
  ) {
    setEarlyInsiderOption ();
  }
//   if (
//     isEarlyOptionRecognized (
//       argumentWithoutDashToBeUsed, K_REGULAR_OPTION_LONG_NAME)
//   ) {
//     setEarlyRegularOption ();
//   }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_QUIET_OPTION_LONG_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_QUIET_OPTION_SHORT_NAME)
  ) {
    setEarlyInsiderOption ();
  }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME)
  ) {
    setEarlyMultiGenerationOutputKind (
      mfMultiGenerationOutputKind::kGenerationLilypond);
  }
  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_GENERATED_OUTPUT_KIND_BRAILLE_NAME)
  ) {
    setEarlyMultiGenerationOutputKind (
      mfMultiGenerationOutputKind::kGenerationBraille);
  }
  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_GENERATED_OUTPUT_KIND_MUSICXML_NAME)
  ) {
    setEarlyMultiGenerationOutputKind (
      mfMultiGenerationOutputKind::kGenerationMusicXML);
  }
  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_GENERATED_OUTPUT_KIND_GUIDO_NAME)
  ) {
    setEarlyMultiGenerationOutputKind (
      mfMultiGenerationOutputKind::kGenerationGuido);
  }
  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_GENERATED_OUTPUT_KIND_MIDI_NAME)
  ) {
    setEarlyMultiGenerationOutputKind (
      mfMultiGenerationOutputKind::kGenerationMidi);
  }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_INCLUDE_OPTION_LONG_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_INCLUDE_OPTION_SHORT_NAME)
  ) {
    appendEarlyIncludeFileName (optionValue);
  }

#ifdef MF_TRACING_IS_ENABLED

  // the trace options are available only if tracing is enabled
  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_EARLY_OPTIONS_OPTION_LONG_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_EARLY_OPTIONS_OPTION_SHORT_NAME)
  ) {
    setTraceEarlyOptions ();
  }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_OAH_VERBOSE_MODE_OPTION_LONG_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_OAH_VERBOSE_MODE_OPTION_SHORT_NAME)
  ) {
    setEarlyOahVerboseMode ();
  }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_OAH_OPTION_LONG_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_OAH_OPTION_SHORT_NAME)
  ) {
    setEarlyTracingOah ();
  }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_OAH_DETAILS_OPTION_LONG_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_OAH_DETAILS_OPTION_SHORT_NAME)
  ) {
    setEarlyTracingOahDetails ();
  }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_COMPONENTS_OPTION_LONG_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_COMPONENTS_OPTION_SHORT_NAME)
  ) {
    setEarlyTraceComponents ();
  }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_PASSES_OPTION_LONG_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_PASSES_OPTION_SHORT_NAME)
  ) {
    setEarlyTracePasses ();
  }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_ONLY_PASS_OPTION_LONG_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_ONLY_PASS_OPTION_SHORT_NAME)
  ) {
    setEarlyTraceOnlyPass (
      mfPassIDKindFromString (optionValue));
  }

#endif
}

void oahEarlyOptions::applyEarlyOptionsIfPresentInArgcArgv (
  int   argc,
  char* argv[])
{
  std::string serviceName = argv [0];

  for (int i = 1; i < argc; ++i) {
    std::string argumentAsString = std::string (argv [i]);

    Bool   argumentIsAnOption;
    std::string argumentWithoutDash;

    // is argumentAsString a double of single dash?
    if (argumentAsString.substr (0, 2) == "--") {
      argumentIsAnOption = true;

      argumentWithoutDash = argumentAsString.substr (2);
    }
    else if (argumentAsString [0] == '-') {
      argumentIsAnOption = true;

      argumentWithoutDash = argumentAsString.substr (1);
    }

    if (argumentIsAnOption) {
#ifdef MF_TRACING_IS_ENABLED
      if (getTraceEarlyOptions ()) {
        std::stringstream ss;

        ss <<
          "argumentIsAnOption, " <<
          serviceName << " main()" <<
          ", argumentWithoutDash: '" << argumentWithoutDash <<
          "'" <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif

      // is argumentWithoutDash starting with a prefix?
      Bool   argumentWithoutDashStartsWithAPrefix;
      std::string argumentWithoutDashToBeUsed = argumentWithoutDash;

      if (argumentWithoutDash.substr (0, 2) == "t=") {
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
         std::string ("t") + argumentWithoutDash.substr (2);
      }
      else if (argumentWithoutDash.substr (0, 2) == "t-") { // JMI CLAR
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
         std::string ("t") + argumentWithoutDash.substr (2);
      }
      else if (argumentWithoutDash.substr (0, 6) == "trace=") {
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
          std::string ("trace-") + argumentWithoutDash.substr (6);
      }
      else if (argumentWithoutDash.substr (0, 6) == "trace-") { // JMI CLAR
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
          std::string ("trace-") + argumentWithoutDash.substr (6);
      }

#ifdef MF_TRACING_IS_ENABLED
      if (getTraceEarlyOptions ()) {
        std::stringstream ss;

        ss <<
          serviceName <<
          " applyEarlyOptionsIfPresentInArgcArgv()" <<
          ", argumentWithoutDash: '" <<
          argumentWithoutDash <<
          ", argumentWithoutDashStartsWithAPrefix: '" <<
          argumentWithoutDashStartsWithAPrefix <<
          ", argumentWithoutDashToBeUsed: '" <<
          argumentWithoutDashToBeUsed <<
          "'" <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif

      // apply argumentWithoutDashToBeUsed early if it is known as such

      // fetch the option value if any
      std::string optionValue;

      if (i < argc - 1) { // JMI v0.9.66
        optionValue = std::string (argv [i + 1]);
      }

      applyEarlyOptionIfRelevant (
        argumentWithoutDashToBeUsed,
        optionValue);
    }
  } // for
}

void oahEarlyOptions::applyEarlyOptionsIfPresentInOptionsAndArguments (
  oahOptionsAndArguments& optionsAndArguments)
{
  const std::vector<std::string>&
    argumentsVector =
      optionsAndArguments.getArgumentsVector ();

  size_t argumentsNumber =
    argumentsVector.size ();

#ifdef MF_TRACING_IS_ENABLED
  if (
    getTraceEarlyOptions ()
      &&
    ! getEarlyQuietOption ()
  ) {
    if (argumentsNumber > 0) {
      gLogStream <<
        "applyEarlyOptionsIfPresentInOptionsAndArguments()," <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        ' ' <<
        argumentsNumber <<
        ' ' <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        " in handler arguments vector for " <<
        gGlobalCurrentServiceRunData->getServiceName () <<
        ":" <<
        std::endl;

      ++gIndenter;

      for (size_t i = 0; i < argumentsNumber; ++i) {
        gLogStream <<
          i << ": " << argumentsVector [i] <<
            std::endl;
      } // for

      gLogStream << std::endl;

      --gIndenter;
    }
    else {
      gLogStream <<
        "There are no arguments to " <<
        "??? fHandlerServiceName" <<
        std::endl;
    }
  }
#endif

  for (size_t i = 0; i < argumentsNumber; ++i) {
    std::string argumentAsString = argumentsVector [i];

    Bool   argumentIsAnOption;
    std::string argumentWithoutDash;

    // is argumentAsString a double of single dash?
    if (argumentAsString.substr (0, 2) == "--") {
      argumentIsAnOption = true;

      argumentWithoutDash = argumentAsString.substr (2);
    }
    else if (argumentAsString [0] == '-') {
      argumentIsAnOption = true;

      argumentWithoutDash = argumentAsString.substr (1);
    }

    if (argumentIsAnOption) {
#ifdef MF_TRACING_IS_ENABLED
      if (getTraceEarlyOptions ()) {
          std::stringstream ss;

          ss <<
            "argumentIsAnOption, " <<
            "??? serviceName" << " main()" <<
            ", argumentWithoutDash: '" << argumentWithoutDash <<
            "'" <<
            std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif

      // is argumentWithoutDash starting with a prefix?
      Bool   argumentWithoutDashStartsWithAPrefix;
      std::string argumentWithoutDashToBeUsed = argumentWithoutDash;

      if (argumentWithoutDash.substr (0, 2) == "t=") {
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
         std::string ("t") + argumentWithoutDash.substr (2);
      }
      else if (argumentWithoutDash.substr (0, 2) == "t-") { // JMI CLAR
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
         std::string ("t") + argumentWithoutDash.substr (2);
      }
      else if (argumentWithoutDash.substr (0, 6) == "trace=") {
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
          std::string ("trace-") + argumentWithoutDash.substr (6);
      }
      else if (argumentWithoutDash.substr (0, 6) == "trace-") { // JMI CLAR
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
          std::string ("trace-") + argumentWithoutDash.substr (6);
      }

#ifdef MF_TRACING_IS_ENABLED
      if (getTraceEarlyOptions ()) {
        std::stringstream ss;

        ss <<
          "??? serviceName" <<
          " applyEarlyOptionsIfPresentInArgcArgv()" <<
          ", argumentWithoutDash: '" <<
          argumentWithoutDash <<
          ", argumentWithoutDashStartsWithAPrefix: '" <<
          argumentWithoutDashStartsWithAPrefix <<
          ", argumentWithoutDashToBeUsed: '" <<
          argumentWithoutDashToBeUsed <<
          "'" <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif

      // apply argumentWithoutDashToBeUsed early if it is known as such
      applyEarlyOptionIfRelevant (
        argumentWithoutDashToBeUsed,
        "basic/HelloWorld.xml");
    }
  } // for
}

void oahEarlyOptions::print (std::ostream& os) const
{
  os <<
    "Early options values:" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 32;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fEarlyLanguageKind" << ": " << fEarlyLanguageKind <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fEarlyInsiderOption" << ": " << fEarlyInsiderOption <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "EarlyRegularOption" << ": " << fEarlyRegularOption <<
//     std::endl <<

    std::setw (fieldWidth) <<
    "fEarlyQuietOption" << ": " << fEarlyQuietOption <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEarlyMultiGenerationOutputKind" << ": " << fEarlyMultiGenerationOutputKind <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fEarlyIncludeFileNamesList" << ": ";

  if (fEarlyIncludeFileNamesList.size ()) {
    os << std::endl;

    ++gIndenter;

    for ( std::string includeFileName : fEarlyIncludeFileNamesList ) {
      os <<
        '[' << includeFileName << ']' <<
        std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << "[NONE]" << std::endl;
  }

#ifdef MF_TRACING_IS_ENABLED

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTraceEarlyOptions" << ": " << fTraceEarlyOptions <<
    std::endl;

#ifdef MF_TRACING_IS_ENABLED
  os << std::left <<
    std::setw (fieldWidth) <<
    "fEarlyOahVerboseMode" << ": " << fEarlyOahVerboseMode <<
    std::endl;
#endif

  os << std::left <<
    std::setw (fieldWidth) <<
    "fEarlyTracingOah" << ": " << fEarlyTracingOah <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEarlyTracingOahDetails" << ": " << fEarlyTracingOahDetails <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEarlyTraceComponents" << ": " << fEarlyTraceComponents <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEarlyTracePasses" << ": " << fEarlyTracePasses <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEarlyTraceOnlyPass" << ": " << fEarlyTraceOnlyPass <<
    std::endl;

#endif

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const oahEarlyOptions& elt)
{
  elt.print (os);
  return os;
}


}
