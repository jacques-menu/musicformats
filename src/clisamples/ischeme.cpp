/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>

#include <cstring>      // memset
#include <signal.h>

#include "ischemeWae.h"

#include "mfTiming.h"

#include "waeInterface.h"
#include "oahWae.h"

#include "mfStaticSettings.h"

#include "libmusicxml.h" // for mfMusicformatsErrorKind

#include "oahDisplayOah.h"

#include "oahEarlyOptions.h"

#include "mfLibraryComponent.h"

#include "ischemeInterpreterInsiderHandler.h"
#include "ischemeInterpreterRegularHandler.h"

#include "ischemeInterpreterInterface.h"

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
  sigaction (SIGILL, &sa, NULL);
  sigaction (SIGFPE, &sa, NULL);
}

#else
static void catchSignals ()  {}
#endif // WIN32

//_______________________________________________________________________________
int main (int argc, char* argv[])
{
  // setup signals catching
  // ------------------------------------------------------

// JMI  catchSignals ();

  // the executable name
  // ------------------------------------------------------

  std::string executableName = argv [0];

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
      ", insiderOption: " << insiderOption <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // the oahHandler, set below
  // ------------------------------------------------------

  S_oahHandler handler;

  try {
    // create an ischeme insider OAH handler
    // ------------------------------------------------------

    S_ischemeInterpreterInsiderHandler
      insiderOahHandler =
        ischemeInterpreterInsiderHandler::create (
          serviceName,
          serviceName + " insider OAH handler with argc/argv");

    // the OAH handler to be used, a regular handler is the default
    // ------------------------------------------------------

    if (insiderOption) {
      // use the insider ischeme OAH handler
      handler = insiderOahHandler;
    }
    else {
      // create a regular ischeme OAH handler
      handler =
        ischemeInterpreterRegularHandler::create (
          serviceName,
          serviceName + " regular OAH handler with argc/argv",
          insiderOahHandler);
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
  }
  catch (mfOahException& e) {
    mfDisplayException (e, gOutput);
    return (int) mfMusicformatsErrorKind::kMusicformatsErrorInvalidOption;
  }
  catch (std::exception& e) {
    mfDisplayException (e, gOutput);
    return (int) mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

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
    scriptSourceName =
      gServiceRunData->getInputSourceName ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::string separator =
      "%--------------------------------------------------------------";

    std::stringstream ss;

    ss <<
      serviceName << ": " <<
      "scriptSourceName = \"" << scriptSourceName << "\"" <<
      std::endl <<
      separator <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // what if no script source name has been supplied?
  if (! scriptSourceName.size ()) {
    if (handler->getOahHandlerFoundAHelpOption ()) {
      return 0; // pure help run
    }
    else {
      std::stringstream ss;

      ss <<
        "this is not a pure help run, \"" <<
        serviceName <<
        " needs an input file name. " <<
        handler->getHandlerUsage ();

      oahWarning (ss.str ());
//       oahError (ss.str ()); JMI
    }
  }

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
    std::stringstream ss;

    ss <<
      "This is " << serviceName << ' ' <<
      getGlobalMusicFormatsVersionNumberAndDate () <<
      std::endl;

    gLog <<
      "Launching the interpretation of ";

    if (scriptSourceName == "-") {
      gLog <<
        "standard input";
    }
    else {
      gLog <<
        "\"" << scriptSourceName << "\"";
    }

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
  }
#endif // MF_TRACE_IS_ENABLED

  // acknoledge end of command line analysis
  // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
    std::stringstream ss;

    ss <<
      "The command line options and arguments have been analyzed" <<
      std::endl;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // do the interpretation
  // ------------------------------------------------------

  mfMusicformatsErrorKind
    err =
      mfMusicformatsErrorKind::kMusicformatsError_NONE;

  try {
    err =
      launchIschemeInterpreter ();
  }
  catch (mfException& e) {
    mfDisplayException (e, gOutput);
    return (int) mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }
  catch (std::exception& e) {
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
      "### " << executableName << " gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      std::endl;

    gIndenter.resetToZero ();
  }

  // over!
  // ------------------------------------------------------

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
