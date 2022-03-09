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

#include <fstream>

#include "libmusicxml.h" // for mfMusicformatsError

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"
#include "mfStringsHandling.h"
#include "mfTiming.h"

#include "LilyPondIssue34Generator.h"

#include "oahOah.h"
#include "oahWae.h"
#include "displayOah.h"

#include "wae.h"

#include "msrGenerationOah.h"

#include "oahEarlyOptions.h"

#include "LilyPondIssue34Component.h"

#include "LilyPondIssue34InsiderHandler.h"
#include "LilyPondIssue34RegularHandler.h"

#include "msr2lilypondInterface.h"
#include "msr2brailleInterface.h"
#include "msr2musicxmlInterface.h"
#include "msr2guidoInterface.h"


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


//------------------------------------------------------------------------
void enforceSomeOptions (
  mfMultiGenerationOutputKind multiGenerationOutputKind)
//------------------------------------------------------------------------
{
  /*
    This is a way to enforce options 'permanently'
    independently of the options chosen by the user
  */

  // trace
  // ------------------------------------------------------

/*
  gGlobalTracingOahGroup->setTraceScores ();
  gGlobalTracingOahGroup->setTracePartGroups ();
  gGlobalTracingOahGroup->setTraceParts ();
  gGlobalTracingOahGroup->setTraceStaves ();
  gGlobalTracingOahGroup->setTraceVoices ();
  gGlobalTracingOahGroup->setTraceSegments ();
  gGlobalTracingOahGroup->setTraceMeasures ();
  gGlobalTracingOahGroup->setTraceNotes ();
*/

  // MSR
  // ------------------------------------------------------

/*
  gGlobalMsrOahGroup->setTraceMsr ();
  gGlobalMsrOahGroup->setTraceMsrVisitors ();
  gGlobalMsrOahGroup->setDisplayMsr ();

  gGlobalMsrOahGroup->setTraceMsrDurations ();
*/

  // generate code dependant specific options
  // ------------------------------------------------------

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::k_NoGeneration:
      // should not occur
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
      // LPSR
      // ------------------------------------------------------

/*
      gGlobalLpsrOahGroup->setTraceLpsr ();
      gGlobalLpsrOahGroup->setTraceLpsrVisitors ();
      gGlobalLpsrOahGroup->setDisplayLpsr ();
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
      gGlobalBsrOahGroup->setTraceBsr ();
      //  gGlobalBsrOahGroup->setTraceBsrVisitors ();
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

	catchSignals ();

  // the service name
  // ------------------------------------------------------

  string serviceName = argv [0];

  // create the global log indented output stream
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

  // initialize this generator's history
  // ------------------------------------------------------
//  initializeLilyPondIssue34GeneratorComponent (); // JMI


  // the oahHandler, set below
  // ------------------------------------------------------

  S_oahHandler handler;

  try {
    // create a generators insider OAH handler
    // ------------------------------------------------------

    S_LilyPondIssue34InsiderHandler
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

  // get the generation API kind from the options
  // ------------------------------------------------------

  msrGenerationAPIKind
    theGenerationAPIKind =
      gGlobalMsrGeneratorsOahGroup->
        getGenerationAPIKind ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "==> generationAPIKind: " <<
      msrGenerationAPIKindAsString (theGenerationAPIKind) <<
      endl;
  }
#endif

  // create and populate the theMsrScore
  // ------------------------------------------------------

  // start the clock
  clock_t startClock = clock ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    string separator =
      "%--------------------------------------------------------------";
    gLogStream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Pass 1: Creating the MSR score with the " <<
      msrGenerationAPIKindAsString (theGenerationAPIKind) <<
      endl <<
      separator <<
      endl;
  }
#endif

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

  mfTimingItemsList::gGlobalTimingItemsList.appendTimingItem (
    "Pass 1",
    "Create the MSR score",
    mfTimingItem::kMandatory,
    startClock,
    endClock);

  // set the desired options
  // ------------------------------------------------------

  enforceSomeOptions (multiGenerationOutputKind);

  // should we generate LilyPond, Braille, MusicXML or Guido?
  // ------------------------------------------------------

  mfMusicformatsError err = mfMusicformatsError::k_NoError;

  switch (multiGenerationOutputKind) {
    case mfMultiGenerationOutputKind::k_NoGeneration:
      // should not occur, unless the run is a pure help one
      break;

    case mfMultiGenerationOutputKind::kGenerationLilypond:
      err =
        msrScore2lilypondWithHandler (
          theMsrScore,
          "Pass 2",
          "Convert the MSR into an LPSR",
          "Pass 3",
          "Convert the LPSR into LilyPond code",
          gOutputStream,
          gLogStream,
          handler);
      break;

    case mfMultiGenerationOutputKind::kGenerationBraille:
      err =
        msrScore2brailleWithHandler (
          theMsrScore,
          "Pass 2a",
          "Create the first BSR from the MSR",
          "Pass 2b",
          "Create the finalized BSR from the first BSR",
          "Pass 3",
          "Convert the BSR into Braille text",
          gOutputStream,
          gLogStream,
          handler);
      break;

    case mfMultiGenerationOutputKind::kGenerationMusicXML:
      err =
        msrScore2musicxmlWithHandler (
          theMsrScore,
          "Pass 2",
          "Convert the MSR score into a second MSR",
          "Pass 3",
          "Convert the second MSR into an MXSR",
          "Pass 4",
          "Convert the MXSR into MusicXML text",
          gOutputStream,
          gLogStream,
          handler);
      break;

    case mfMultiGenerationOutputKind::kGenerationGuido:
      err =
        msrScore2guidoWithHandler (
          theMsrScore,
          "Pass 2",
          "Convert the MSR score into a second MSR",
          "Pass 3",
          "Convert the second MSR into an MXSR",
          "Pass 4",
          "Convert the MXSR into Guido text",
          gOutputStream,
          gLogStream,
          handler);
      break;

    case mfMultiGenerationOutputKind::kGenerationMidi:
      break;
  } // switch

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
      " thru the " <<
      msrGenerationAPIKindAsString (
        theGenerationAPIKind) <<
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
