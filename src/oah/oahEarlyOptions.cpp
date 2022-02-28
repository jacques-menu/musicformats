/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <iomanip>      // setw, setprecision, ...

#include "mfStringsHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahEarlyOptions.h"

#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
//#define DEBUG_EARLY_OPTIONS

//_______________________________________________________________________________
oahEarlyOptions gGlobalOahEarlyOptions;

oahEarlyOptions::oahEarlyOptions ()
{
#ifdef DEBUG_EARLY_OPTIONS
  cerr << // JMI
    "Enforcing fTraceEarlyOptions" <<
    endl;

  fTraceEarlyOptions = true;
#elsif
  fTraceEarlyOptions = false;
#endif
}

oahEarlyOptions::~oahEarlyOptions ()
{}

//_______________________________________________________________________________
const string K_INSIDER_OPTION_LONG_NAME  = "insider";
const string K_INSIDER_OPTION_SHORT_NAME = "ins";

void oahEarlyOptions::setEarlyInsiderOption ()
{
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyInsiderOption" <<
      endl;
  }

  fEarlyInsiderOption = true;
}

// const string K_REGULAR_OPTION_LONG_NAME  = "regular";
// const string K_REGULAR_OPTION_SHORT_NAME = "reg";
//
// void oahEarlyOptions::setEarlyRegularOption ()
// {
//   if (fTraceEarlyOptions) {
//     gLogStream <<
//       "Setting fEarlyRegularOption" <<
//       endl;
//   }
//
//   fEarlyRegularOption = true;
// }
//
// Bool oahEarlyOptions::getEarlyRegularOption ()
// {
//   return fEarlyRegularOption;
// }

//_______________________________________________________________________________
const string K_QUIET_OPTION_LONG_NAME  = "quiet";
const string K_QUIET_OPTION_SHORT_NAME = "q";

void oahEarlyOptions::setEarlyQuietOption ()
{
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyQuietOption" <<
      endl;
  }

  fEarlyQuietOption = true;
}

//______________________________________________________________________________
void oahEarlyOptions::setEarlyMultiGenerationOutputKind (
  mfMultiGenerationOutputKind value)
{
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyInsiderOption" <<
      endl;
  }

  fEarlyMultiGenerationOutputKind = value;
}

//______________________________________________________________________________
const string K_INCLUDE_OPTION_LONG_NAME  = "include";
const string K_INCLUDE_OPTION_SHORT_NAME = "inc";

void oahEarlyOptions::appendEarlyIncludeFileName (string includeFileName)
{
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Appending fEarlyIncludeFileName [" <<
      includeFileName <<
      "]" <<
      endl;
  }

  fEarlyIncludeFileNamesList.push_back (includeFileName);
}

#ifdef TRACING_IS_ENABLED

//_______________________________________________________________________________
const string K_TRACE_EARLY_OPTIONS_LONG_OPTION_NAME  ("trace-early-options");
const string K_TRACE_EARLY_OPTIONS_SHORT_OPTION_NAME = "teo";

void oahEarlyOptions::setTraceEarlyOptions ()
{
#ifdef DEBUG_EARLY_OPTIONS
  gLogStream <<
    "Setting fTraceEarlyOptions" <<
    endl;
#endif

  fTraceEarlyOptions = true;
}

//_______________________________________________________________________________
const string K_OAH_VERBOSE_MODE_LONG_OPTION_NAME  = "oah-verbose-mode";
const string K_OAH_VERBOSE_MODE_SHORT_OPTION_NAME = "ovm";

void oahEarlyOptions::setEarlyOahVerboseMode ()
{
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyOahVerboseMode" <<
      endl;
  }

  fEarlyOahVerboseMode = true;
}

//_______________________________________________________________________________
const string K_TRACE_OAH_LONG_OPTION_NAME  = "trace-oah";
const string K_TRACE_OAH_SHORT_OPTION_NAME = "toah";

void oahEarlyOptions::setEarlyTracingOah ()
{
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyTracingOah" <<
      endl;
  }

  fEarlyTracingOah = true;
}

//_______________________________________________________________________________
const string K_TRACE_OAH_DETAILS_LONG_OPTION_NAME  = "trace-oah-details";
const string K_TRACE_OAH_DETAILS_SHORT_OPTION_NAME = "toahd";

void oahEarlyOptions::setEarlyTracingOahDetails ()
{
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyTracingOahDetails" <<
      endl;
  }

  fEarlyTracingOahDetails = true;
}

//_______________________________________________________________________________
const string K_TRACE_COMPONENTS_LONG_OPTION_NAME  = "trace-components";
const string K_TRACE_COMPONENTS_SHORT_OPTION_NAME = "tcomps";

void oahEarlyOptions::setEarlyTraceComponents ()
{
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyTraceComponents" <<
      endl;
  }

  fEarlyTraceComponents = true;
}

//_______________________________________________________________________________
const string K_TRACE_PASSES_LONG_OPTION_NAME  = "trace-passes";
const string K_TRACE_PASSES_SHORT_OPTION_NAME = "tpasses";

void oahEarlyOptions::setEarlyTracePasses ()
{
  if (fTraceEarlyOptions) {
    gLogStream <<
      "Setting fEarlyTracePasses" <<
      endl;
  }

  fEarlyTracePasses = true;
}

#endif

//______________________________________________________________________________
Bool oahEarlyOptions::isEarlyOptionRecognized (
  const string& theString,
  const string& optionName)
{
  Bool result;

//   if (fTraceEarlyOptions) { JMI
//     gLogStream <<
//       "isEarlyOptionRecognized()" <<
//       ", theString: " << theString <<
//       ", optionName: " << optionName <<
//       endl;
//   }

  if (theString == optionName) {
    if (fTraceEarlyOptions) {
      gLogStream <<
        "Option '-" <<
        optionName <<
        "' has been recognized early" <<
        endl;
    }

    result = true;
  }

  return result;
}

void oahEarlyOptions::applyEarlyOptionIfRelevant (
  const string& argumentWithoutDashToBeUsed,
  const string& optionValue)
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

#ifdef TRACING_IS_ENABLED

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
  string serviceName = argv [0];

	for (int i = 1; i < argc; ++i) {
	  string argumentAsString = string (argv [i]);

    Bool   argumentIsAnOption;
    string argumentWithoutDash;

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
      if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
          gLogStream <<
            "argumentIsAnOption, " <<
            serviceName << " main()" <<
            ", argumentWithoutDash: '" << argumentWithoutDash <<
            "'" <<
            endl;
      }

      // is argumentWithoutDash starting with a prefix?
      Bool   argumentWithoutDashStartsWithAPrefix;
      string argumentWithoutDashToBeUsed = argumentWithoutDash;

      if (argumentWithoutDash.substr (0, 2) == "t=") {
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
         string ("t") + argumentWithoutDash.substr (2);
      }
      else if (argumentWithoutDash.substr (0, 2) == "t-") { // JMI CLAR
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
         string ("t") + argumentWithoutDash.substr (2);
      }
      else if (argumentWithoutDash.substr (0, 6) == "trace=") {
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
          string ("trace-") + argumentWithoutDash.substr (6);
      }
      else if (argumentWithoutDash.substr (0, 6) == "trace-") { // JMI CLAR
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
          string ("trace-") + argumentWithoutDash.substr (6);
      }

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
          endl;
      }

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
  const vector<string>&
    argumentsVector =
      optionsAndArguments.getArgumentsVector ();

  unsigned int argumentsNumber =
    argumentsVector.size ();

#ifdef TRACING_IS_ENABLED
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
        " " <<
        argumentsNumber <<
        " " <<
        mfSingularOrPluralWithoutNumber (
          argumentsNumber, "argument", "arguments") <<
        " in handler arguments vector for " <<
        gGlobalServiceRunData->getServiceName () <<
        ":" <<
        endl;

      ++gIndenter;

      for (unsigned int i = 0; i < argumentsNumber; ++i) {
        gLogStream <<
          i << " : " << argumentsVector [i] <<
            endl;
      } // for

      gLogStream << endl;

      --gIndenter;
    }
    else {
      gLogStream <<
        "There are no arguments to " <<
        "??? fHandlerServiceName" <<
        endl;
    }
  }
#endif

  for (unsigned int i = 0; i < argumentsNumber; ++i) {
    string argumentAsString = argumentsVector [i];

    Bool   argumentIsAnOption;
    string argumentWithoutDash;

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
      if (gGlobalOahEarlyOptions.getTraceEarlyOptions ()) {
          gLogStream <<
            "argumentIsAnOption, " <<
            "??? serviceName" << " main()" <<
            ", argumentWithoutDash: '" << argumentWithoutDash <<
            "'" <<
            endl;
      }

      // is argumentWithoutDash starting with a prefix?
      Bool   argumentWithoutDashStartsWithAPrefix;
      string argumentWithoutDashToBeUsed = argumentWithoutDash;

      if (argumentWithoutDash.substr (0, 2) == "t=") {
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
         string ("t") + argumentWithoutDash.substr (2);
      }
      else if (argumentWithoutDash.substr (0, 2) == "t-") { // JMI CLAR
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
         string ("t") + argumentWithoutDash.substr (2);
      }
      else if (argumentWithoutDash.substr (0, 6) == "trace=") {
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
          string ("trace-") + argumentWithoutDash.substr (6);
      }
      else if (argumentWithoutDash.substr (0, 6) == "trace-") { // JMI CLAR
        argumentWithoutDashStartsWithAPrefix = true;

        argumentWithoutDashToBeUsed =
          string ("trace-") + argumentWithoutDash.substr (6);
      }

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
          endl;
      }

      // apply argumentWithoutDashToBeUsed early if it is known as such
      gGlobalOahEarlyOptions.applyEarlyOptionIfRelevant (
        argumentWithoutDashToBeUsed,
        "basic/HelloWorld.xml");
    }
  } // for
}

void oahEarlyOptions::print (ostream& os) const
{
  os <<
    "Early options values:" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 26;

  os << left <<
    setw (fieldWidth) <<
    "EarlyInsiderOption" << " : " << fEarlyInsiderOption <<
    endl <<
//     setw (fieldWidth) <<
//     "EarlyRegularOption" << " : " << fEarlyRegularOption <<
//     endl <<

    setw (fieldWidth) <<
    "EarlyQuietOption" << " : " << fEarlyQuietOption <<
    endl <<

    setw (fieldWidth) <<
    "TraceEarlyOptions" << " : " << fTraceEarlyOptions <<
    endl <<

    setw (fieldWidth) <<
    "EarlyOahVerboseMode" << " : " << fEarlyOahVerboseMode <<
    endl <<

    setw (fieldWidth) <<
    "EarlyTracingOah" << " : " << fEarlyTracingOah <<
    endl <<
    setw (fieldWidth) <<
    "EarlyTracingOahDetails" << " : " << fEarlyTracingOahDetails <<
    endl <<

    setw (fieldWidth) <<
    "EarlyTraceComponents" << " : " << fEarlyTraceComponents <<
    endl <<
    setw (fieldWidth) <<
    "EarlyTracePasses" << " : " << fEarlyTracePasses <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "EarlyIncludeFileNamesList" << " : ";

  if (fEarlyIncludeFileNamesList.size ()) {
    os <<
      endl;

    ++gIndenter;

    for (auto fileName : fEarlyIncludeFileNamesList) {
    os <<
      "[" << fileName << "]" <<
      endl;
    } // for

    --gIndenter;
  }
  else {
    os << "none" << endl;
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const oahEarlyOptions& elt)
{
  elt.print (os);
  return os;
}


}
