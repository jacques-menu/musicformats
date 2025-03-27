/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfStringsHandling.h"

#include "mfPreprocessorSettings.h"

#include "languages.h"

#include "oahWae.h"

#include "oahEarlyOptions.h"

#include "waeOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
//#define DEBUG_EARLY_OPTIONS

//_______________________________________________________________________________
oahEarlyOptions::oahEarlyOptions ()
{
  // cannot use method setEarlyLanguageKind() in constructor

  // english is the default language
  fEarlyLanguageKind =
    mfLanguageKind::kMusicFormatsLanguageEnglish;
//     mfLanguageKind::kMusicFormatsLanguageFrench;

  // english is the default language
  setGlobalLanguage (
    mfLanguageKind::kMusicFormatsLanguageEnglish);

#ifdef DEBUG_EARLY_OPTIONS
  std::cerr << // JMI
    "Enforcing fTraceEarlyOptions" <<
    std::endl;
#endif // DEBUG_EARLY_OPTIONS

  initializeEarlyOptions ();
}

oahEarlyOptions::~oahEarlyOptions ()
{}

void oahEarlyOptions::initializeEarlyOptions ()
{
#ifdef MF_TRACE_IS_ENABLED
  fEarlyTraceOnlyPass =
    mfPassIDKind::kMfPassID_ALL; // default value for passes trace
#endif // MF_TRACE_IS_ENABLED
}

//_______________________________________________________________________________
const std::string K_LANGUAGE_OPTION_LONG_NAME  = "language";
const std::string K_LANGUAGE_OPTION_SHORT_NAME = "lang";

void oahEarlyOptions::setEarlyLanguageKind (
  mfLanguageKind languageKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceEarlyOptions) {
    std::stringstream ss;

    ss <<
      "Setting fEarlyLanguageKind to " <<
      languageKind;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fEarlyLanguageKind = languageKind;

  setGlobalLanguage (fEarlyLanguageKind);
}

//_______________________________________________________________________________
const std::string K_INSIDER_OPTION_LONG_NAME  = "insider";
const std::string K_INSIDER_OPTION_SHORT_NAME = "ins";

void oahEarlyOptions::setEarlyInsiderOption ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceEarlyOptions) {
    std::stringstream ss;

    ss <<
      "Setting fEarlyInsiderOption";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fEarlyInsiderOption = true;
}

// const std::string K_REGULAR_OPTION_LONG_NAME  = "regular";
// const std::string K_REGULAR_OPTION_SHORT_NAME = "reg";
//
// void oahEarlyOptions::setEarlyRegularOption ()
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (fTraceEarlyOptions) {
//     gLog <<
//       "Setting fEarlyRegularOption" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
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
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceEarlyOptions) {
    std::stringstream ss;

    ss <<
      "Setting fEarlyQuietOption";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fEarlyQuietOption = true;
}

//______________________________________________________________________________
void oahEarlyOptions::setEarlyMultiGenerationOutputKind (
  mfMultiGenerationOutputKind value)
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceEarlyOptions) {
    std::stringstream ss;

    ss <<
      "Setting fEarlyMultiGenerationOutputKind";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fEarlyMultiGenerationOutputKind = value;
}

//______________________________________________________________________________
const std::string K_INCLUDE_OPTION_LONG_NAME  = "include";
const std::string K_INCLUDE_OPTION_SHORT_NAME = "incl";

void oahEarlyOptions::appendEarlyIncludeFileName (std::string includeFileName)
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceEarlyOptions) {
    std::stringstream ss;

    ss <<
      "Appending fEarlyIncludeFileName [" <<
      includeFileName <<
      ']';

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fEarlyIncludeFileNamesList.push_back (includeFileName);
}

#ifdef MF_TRACE_IS_ENABLED

//_______________________________________________________________________________
const std::string K_TRACE_EARLY_OPTIONS_OPTION_LONG_NAME  ("trace-early-options");
const std::string K_TRACE_EARLY_OPTIONS_OPTION_SHORT_NAME = "tearlyopts";

void oahEarlyOptions::setTraceEarlyOptions ()
{
#ifdef DEBUG_EARLY_OPTIONS
  gLog <<
    "Setting fTraceEarlyOptions" <<
    std::endl;
#endif // DEBUG_EARLY_OPTIONS

  fTraceEarlyOptions = true;
}

//_______________________________________________________________________________
const std::string K_OAH_VERBOSE_MODE_OPTION_LONG_NAME  = "oah-verbose-mode";
const std::string K_OAH_VERBOSE_MODE_OPTION_SHORT_NAME = "oahverbmpde";

void oahEarlyOptions::setEarlyOahVerboseMode ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceEarlyOptions) {
    std::stringstream ss;

    ss <<
      "Setting fEarlyOahVerboseMode";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fEarlyOahVerboseMode = true;
}

//_______________________________________________________________________________
const std::string K_TRACE_OAH_OPTION_LONG_NAME  = "trace-oah";
const std::string K_TRACE_OAH_OPTION_SHORT_NAME = "toah";

void oahEarlyOptions::setEarlyTraceOah ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceEarlyOptions) {
    std::stringstream ss;

    ss <<
      "Setting fEarlyTraceOah";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fEarlyTraceOah = true;
}

//_______________________________________________________________________________
const std::string K_TRACE_OAH_DETAILS_OPTION_LONG_NAME  = "trace-oah-details";
const std::string K_TRACE_OAH_DETAILS_OPTION_SHORT_NAME = "toahdets";

void oahEarlyOptions::setEarlyTraceOahDetails ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceEarlyOptions) {
    std::stringstream ss;

    ss <<
      "Setting fEarlyTraceOahDetails";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fEarlyTraceOahDetails = true;
}

//_______________________________________________________________________________
const std::string K_TRACE_COMPONENTS_OPTION_LONG_NAME  = "trace-components";
const std::string K_TRACE_COMPONENTS_OPTION_SHORT_NAME = "tcomps";

void oahEarlyOptions::setEarlyTraceComponents ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (fTraceEarlyOptions) {
    std::stringstream ss;

    ss <<
      "Setting fEarlyTraceComponents";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
const std::string K_TRACE_ONLY_PASS_OPTION_SHORT_NAME = "tonlypass";

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

#endif // MF_TRACE_IS_ENABLED

//______________________________________________________________________________
Bool oahEarlyOptions::isEarlyOptionRecognized (
  const std::string& theString,
  const std::string& optionName)
{
  Bool result;

//   if (fTraceEarlyOptions) { JMI
//     gLog <<
//       "isEarlyOptionRecognized()" <<
//       ", theString: " << theString <<
//       ", optionName: " << optionName <<
//       std::endl;
//   }

  if (theString == optionName) {
#ifdef MF_TRACE_IS_ENABLED
    if (fTraceEarlyOptions) {
      std::stringstream ss;

      ss <<
        "Option '-" <<
        optionName <<
        "' has been recognized early";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED

  // the trace options are available only if trace is enabled
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
    setEarlyTraceOah ();
  }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_OAH_DETAILS_OPTION_LONG_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_OAH_DETAILS_OPTION_SHORT_NAME)
  ) {
    setEarlyTraceOahDetails ();
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

#endif // MF_TRACE_IS_ENABLED
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
#ifdef MF_TRACE_IS_ENABLED
      if (getTraceEarlyOptions ()) {
        std::stringstream ss;

        ss <<
          "argumentIsAnOption, " <<
          serviceName << " main()" <<
          ", argumentWithoutDash: '" << argumentWithoutDash <<
          '\'' <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
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
          '\'' <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // apply argumentWithoutDashToBeUsed early if it is known as such

      // fetch the option value if any
      std::string optionValue;

      if (i < argc - 1) { // JMI 0.9.66
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
  const std::vector <std::string>&
    argumentsVector =
      optionsAndArguments.getArgumentsVector ();

  size_t argumentsNumber =
    argumentsVector.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (
    getTraceEarlyOptions ()
      &&
    ! getEarlyQuietOption ()
  ) {
    if (argumentsNumber > 0) {
      gLog <<
        "applyEarlyOptionsIfPresentInOptionsAndArguments()," <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "There is", "There are") <<
        ' ' <<
        argumentsNumber <<
        ' ' <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        " in handler arguments vector for " <<
        gServiceRunData->getServiceName () <<
        ":" <<
        std::endl;

      ++gIndenter;

      for (size_t i = 0; i < argumentsNumber; ++i) {
        gLog <<
          i << ": " << argumentsVector [i] <<
            std::endl;
      } // for

      gLog << std::endl;

      --gIndenter;
    }
    else {
      gLog <<
        "There are no arguments to " <<
        "??? fHandlerServiceName" <<
        std::endl;
    }
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
      if (getTraceEarlyOptions ()) {
          std::stringstream ss;

          ss <<
            "argumentIsAnOption, " <<
            "??? serviceName" << " main()" <<
            ", argumentWithoutDash: '" << argumentWithoutDash <<
            '\'' <<
            std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
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
          '\'' <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

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

  constexpr int fieldWidth = 32;

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
    os << "[NULL]" << std::endl;
  }

#ifdef MF_TRACE_IS_ENABLED

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTraceEarlyOptions" << ": " << fTraceEarlyOptions <<
    std::endl;

#ifdef MF_TRACE_IS_ENABLED
  os << std::left <<
    std::setw (fieldWidth) <<
    "fEarlyOahVerboseMode" << ": " << fEarlyOahVerboseMode <<
    std::endl;
#endif // MF_TRACE_IS_ENABLED

  os << std::left <<
    std::setw (fieldWidth) <<
    "fEarlyTraceOah" << ": " << fEarlyTraceOah <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEarlyTraceOahDetails" << ": " << fEarlyTraceOahDetails <<
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

#endif // MF_TRACE_IS_ENABLED

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const oahEarlyOptions& elt)
{
  elt.print (os);
  return os;
}

//________________________________________________________________________
// hidden global OAH early options variable
EXP oahEarlyOptions pGlobalOahEarlyOptions;

EXP oahEarlyOptions& getGlobalOahEarlyOptions ()
{
  return pGlobalOahEarlyOptions;
}


}
