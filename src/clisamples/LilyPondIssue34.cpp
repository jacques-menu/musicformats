/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <cstring>      // memset
#include <signal.h>

#include <fstream>

#include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind

#include "mfPreprocessorSettings.h"

#include "mfPasses.h"
#include "mfServices.h"
#include "mfStringsHandling.h"
#include "mfTiming.h"

#include "msrScores.h"

#include "LilyPondIssue34Generator.h"

#include "oahOah.h"
#include "oahWae.h"
#include "oahDisplayOah.h"

#include "waeInterface.h"

#include "msrGenerationOah.h"

#include "oahEarlyOptions.h"

#include "LilyPondIssue34Component.h"

#include "LilyPondIssue34InsiderHandler.h"
#include "LilyPondIssue34RegularHandler.h"

#include "msr2lilypondInterface.h"
#include "msr2brailleInterface.h"
#include "msr2musicxmlInterface.h"
#include "msr2guidoInterface.h"

#include "waeHandlers.h"


using namespace MusicFormats;

//_______________________________________________________________________________
#ifdef MF_CATCH_SIGNALS_IS_ENABLED

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
  sigaction (SIGILL, &sa, NULL);
  sigaction (SIGFPE, &sa, NULL);
}

#else
static void catchSignals ()  {}
#endif // WIN32

#endif // MF_CATCH_SIGNALS_IS_ENABLED

//------------------------------------------------------------------------
void enforceSomeOptions (
  mfMultiGenerationOutputKind multiGenerationOutputKind)
//------------------------------------------------------------------------
{
  /*
    This is a way to enforce options 'permanently'
    independently of the options selected by the user
  */

  // trace
  // ------------------------------------------------------

/*
  gTraceOahGroup->setTraceScores ();
  gTraceOahGroup->setTracePartGroups ();
  gTraceOahGroup->setTraceParts ();
  gTraceOahGroup->setTraceStaves ();
  gTraceOahGroup->setTraceVoices ();
  gTraceOahGroup->setTraceSegments ();
  gTraceOahGroup->setTraceMeasures ();
  gTraceOahGroup->setTraceNotes ();
*/

  // MSR
  // ------------------------------------------------------

/*
  gMsrOahGroup->setTraceMsr ();
  gMsrOahGroup->setTraceMsrVisitors ();
  gMsrOahGroup->setDisplayMsr ();

  gMsrOahGroup->setTraceMsrNotesDurations ();
*/

  // generate code dependant specific options
  // ------------------------------------------------------

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
      // should not occur
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
      // LPSR
      // ------------------------------------------------------

/*
      gLpsrOahGroup->setTraceLpsr ();
      gLpsrOahGroup->setTraceLpsrVisitors ();
      gLpsrOahGroup->setDisplayLpsr ();
*/

      // lpsr2lilypond
      // ------------------------------------------------------

/*
      gGlobalLpsr2lilypondOahGroup->setLilypondRunDate (); // JMI NOT OK
      gGlobalLpsr2lilypondOahGroup->setInputLineNumbers ();
*/
      break;

    case mfMultiGenerationOutputKind::kGenerationBraille:
      // BSR
      // ------------------------------------------------------

/*
      gBsrOahGroup->setTraceBsr ();
      //  gBsrOahGroup->setTraceBsrVisitors ();
*/

      // bsr2braille
      // ------------------------------------------------------

/*
      gGlobalBrailleGenerationOahGroup->
        setBrailleOutputKind (bsrBrailleOutputKind::kBrailleOutputUTF8Debug);
*/
      break;

    case mfMultiGenerationOutputKind::kGenerationMusicXML:
      // MusicXML
      // ------------------------------------------------------

/*
      gGlobalMsr2mxsrOahGroup->setTraceDivisions ();

      gGlobalMxsrOahGroup->setTraceMxsrVisitors ();
*/
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:
/*
     gGlobalMsr2mxsrOahGroup->setTraceDivisions ();
*/
     break;

    case mfMultiGenerationOutputKind::kGenerationMidi:
/*
*/
      break;
  } // switch
}

//------------------------------------------------------------------------
int main (int argc, char*  argv[])
//------------------------------------------------------------------------
{
  // setup signals catching
  // ------------------------------------------------------

#ifdef MF_CATCH_SIGNALS_IS_ENABLED
  catchSignals ();
#endif // MF_CATCH_SIGNALS_IS_ENABLED

  // the service name
  // ------------------------------------------------------

  std::string serviceName = argv [0];

  // create the global log indented output stream
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

  // initialize this generator's history
  // ------------------------------------------------------
//  initializeLilyPondIssue34GeneratorComponent (); // JMI


  // the oahHandler, set below
  // ------------------------------------------------------

  S_oahHandler handler;

  try {
    // create a generators insider OAH handler
    // ------------------------------------------------------

    const S_LilyPondIssue34InsiderHandler&
      insiderOahHandler =
        LilyPondIssue34InsiderHandler::create (
          serviceName,
          serviceName + " insider OAH handler with argc/argv",
          multiGenerationOutputKind);

    // the OAH handler to be used, a regular handler is the default
    // ------------------------------------------------------

    if (insiderOption) {
      // use the insider generators OAH handler
      handler = insiderOahHandler;
    }
    else {
      // create a regular generators OAH handler
      handler =
        LilyPondIssue34RegularHandler::create (
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

  // get the generation API kind from the options
  // ------------------------------------------------------

  msrGenerationAPIKind
    theGenerationAPIKind =
      gGlobalMsrGeneratorsOahGroup->
        getGenerationAPIKind ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "==> generationAPIKind: " <<
      msrGenerationAPIKindAsString (theGenerationAPIKind);

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create and populate the theMsrScore (pass 1)
  // ------------------------------------------------------

  // start the clock
  clock_t startClock = clock ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream ss;

    ss <<
      std::endl <<
      separator <<
      std::endl <<
      gTab <<
      gTab <<
      gLanguage->passIDKindAsString (mfPassIDKind::kMfPassID_1) <<
      ": " <<
      "Creating and populating the MSR score with the " <<
      msrGenerationAPIKindAsString (theGenerationAPIKind) <<
      std::endl <<
      separator;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // the generator
  S_LilyPondIssue34Generator
    generator =
      LilyPondIssue34Generator::create ();

  // create and populate the score
  S_msrScore
    theMsrScore =
      generator->
        createAndPopulateTheScore (
          theGenerationAPIKind);

  // register time spent
  clock_t endClock = clock ();

  gGlobalTimingItemsList.appendTimingItem (
    mfPassIDKind::kMfPassID_1,
    "Create the MSR score",
    mfTimingItemKind::kMandatory,
    startClock,
    endClock);

  // set the desired options
  // ------------------------------------------------------

  enforceSomeOptions (multiGenerationOutputKind);

  // should we generate LilyPond, Braille, MusicXML or Guido?
  // ------------------------------------------------------

  mfMusicformatsErrorKind err = mfMusicformatsErrorKind::kMusicformatsError_NONE;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::kGeneration_UNKNOWN_:
      // should not occur, unless the run is a pure help one
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond: // (pass 2 and pass 3)
      err =
        msrScore2lilypondWithHandler (
          theMsrScore,
          mfPassIDKind::kMfPassID_2,
          "Convert the MSR into an LPSR",
          mfPassIDKind::kMfPassID_3,
          gLanguage->convertTheLPSRIntoLilyPondCode (),
          gOutput,
          gLog,
          handler);
      break;

    case mfMultiGenerationOutputKind::kGenerationBraille: // (pass 2, pass 3 and pass 4)
      err =
        msrScore2brailleWithHandler (
          theMsrScore,
          mfPassIDKind::kMfPassID_2,
          "Create the first BSR from the MSR",
          mfPassIDKind::kMfPassID_3,
          "Create the finalized BSR from the first BSR",
          mfPassIDKind::kMfPassID_4,
          "Convert the BSR into Braille text",
          gOutput,
          gLog,
          handler);
      break;

    case mfMultiGenerationOutputKind::kGenerationMusicXML: // (pass 2, pass 3 and pass 4)
      err =
        msrScore2musicxmlWithHandler (
          theMsrScore,
          mfPassIDKind::kMfPassID_2,
          "Convert the MSR score into a second MSR",
          mfPassIDKind::kMfPassID_3,
          "Convert the second MSR into an MXSR",
          mfPassIDKind::kMfPassID_4,
          "Convert the MXSR into MusicXML text",
          gOutput,
          gLog,
          handler);
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:// (pass 2, pass 3 and pass 4)
      err =
        msrScore2guidoWithHandler (
          theMsrScore,
          mfPassIDKind::kMfPassID_2,
          "Convert the MSR score into a second MSR",
          mfPassIDKind::kMfPassID_3,
          "Convert the second MSR into an MXSR",
          mfPassIDKind::kMfPassID_4,
          "Convert the MXSR into Guido text",
          gOutput,
          gLog,
          handler);
      break;

    case mfMultiGenerationOutputKind::kGenerationMidi:
      break;
  } // switch

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
      " thru the " <<
      msrGenerationAPIKindAsString (
        theGenerationAPIKind) <<
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
