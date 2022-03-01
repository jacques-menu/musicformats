/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <cstring>
#include <signal.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <fstream>

#include "mfServiceRunData.h"
#include "mfStringsHandling.h"
#include "mfTiming.h"

#include "wae.h"
#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"

#include "libmusicxml.h" // for mfMusicformatsError

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


using namespace std;

using namespace MusicFormats;

//_______________________________________________________________________________
#ifndef WIN32
static void _sigaction (int signal, siginfo_t *si, void *arg)
{
  cerr << "Signal #" << signal << " catched!" << endl;
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
//     case mfMultiGenerationOutputKind::k_NoGeneration:
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

mfMusicformatsError generateCodeFromStandardInput (
  S_oahHandler             handler,
  mfMultiGenerationOutputKind multiGenerationOutputKind)
{
  mfMusicformatsError result = mfMusicformatsError::k_NoError;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::k_NoGeneration:
      // should not occur, unless the run is a pure help one
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
      result =
        convertMsdlStream2lilypondWithHandler (
          "stdin",
          cin,
          handler,
          gOutputStream,
          gLogStream);
      break;

    case mfMultiGenerationOutputKind::kGenerationBraille:
      result =
        convertMsdlStream2brailleWithHandler (
          "stdin",
          cin,
          handler,
          gOutputStream,
          gLogStream);
      break;

    case mfMultiGenerationOutputKind::kGenerationMusicXML:
      result =
        convertMsdlStream2musicxmlWithHandler (
          "stdin",
          cin,
          handler,
          gOutputStream,
          gLogStream);
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:
      result =
        convertMsdlStream2guidoWithHandler (
          "stdin",
          cin,
          handler,
          gOutputStream,
          gLogStream);
      break;

    case mfMultiGenerationOutputKind::kGenerationMidi:
      break;
  } // switch

  return result;
}

mfMusicformatsError generateCodeFromAFile (
  string              inputFileName,
  S_oahHandler        handler,
  mfMultiGenerationOutputKind multiGenerationOutputKind)
{
  mfMusicformatsError result = mfMusicformatsError::k_NoError;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::k_NoGeneration:
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

  string serviceName = argv [0];

  // create the global output and log indented streams
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (cout, cerr);

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
      endl;
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
      endl;
  }
#endif

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::k_NoGeneration:
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
        endl;

      return 0;
      break;
  } // switch

  // the oahHandler, set below
  // ------------------------------------------------------

  S_oahHandler handler;

  try {
    // create an msdlConverter insider OAH handler
    // ------------------------------------------------------

    S_msdlConverterInsiderHandler
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
    return (int) mfMusicformatsError::kErrorInvalidOption;
  }
  catch (exception& e) {
    mfDisplayException (e, gOutputStream);
    return (int) mfMusicformatsError::kErrorInvalidFile;
  }

  // has the output kind been chosen?
  // ------------------------------------------------------

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::k_NoGeneration:
      {
        stringstream s;

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
      gIndenter.getIndent () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  // display the OAH handler if needed
  // ------------------------------------------------------

  if (gGlobalDisplayOahGroup->getDisplayOahHandler ()) {
    gLogStream <<
      "The OAH handler contains:" <<
      endl;

    ++gIndenter;
    handler->print (gLogStream);
    --gIndenter;
  }

  if (gGlobalDisplayOahGroup->getDisplayOahHandlerSummary ()) {
    gLogStream <<
      "The summary of the OAH handler contains:" <<
      endl;

    ++gIndenter;
    handler->printSummary (gLogStream);
    --gIndenter;
  }

  if (gGlobalDisplayOahGroup->getDisplayOahHandlerEssentials ()) {
    gLogStream <<
      "The essentials of the OAH handler contains:" <<
      endl;

    ++gIndenter;
    handler->printHandlerEssentials (
      gLogStream,
      30); // fieldWidth
    --gIndenter;
  }

  // let's go ahead
  // ------------------------------------------------------

  string
    inputSourceName =
      gGlobalServiceRunData->getInputSourceName ();

  string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      serviceName << ": " <<
      "inputSourceName = \"" << inputSourceName << "\"" <<
      ", outputFileName = \"" << outputFileName << "\"" <<
      endl <<
      separator <<
      endl;
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
      "This is " << serviceName << " " <<
      getGlobalMusicFormatsVersionNumberAndDate () <<
      endl;

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
      endl;

    gLogStream <<
      "Time is " <<
      gGlobalServiceRunData->getRunDateFull () <<
      endl;

    gLogStream <<
      "The command line is:" <<
      endl;

    ++gIndenter;
    gLogStream <<
      handler->
        getLaunchCommandAsSupplied () <<
      endl;
    --gIndenter;

    gLogStream <<
      "or with options long names:" <<
      endl;

    ++gIndenter;
    gLogStream <<
      handler->
        getLaunchCommandWithLongOptionsNames () <<
      endl;
    --gIndenter;

    gLogStream <<
      "or with options short names:" <<
      endl;

    ++gIndenter;
    gLogStream <<
      handler->
        getLaunchCommandWithShortOptionsNames () <<
      endl;
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
    gLogStream << endl;
  }
#endif

  // acknoledge end of command line analysis
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    gLogStream <<
      "The command line options and arguments have been analyzed" <<
      endl;
  }
#endif

  // do the job
  // ------------------------------------------------------

  mfMusicformatsError err = mfMusicformatsError::k_NoError;// JMI

  // generate code
  try {
    if (inputSourceName == MSDR_STANDARD_INPUT_NAME) {
      // MSDL data comes from standard input
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
      gLogStream << "Reading standard input" << endl;
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
        gLogStream << "Reading file \"" << inputSourceName << "\"" << endl;
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
      if (err != mfMusicformatsError::k_NoError) {
        gLogStream <<
          serviceName << ", " <<
          mfMultiGenerationOutputKindAsString (multiGenerationOutputKind) <<
          ", err = " <<
          mfMusicFormatsErrorAsString (err) <<
          endl;
      }
    }
#endif
  }
  catch (mfException& e) {
    mfDisplayException (e, gOutputStream);
    return (int) mfMusicformatsError::kErrorInvalidFile;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return (int) mfMusicformatsError::kErrorInvalidFile;
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
      gIndenter.getIndent () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  // over!
  // ------------------------------------------------------

  if (err != mfMusicformatsError::k_NoError) {
    gLogStream <<
      "### The generation of " <<
      mfMultiGenerationOutputKindAsString (multiGenerationOutputKind) <<
      " output failed ###" <<
      endl;
  }

  switch (err) {
    case mfMusicformatsError::k_NoError:
      return 0;
      break;
    case mfMusicformatsError::kErrorInvalidFile:
      return 1;
      break;
    case mfMusicformatsError::kErrorInvalidOption:
      return 2;
      break;
    case mfMusicformatsError::kErrorUnsupported:
      return 3;
      break;
  } // switch
}
