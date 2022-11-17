/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <cstring> // memset
#include <signal.h>

#include <fstream>

#include "mfServiceRunData.h"
#include "mfStringsHandling.h"
#include "mfTiming.h"

#include "wae.h"
#include "oahWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"

#include "libmusicxml.h" // for mfMusicformatsErrorKind

#include "oahOah.h"
#include "waeOah.h"
#include "displayOah.h"

#include "oahEarlyOptions.h"

#include "mfcLibraryComponent.h"
#include "msdlComponent.h"
#include "msdlConverterComponent.h"

#include "msdlConverterInsiderHandler.h"
#include "msdlConverterRegularHandler.h"

#include "msdl2lilypondInterface.h"
#include "msdl2brailleInterface.h"
#include "msdl2musicxmlInterface.h"
#include "msdl2guidoInterface.h"


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
#endif


// //------------------------------------------------------------------------
// void enforceSomeOptions (
//   mfMultiGenerationOutputKind multiGenerationOutputKind)
// //------------------------------------------------------------------------
// {
//   /*
//     This is a way to enforce options 'permanently'
//     independently of the options chosen by the user
//   */
//
//   // trace
//   // ------------------------------------------------------
//
// /*
//   gGlobalTracingOahGroup->setTraceScores ();
//   gGlobalTracingOahGroup->setTracePartGroups ();
//   gGlobalTracingOahGroup->setTraceParts ();
//   gGlobalTracingOahGroup->setTraceStaves ();
//   gGlobalTracingOahGroup->setTraceVoices ();
//   gGlobalTracingOahGroup->setTraceSegments ();
//   gGlobalTracingOahGroup->setTraceMeasures ();
//   gGlobalTracingOahGroup->setTraceNotes ();
// */
//
//   // MSR
//   // ------------------------------------------------------
//
// /*
//   gGlobalMsrOahGroup->setTraceMsr ();
//   gGlobalMsrOahGroup->setTraceMsrVisitors ();
//   gGlobalMsrOahGroup->setDisplayMsr ();
//
//   gGlobalMsrOahGroup->setTraceMsrDurations ();
// */
//
//   // generate code dependant specific options
//   // ------------------------------------------------------
//
//   switch (multiGenerationOutputKind) {
//     case mfMultiGenerationOutputKind::kGeneration_NO_:
//       // should not occur
//       break;
//
//     case mfMultiGenerationOutputKind::kGenerationLilypond:
//       // LPSR
//       // ------------------------------------------------------
//
// /*
//       gGlobalLpsrOahGroup->setTraceLpsr ();
//       gGlobalLpsrOahGroup->setTraceLpsrVisitors ();
//       gGlobalLpsrOahGroup->setDisplayLpsr ();
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
//       gGlobalBsrOahGroup->setTraceBsr ();
//       //  gGlobalBsrOahGroup->setTraceBsrVisitors ();
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
  mfMusicformatsErrorKind result = mfMusicformatsErrorKind::kMusicformatsError_NO_;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_NO_:
      // should not occur, unless the run is a pure help one
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
      result =
        convertMsdlStream2lilypondWithHandler (
          "stdin",
          std::cin,
          handler,
          gOutputStream,
          gLogStream);
      break;

    case mfMultiGenerationOutputKind::kGenerationBraille:
      result =
        convertMsdlStream2brailleWithHandler (
          "stdin",
          std::cin,
          handler,
          gOutputStream,
          gLogStream);
      break;

    case mfMultiGenerationOutputKind::kGenerationMusicXML:
      result =
        convertMsdlStream2musicxmlWithHandler (
          "stdin",
          std::cin,
          handler,
          gOutputStream,
          gLogStream);
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:
      result =
        convertMsdlStream2guidoWithHandler (
          "stdin",
          std::cin,
          handler,
          gOutputStream,
          gLogStream);
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
  mfMusicformatsErrorKind result = mfMusicformatsErrorKind::kMusicformatsError_NO_;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_NO_:
      // should not occur, unless the run is a pure help one
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
      result =
        convertMsdlFile2lilypondWithHandler (
          inputFileName,
          handler,
          gOutputStream,
          gLogStream);
      break;

    case mfMultiGenerationOutputKind::kGenerationBraille:
      result =
        convertMsdlFile2brailleWithHandler (
          inputFileName,
          handler,
          gOutputStream,
          gLogStream);
      break;

    case mfMultiGenerationOutputKind::kGenerationMusicXML:
      result =
        convertMsdlFile2musicxmlWithHandler (
          inputFileName,
          handler,
          gOutputStream,
          gLogStream);
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:
      result =
        msdlFile2guidoWithHandler (
          inputFileName,
          handler,
          gOutputStream,
          gLogStream);
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

//  setEarlyTracingOah (); // JMI TEMP

  // the service name
  // ------------------------------------------------------

  std::string serviceName = argv [0];

  // create the global output and log indented streams
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (std::cout, std::cerr);

  // apply early options if any
  // ------------------------------------------------------

  gGlobalOahEarlyOptions.applyEarlyOptionsIfPresentInArgcArgv (
    argc,
    argv);

  // has the '-insider' option been used?
  // ------------------------------------------------------

  Bool insiderOption =
    gGlobalOahEarlyOptions.getEarlyInsiderOption ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      serviceName << " main()" <<
      ", insiderOption: " << insiderOption <<
      std::endl;
  }
#endif

  // fetch the generated output kind from optionsAndArguments,
  // right now before any OAH handler is created,
  // since the OAH handler should only use the OAH groups it needs
  // depending on the generated output kind
  // ------------------------------------------------------

  mfMultiGenerationOutputKind
    multiGenerationOutputKind =
      gGlobalOahEarlyOptions.
        getEarlyMultiGenerationOutputKind ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> multiGenerationOutputKind: " <<
      mfMultiGenerationOutputKindAsString (multiGenerationOutputKind) <<
      std::endl;
  }
#endif

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_NO_:
      // wait until after help options have been handled
      // before issuing an error message
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
    case mfMultiGenerationOutputKind::kGenerationBraille:
    case mfMultiGenerationOutputKind::kGenerationMusicXML:
    case mfMultiGenerationOutputKind::kGenerationGuido:
      break;

    case mfMultiGenerationOutputKind::kGenerationMidi:
      gLogStream <<
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

    gGlobalServiceRunData =
      mfServiceRunData::create (
        serviceName);

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
  }
  catch (mfOahException& e) {
    mfDisplayException (e, gOutputStream);
    return (int) mfMusicformatsErrorKind::kMusicformatsErrorInvalidOption;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return (int) mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // has the output kind been chosen?
  // ------------------------------------------------------

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_NO_:
      {
        std::stringstream s;

        s <<
          serviceName <<
          " needs the output kind to be suplied thru one of the options" <<
          " '-" << K_GENERATED_OUTPUT_KIND_GUIDO_NAME << "'" <<
          ", '-" << K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME << "'" <<
          ", '-" << K_GENERATED_OUTPUT_KIND_BRAILLE_NAME << "'" <<
          ", '-" << K_GENERATED_OUTPUT_KIND_MUSICXML_NAME << "'" <<
          " or '-" << K_GENERATED_OUTPUT_KIND_MIDI_NAME << "'" <<
          ", otherwise the run is a pure help one";

        oahWarning (s.str ());
      }
      break;

    default:
      ;
  } // switch

  // check indentation
  // ------------------------------------------------------

  if (gIndenter != 0) {
    gLogStream <<
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
    gLogStream <<
      "The OAH handler contains:" <<
      std::endl;

    ++gIndenter;
    handler->print (gLogStream);
    --gIndenter;
  }

  if (gGlobalDisplayOahGroup->getDisplayOahHandlerSummary ()) {
    gLogStream <<
      "The summary of the OAH handler contains:" <<
      std::endl;

    ++gIndenter;
    handler->printSummary (gLogStream);
    --gIndenter;
  }

  if (gGlobalDisplayOahGroup->getDisplayOahHandlerEssentials ()) {
    gLogStream <<
      "The essentials of the OAH handler contains:" <<
      std::endl;

    ++gIndenter;
    handler->printHandlerEssentials (
      gLogStream,
      30); // fieldWidth
    --gIndenter;
  }

  // let's go ahead
  // ------------------------------------------------------

  std::string
    inputSourceName =
      gGlobalServiceRunData->getInputSourceName ();

  std::string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      serviceName << ": " <<
      "inputSourceName = \"" << inputSourceName << "\"" <<
      ", outputFileName = \"" << outputFileName << "\"" <<
      std::endl <<
      separator <<
      std::endl;
  }
#endif

  // set the desired options
  // ------------------------------------------------------

//  enforceSomeOptions (multiGenerationOutputKind);

  // should we generate LilyPond, Braille, MusicXML or Guido?
  // ------------------------------------------------------


  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // welcome message
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    int
      outputFileNameSize =
        outputFileName.size ();

    gLogStream <<
      "This is " << serviceName << ' ' <<
      getGlobalMusicFormatsVersionNumberAndDate () <<
      std::endl;

    gLogStream <<
      "Launching the conversion of ";

    if (inputSourceName == MSDR_STANDARD_INPUT_NAME) {
      gLogStream <<
        "standard input";
    }
    else {
      gLogStream <<
        "\"" << inputSourceName << "\"";
    }

    gLogStream <<
      " into " <<
      mfMultiGenerationOutputKindAsString (multiGenerationOutputKind) <<
      std::endl;

    gLogStream <<
      "Time is " <<
      gGlobalServiceRunData->getRunDateFull () <<
      std::endl;

    gLogStream <<
      "The command line is:" <<
      std::endl;

    ++gIndenter;
    gLogStream <<
      handler->
        getLaunchCommandAsSupplied () <<
      std::endl;
    --gIndenter;

    gLogStream <<
      "or with options long names:" <<
      std::endl;

    ++gIndenter;
    gLogStream <<
      handler->
        getLaunchCommandWithLongOptionsNames () <<
      std::endl;
    --gIndenter;

    gLogStream <<
      "or with options short names:" <<
      std::endl;

    ++gIndenter;
    gLogStream <<
      handler->
        getLaunchCommandWithShortOptionsNames () <<
      std::endl;
    --gIndenter;

    gLogStream <<
      "LilyPond code will be written to "; // JMI
    if (outputFileNameSize) {
      gLogStream <<
        outputFileName;
    }
    else {
      gLogStream <<
        "standard output";
    }
    gLogStream << std::endl;
  }
#endif

  // acknoledge end of command line analysis
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    gLogStream <<
      "The command line options and arguments have been analyzed" <<
      std::endl;
  }
#endif

  // do the job
  // ------------------------------------------------------

  mfMusicformatsErrorKind err = mfMusicformatsErrorKind::kMusicformatsError_NO_;// JMI

  // generate code
  try {
    if (inputSourceName == MSDR_STANDARD_INPUT_NAME) {
      // MSDL data comes from standard input
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream << "Reading standard input" << std::endl;
      }
#endif

      err =
        generateCodeFromStandardInput (
          handler,
          multiGenerationOutputKind);
    }

    else {
      // MSDL data comes from a file
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
        gLogStream << "Reading file \"" << inputSourceName << "\"" << std::endl;
      }
#endif

    err =
      generateCodeFromAFile (
        inputSourceName,
        handler,
        multiGenerationOutputKind);
    }

#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      if (err != mfMusicformatsErrorKind::kMusicformatsError_NO_) {
        gLogStream <<
          serviceName << ", " <<
          mfMultiGenerationOutputKindAsString (multiGenerationOutputKind) <<
          ", err = " <<
          mfMusicFormatsErrorAsString (err) <<
          std::endl;
      }
    }
#endif
  }
  catch (mfException& e) {
    mfDisplayException (e, gOutputStream);
    return (int) mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return (int) mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  // display the input line numbers for which messages have been issued
  // ------------------------------------------------------

  displayWarningsAndErrorsInputLineNumbers ();

  // print timing information
  // ------------------------------------------------------

  if (gGlobalDisplayOahGroup->getDisplayCPUusage ()) {
    gLogStream <<
      mfTimingItemsList::gGlobalTimingItemsList;
  }

  // check indentation
  // ------------------------------------------------------

  if (gIndenter != 0) {
    gLogStream <<
      "### " << serviceName << " gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }

  // over!
  // ------------------------------------------------------

  if (err != mfMusicformatsErrorKind::kMusicformatsError_NO_) {
    gLogStream <<
      "### The generation of " <<
      mfMultiGenerationOutputKindAsString (multiGenerationOutputKind) <<
      " output failed ###" <<
      std::endl;
  }

  switch (err) {
    case mfMusicformatsErrorKind::kMusicformatsError_NO_:
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
