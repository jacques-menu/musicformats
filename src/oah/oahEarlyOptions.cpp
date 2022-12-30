/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfStringsHandling.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "oahEarlyOptions.h"

#include "waeOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
//#define DEBUG_EARLY_OPTIONS

//_______________________________________________________________________________
oahEarlyOptions gGlobalOahEarlyOptions;

oahEarlyOptions::oahEarlyOptions ()
{
#ifdef DEBUG_EARLY_OPTIONS
  std::cerr << // JMI
    "Enforcing fTraceEarlyOptions" <<
    std::endl;

#ifdef OAH_TRACING_IS_ENABLED
  fTraceEarlyOptions = true;
#endif

#elsif
#ifdef OAH_TRACING_IS_ENABLED
  fTraceEarlyOptions = false;
#endif

#endif
}

oahEarlyOptions::~oahEarlyOptions ()
{}

//_______________________________________________________________________________
const std::string K_INSIDER_OPTION_LONG_NAME  = "insider";
const std::string K_INSIDER_OPTION_SHORT_NAME = "ins";

void oahEarlyOptions::setEarlyInsiderOption ()
{
#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyInsiderOption" <<
      std::endl;
  }
#endif

  fEarlyInsiderOption = true;
}

// const std::string K_REGULAR_OPTION_LONG_NAME  = "regular";
// const std::string K_REGULAR_OPTION_SHORT_NAME = "reg";
//
// void oahEarlyOptions::setEarlyRegularOption ()
// {
// #ifdef OAH_TRACING_IS_ENABLED
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
#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyQuietOption" <<
      std::endl;
  }
#endif

  fEarlyQuietOption = true;
}

//______________________________________________________________________________
void oahEarlyOptions::setEarlyMultiGenerationOutputKind (
  mfMultiGenerationOutputKind value)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyInsiderOption" <<
      std::endl;
  }
#endif

  fEarlyMultiGenerationOutputKind = value;
}

//______________________________________________________________________________
const std::string K_INCLUDE_OPTION_LONG_NAME  = "include";
const std::string K_INCLUDE_OPTION_SHORT_NAME = "inc";

void oahEarlyOptions::appendEarlyIncludeFileName (std::string includeFileName)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Appending fEarlyIncludeFileName [" <<
      includeFileName <<
      ']' <<
      std::endl;
  }
#endif

  fEarlyIncludeFileNamesList.push_back (includeFileName);
}

#ifdef OAH_TRACING_IS_ENABLED

//_______________________________________________________________________________
const std::string K_TRACE_EARLY_OPTIONS_LONG_OPTION_NAME  ("trace-early-options");
const std::string K_TRACE_EARLY_OPTIONS_SHORT_OPTION_NAME = "teo";

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
const std::string K_OAH_VERBOSE_MODE_LONG_OPTION_NAME  = "oah-verbose-mode";
const std::string K_OAH_VERBOSE_MODE_SHORT_OPTION_NAME = "ovm";

void oahEarlyOptions::setEarlyOahVerboseMode ()
{
#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyOahVerboseMode" <<
      std::endl;
  }
#endif

  fEarlyOahVerboseMode = true;
}

//_______________________________________________________________________________
const std::string K_TRACE_OAH_LONG_OPTION_NAME  = "trace-oah";
const std::string K_TRACE_OAH_SHORT_OPTION_NAME = "toah";

void oahEarlyOptions::setEarlyTracingOah ()
{
#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyTracingOah" <<
      std::endl;
  }
#endif

  fEarlyTracingOah = true;
}

//_______________________________________________________________________________
const std::string K_TRACE_OAH_DETAILS_LONG_OPTION_NAME  = "trace-oah-details";
const std::string K_TRACE_OAH_DETAILS_SHORT_OPTION_NAME = "toahd";

void oahEarlyOptions::setEarlyTracingOahDetails ()
{
#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyTracingOahDetails" <<
      std::endl;
  }
#endif

  fEarlyTracingOahDetails = true;
}

//_______________________________________________________________________________
const std::string K_TRACE_COMPONENTS_LONG_OPTION_NAME  = "trace-components";
const std::string K_TRACE_COMPONENTS_SHORT_OPTION_NAME = "tcomps";

void oahEarlyOptions::setEarlyTraceComponents ()
{
#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyTraceComponents" <<
      std::endl;
  }
#endif

  fEarlyTraceComponents = true;
}

//_______________________________________________________________________________
const std::string K_TRACE_PASSES_LONG_OPTION_NAME  = "trace-passes";
const std::string K_TRACE_PASSES_SHORT_OPTION_NAME = "tpasses";

void oahEarlyOptions::setEarlyTracePasses ()
{
#ifdef OAH_TRACING_IS_ENABLED
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyTracePasses" <<
      std::endl;
  }
#endif

  fEarlyTracePasses = true;
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
#ifdef OAH_TRACING_IS_ENABLED
    if (fTraceEarlyOptions) {
      gLogStream <<
        "Option '-" <<
        optionName <<
        "' has been recognized early" <<
        std::endl;
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

#ifdef OAH_TRACING_IS_ENABLED

  // the trace options are available only if tracing is enabled
  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_EARLY_OPTIONS_LONG_OPTION_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_EARLY_OPTIONS_SHORT_OPTION_NAME)
  ) {
    setTraceEarlyOptions ();
  }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_OAH_VERBOSE_MODE_LONG_OPTION_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_OAH_VERBOSE_MODE_SHORT_OPTION_NAME)
  ) {
    setEarlyOahVerboseMode ();
  }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_OAH_LONG_OPTION_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_OAH_SHORT_OPTION_NAME)
  ) {
    setEarlyTracingOah ();
  }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_OAH_DETAILS_LONG_OPTION_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_OAH_DETAILS_SHORT_OPTION_NAME)
  ) {
    setEarlyTracingOahDetails ();
  }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_COMPONENTS_LONG_OPTION_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_COMPONENTS_SHORT_OPTION_NAME)
  ) {
    setEarlyTraceComponents ();
  }

  if (
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_PASSES_LONG_OPTION_NAME)
      ||
    isEarlyOptionRecognized (
      argumentWithoutDashToBeUsed, K_TRACE_PASSES_SHORT_OPTION_NAME)
  ) {
    setEarlyTracePasses ();
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
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
          gLogStream <<
            "argumentIsAnOption, " <<
            serviceName << " main()" <<
            ", argumentWithoutDash: '" << argumentWithoutDash <<
            "'" <<
            std::endl;
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

#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
        gLogStream <<
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
      }
#endif

      // apply argumentWithoutDashToBeUsed early if it is known as such
      gGlobalOahEarlyOptions.applyEarlyOptionIfRelevant (
        argumentWithoutDashToBeUsed,
        "basic/HelloWorld.xml");
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

#ifdef OAH_TRACING_IS_ENABLED
  if (
    gGlobalOahEarlyOptions.getTraceEarlyOptions ()
      &&
    ! gGlobalOahEarlyOptions.getEarlyQuietOption ()
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
        gGlobalServiceRunData->getServiceName () <<
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
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
          gLogStream <<
            "argumentIsAnOption, " <<
            "??? serviceName" << " main()" <<
            ", argumentWithoutDash: '" << argumentWithoutDash <<
            "'" <<
            std::endl;
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

#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
        gLogStream <<
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
      }
#endif

      // apply argumentWithoutDashToBeUsed early if it is known as such
      gGlobalOahEarlyOptions.applyEarlyOptionIfRelevant (
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

  const int fieldWidth = 26;

  os << std::left <<
    std::setw (fieldWidth) <<
    "EarlyInsiderOption" << ": " << fEarlyInsiderOption <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "EarlyRegularOption" << ": " << fEarlyRegularOption <<
//     std::endl <<

    std::setw (fieldWidth) <<
    "EarlyQuietOption" << ": " << fEarlyQuietOption <<
    std::endl;

#ifdef OAH_TRACING_IS_ENABLED
  os << std::left <<
    std::setw (fieldWidth) <<
    "EarlyOahVerboseMode" << ": " << fEarlyOahVerboseMode <<
    std::endl;
#endif

  os << std::left <<
    std::setw (fieldWidth) <<
    "EarlyIncludeFileNamesList" << ": " <<
    std::endl;

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

#ifdef OAH_TRACING_IS_ENABLED

  os << std::left <<
    std::setw (fieldWidth) <<
    "TraceEarlyOptions" << ": " << fTraceEarlyOptions <<
    std::endl <<

    std::setw (fieldWidth) <<
    "EarlyTracingOah" << ": " << fEarlyTracingOah <<
    std::endl <<
    std::setw (fieldWidth) <<
    "EarlyTracingOahDetails" << ": " << fEarlyTracingOahDetails <<
    std::endl <<

    std::setw (fieldWidth) <<
    "EarlyTraceComponents" << ": " << fEarlyTraceComponents <<
    std::endl <<
    std::setw (fieldWidth) <<
    "EarlyTracePasses" << ": " << fEarlyTracePasses <<
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
