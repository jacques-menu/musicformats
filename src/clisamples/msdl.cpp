/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <cstring> // memset
#include <signal.h>

#include <fstream>

#include "mfPreprocessorSettings.h"

#include "mfServices.h"
#include "mfStringsHandling.h"
#include "mfTiming.h"

#include "waeInterface.h"
#include "oahWae.h"

#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind

#include "oahOah.h"
#include "waeOah.h"
#include "oahDisplayOah.h"

#include "oahEarlyOptions.h"

#include "mfLibraryComponent.h"
#include "msdlComponent.h"
#include "msdlConverterComponent.h"

#include "msdlConverterInsiderHandler.h"
#include "msdlConverterRegularHandler.h"

#include "msdl2lilypondInterface.h"
#include "msdl2brailleInterface.h"
#include "msdl2musicxmlInterface.h"
#include "msdl2guidoInterface.h"

#include "waeHandlers.h"


using namespace MusicFormats;

//_______________________________________________________________________________
#ifndef WIN32
static void _sigaction (int signal, siginfo_t *si, void *arg)
{
  std::cerr << "Signal #" << signal << " catched!" << std::endl;
  exit (-2);
}

static void catchSignals ()
{
  struct sigaction sa;

  memset (&sa, 0, sizeof(struct sigaction));

  sigemptyset (&sa.sa_mask);

  sa.sa_sigaction = _sigaction;
  sa.sa_flags     = SA_SIGINFO;

  sigaction (SIGSEGV, &sa, NULL);
  sigaction (SIGILL,  &sa, NULL);
  sigaction (SIGFPE,  &sa, NULL);
}
#else
static void catchSignals () {}
#endif // WIN32


// //------------------------------------------------------------------------
// void enforceSomeOptions (
//   mfMultiGenerationOutputKind multiGenerationOutputKind)
// //------------------------------------------------------------------------
// {
//   /*
//     This is a way to enforce options 'permanently'
//     independently of the options selected by the user
//   */
//
//   // trace
//   // ------------------------------------------------------
//
// /*
//   gTraceOahGroup->setTraceScores ();
//   gTraceOahGroup->setTracePartGroups ();
//   gTraceOahGroup->setTraceParts ();
//   gTraceOahGroup->setTraceStaves ();
//   gTraceOahGroup->setTraceVoices ();
//   gTraceOahGroup->setTraceSegments ();
//   gTraceOahGroup->setTraceMeasures ();
//   gTraceOahGroup->setTraceNotes ();
// */
//
//   // MSR
//   // ------------------------------------------------------
//
// /*
//   gMsrOahGroup->setTraceMsr ();
//   gMsrOahGroup->setTraceMsrVisitors ();
//   gMsrOahGroup->setDisplayMsr ();
//
//   gMsrOahGroup->setTraceMsrNotesDurations ();
// */
//
//   // generate code dependant specific options
//   // ------------------------------------------------------
//
//   switch (multiGenerationOutputKind) {
//     case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
//       // should not occur
//       break;
//
//     case mfMultiGenerationOutputKind::kGenerationLilypond:
//       // LPSR
//       // ------------------------------------------------------
//
// /*
//       gLpsrOahGroup->setTraceLpsr ();
//       gLpsrOahGroup->setTraceLpsrVisitors ();
//       gLpsrOahGroup->setDisplayLpsr ();
// */
//
//       // lpsr2lilypond
//       // ------------------------------------------------------
//
// /*
//       gGlobalLpsr2lilypondOahGroup->setLilypondRunDate (); // JMI NOT OK
//       gGlobalLpsr2lilypondOahGroup->setInputLineNumbers ();
// */
//       break;
//
//     case mfMultiGenerationOutputKind::kGenerationBraille:
//       // BSR
//       // ------------------------------------------------------
//
// /*
//       gBsrOahGroup->setTraceBsr ();
//       //  gBsrOahGroup->setTraceBsrVisitors ();
// */
//
//       // bsr2braille
//       // ------------------------------------------------------
//
// /*
//       gGlobalBrailleGenerationOahGroup->
//         setBrailleOutputKind (bsrBrailleOutputKind::kBrailleOutputUTF8Debug);
// */
//       break;
//
//     case mfMultiGenerationOutputKind::kGenerationMusicXML:
//       // MusicXML
//       // ------------------------------------------------------
//
// /*
//       gGlobalMsr2mxsrOahGroup->setTraceDivisions ();
//
//       gGlobalMxsrOahGroup->setTraceMxsrVisitors ();
// */
//       break;
//
//     case mfMultiGenerationOutputKind::kGenerationGuido:
// /*
//      gGlobalMsr2mxsrOahGroup->setTraceDivisions ();
// */
//      break;
//
//     case mfMultiGenerationOutputKind::kGenerationMidi:
// /*
// */
//       break;
//   } // switch
// }

mfMusicformatsErrorKind generateCodeFromStandardInput (
  S_oahHandler             handler,
  mfMultiGenerationOutputKind multiGenerationOutputKind)
{
  mfMusicformatsErrorKind result = mfMusicformatsErrorKind::kMusicformatsError_NONE;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
      // should not occur, unless the run is a pure help one
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
      result =
        convertMsdlStream2lilypondWithHandler (
          "stdin",
          std::cin,
          handler,
          gOutput,
          gLog);
      break;

    case mfMultiGenerationOutputKind::kGenerationBraille:
      result =
        convertMsdlStream2brailleWithHandler (
          "stdin",
          std::cin,
          handler,
          gOutput,
          gLog);
      break;

    case mfMultiGenerationOutputKind::kGenerationMusicXML:
      result =
        convertMsdlStream2musicxmlWithHandler (
          "stdin",
          std::cin,
          handler,
          gOutput,
          gLog);
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:
      result =
        convertMsdlStream2guidoWithHandler (
          "stdin",
          std::cin,
          handler,
          gOutput,
          gLog);
      break;

    case mfMultiGenerationOutputKind::kGenerationMidi:
      break;
  } // switch

  return result;
}

mfMusicformatsErrorKind generateCodeFromAFile (
  std::string         inputFileName,
  S_oahHandler        handler,
  mfMultiGenerationOutputKind multiGenerationOutputKind)
{
  mfMusicformatsErrorKind result = mfMusicformatsErrorKind::kMusicformatsError_NONE;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
      // should not occur, unless the run is a pure help one
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
      result =
        convertMsdlFile2lilypondWithHandler (
          inputFileName,
          handler,
          gOutput,
          gLog);
      break;

    case mfMultiGenerationOutputKind::kGenerationBraille:
      result =
        convertMsdlFile2brailleWithHandler (
          inputFileName,
          handler,
          gOutput,
          gLog);
      break;

    case mfMultiGenerationOutputKind::kGenerationMusicXML:
      result =
        convertMsdlFile2musicxmlWithHandler (
          inputFileName,
          handler,
          gOutput,
          gLog);
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:
      result =
        msdlFile2guidoWithHandler (
          inputFileName,
          handler,
          gOutput,
          gLog);
      break;

    case mfMultiGenerationOutputKind::kGenerationMidi:
      break;
  } // switch

  return result;
}

//------------------------------------------------------------------------
int main (int argc, char*  argv[])
//------------------------------------------------------------------------
{
  // setup signals catching
  // ------------------------------------------------------

  catchSignals ();

//  setEarlyTraceOah (); // JMI TEMP

  // the service name
  // ------------------------------------------------------

  std::string serviceName = argv [0];

  // create the global output and log indented streams
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (std::cout, std::cerr);

  // apply early options if any
  // ------------------------------------------------------

  gEarlyOptions.applyEarlyOptionsIfPresentInArgcArgv (
    argc,
    argv);

  // has the '-insider' option been used?
  // ------------------------------------------------------

  Bool insiderOption =
    gEarlyOptions.getEarlyInsiderOption ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      serviceName << " main()" <<
      ", insiderOption: " << insiderOption;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // fetch the generated output kind from optionsAndArguments,
  // right now before any OAH handler is created,
  // since the OAH handler should only use the OAH groups it needs
  // depending on the generated output kind
  // ------------------------------------------------------

  mfMultiGenerationOutputKind
    multiGenerationOutputKind =
      gEarlyOptions.
        getEarlyMultiGenerationOutputKind ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> multiGenerationOutputKind: " <<
      mfMultiGenerationOutputKindAsString (multiGenerationOutputKind);

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
      // wait until after help options have been handled
      // before issuing an error message
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
    case mfMultiGenerationOutputKind::kGenerationBraille:
    case mfMultiGenerationOutputKind::kGenerationMusicXML:
    case mfMultiGenerationOutputKind::kGenerationGuido:
      break;

    case mfMultiGenerationOutputKind::kGenerationMidi:
      gLog <<
        "MIDI output is not implemented yet, sorry" <<
        std::endl;

      return 0;
      break;
  } // switch

  // the oahHandler, set below
  // ------------------------------------------------------

  S_oahHandler handler;

  try {
    // create an msdlConverter insider OAH handler
    // ------------------------------------------------------

    const S_msdlConverterInsiderHandler&
      insiderOahHandler =
        msdlConverterInsiderHandler::create (
          serviceName,
          serviceName + " insider OAH handler with argc/argv",
          multiGenerationOutputKind);

    // the OAH handler to be used, a regular handler is the default
    // ------------------------------------------------------

    if (insiderOption) {
      // use the insider msdlConverter OAH handler
      handler = insiderOahHandler;
    }
    else {
      // create a regular msdlConverter OAH handler
      handler =
        msdlConverterRegularHandler::create (
          serviceName,
          serviceName + " regular OAH handler with argc/argv",
          insiderOahHandler,
          multiGenerationOutputKind);
    }

    // create the global run data
    // ------------------------------------------------------

    setGlobalServiceRunData (
      mfServiceRunData::create (
        serviceName));

    // handle the command line options and arguments
    // ------------------------------------------------------

    // handle the options and arguments from argc/argv
    oahElementHelpOnlyKind
      helpOnlyKind =
        handler->
          handleOptionsAndArgumentsFromArgcArgv (
            argc,
            argv);

    // have help options been used?
    switch (helpOnlyKind) {
      case oahElementHelpOnlyKind::kElementHelpOnlyYes:
        return 0; // quit now
        break;
      case oahElementHelpOnlyKind::kElementHelpOnlyNo:
        // go ahead
        break;
    } // switch
  } // try

  catch (mfOahException& e) {
    mfDisplayException (e, gOutput);
    return (int) mfMusicformatsErrorKind::kMusicformatsErrorInvalidOption;
  }
  catch ( std::exception& e) {
    mfDisplayException (e, gOutput);
    return (int) mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // has the output kind been selected?
  // ------------------------------------------------------

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
      {
        std::stringstream ss;

        ss <<
          serviceName <<
          " needs the output kind to be suplied thru one of the options" <<
          " '-" << K_GENERATED_OUTPUT_KIND_GUIDO_NAME << '\'' <<
          ", '-" << K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME << '\'' <<
          ", '-" << K_GENERATED_OUTPUT_KIND_BRAILLE_NAME << '\'' <<
          ", '-" << K_GENERATED_OUTPUT_KIND_MUSICXML_NAME << '\'' <<
          " or '-" << K_GENERATED_OUTPUT_KIND_MIDI_NAME << '\'' <<
          ", otherwise the run is a pure help one";

        oahWarning (ss.str ());
      }
      break;

    default:
      ;
  } // switch

  // check indentation
  // ------------------------------------------------------

  if (gIndenter != 0) {
    gLog <<
      "### " <<
      serviceName <<
      " gIndenter value after options ands arguments checking: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }

  // display the OAH handler if needed
  // ------------------------------------------------------

  if (gGlobalDisplayOahGroup->getDisplayOahHandler ()) {
    gLog <<
      "The OAH handler contains:" <<
      std::endl;

    ++gIndenter;
    handler->print (gLog);
    --gIndenter;
  }

  if (gGlobalDisplayOahGroup->getDisplayOahHandlerSummary ()) {
    gLog <<
      "The summary of the OAH handler contains:" <<
      std::endl;

    ++gIndenter;
    handler->printSummary (gLog);
    --gIndenter;
  }

  if (gGlobalDisplayOahGroup->getDisplayOahHandlerEssentials ()) {
    gLog <<
      "The essentials of the OAH handler contains:" <<
      std::endl;

    ++gIndenter;
    handler->printHandlerEssentials (
      gLog,
      30); // fieldWidth
    --gIndenter;
  }

  // let's go ahead
  // ------------------------------------------------------

  std::string
    inputSourceName =
      gServiceRunData->getInputSourceName ();

  std::string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream ss;

    ss <<
      serviceName << ": " <<
      "inputSourceName = \"" << inputSourceName << "\"" <<
      ", outputFileName = \"" << outputFileName << "\"" <<
      std::endl <<
      separator;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set the desired options
  // ------------------------------------------------------

//  enforceSomeOptions (multiGenerationOutputKind);

  // should we generate LilyPond, Braille, MusicXML or Guido?
  // ------------------------------------------------------


  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gEarlyOptions.getEarlyQuietOption ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // welcome message
  // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    int
      outputFileNameSize =
        outputFileName.size ();

    gLog <<
      "This is " << serviceName << ' ' <<
      getGlobalMusicFormatsVersionNumberAndDate () <<
      std::endl;

    gLog <<
      "Launching the conversion of ";

    if (inputSourceName == MSDR_STANDARD_INPUT_NAME) {
      gLog <<
        "standard input";
    }
    else {
      gLog <<
        "\"" << inputSourceName << "\"";
    }

    gLog <<
      " into " <<
      mfMultiGenerationOutputKindAsString (multiGenerationOutputKind) <<
      std::endl;

    gLog <<
      "Time is " <<
      gServiceRunData->getRunDateFull () <<
      std::endl;

    gLog <<
      "The command line is:" <<
      std::endl;

    ++gIndenter;
    gLog <<
      handler->
        getLaunchCommandAsSupplied () <<
      std::endl;
    --gIndenter;

    gLog <<
      "or with options long names:" <<
      std::endl;

    ++gIndenter;
    gLog <<
      handler->
        getLaunchCommandWithLongOptionsNames () <<
      std::endl;
    --gIndenter;

    gLog <<
      "or with options short names:" <<
      std::endl;

    ++gIndenter;
    gLog <<
      handler->
        getLaunchCommandWithShortOptionsNames () <<
      std::endl;
    --gIndenter;

    gLog <<
      "LilyPond code will be written to "; // JMI
    if (outputFileNameSize) {
      gLog <<
        outputFileName;
    }
    else {
      gLog <<
        "standard output";
    }
    gLog << std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  // acknoledge end of command line analysis
  // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    std::stringstream ss;

    ss <<
      "The command line options and arguments have been analyzed";

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // do the job
  // ------------------------------------------------------

  mfMusicformatsErrorKind err = mfMusicformatsErrorKind::kMusicformatsError_NONE;// JMI

  // generate code
  try {
    if (inputSourceName == MSDR_STANDARD_INPUT_NAME) {
      // MSDL data comes from standard input
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptions ()) {
      gLog << "Reading standard input" << std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      err =
        generateCodeFromStandardInput (
          handler,
          multiGenerationOutputKind);
    }

    else {
      // MSDL data comes from a file
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptions ()) {
        gLog << "Reading file \"" << inputSourceName << "\"" << std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

    err =
      generateCodeFromAFile (
        inputSourceName,
        handler,
        multiGenerationOutputKind);
    }

#ifdef MF_TRACE_IS_ENABLED
    if (gEarlyOptions.getTraceEarlyOptions ()) {
      if (err != mfMusicformatsErrorKind::kMusicformatsError_NONE) {
        gLog <<
          serviceName << ", " <<
          mfMultiGenerationOutputKindAsString (multiGenerationOutputKind) <<
          ", err: \"" <<
          mfMusicFormatsErrorAsString (err) <<
          std::endl;
      }
    }
#endif // MF_TRACE_IS_ENABLED
  } // try

  catch (mfException& e) {
    mfDisplayException (e, gOutput);
    return (int) mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch ( std::exception& e) {
    mfDisplayException (e, gOutput);
    return (int) mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // display the input line numbers for which messages have been issued
  // ------------------------------------------------------

  displayWarningsAndErrorsInputLineNumbers ();

  // print timing information
  // ------------------------------------------------------

  if (gGlobalDisplayOahGroup->getDisplayCPUusage ()) {
    gLog <<
      gGlobalTimingItemsList;
  }

  // check indentation
  // ------------------------------------------------------

  if (gIndenter != 0) {
    gLog <<
      "### " << serviceName << " gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }

  // over!
  // ------------------------------------------------------

  if (err != mfMusicformatsErrorKind::kMusicformatsError_NONE) {
    gLog <<
      "### The generation of " <<
      mfMultiGenerationOutputKindAsString (multiGenerationOutputKind) <<
      " output failed ###" <<
      std::endl;
  }

  switch (err) {
    case mfMusicformatsErrorKind::kMusicformatsError_NONE:
      return 0;
      break;
    case mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile:
      return 1;
      break;
    case mfMusicformatsErrorKind::kMusicformatsErrorInvalidOption:
      return 2;
      break;
    case mfMusicformatsErrorKind::kMusicformatsErrorUnsupported:
      return 3;
      break;
  } // switch
}
