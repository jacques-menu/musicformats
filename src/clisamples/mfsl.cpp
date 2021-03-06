/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>

#include <cstring>      // memset
#include <signal.h>

#include "mfslWae.h"

#include "mfTiming.h"

#include "wae.h"
#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "libmusicxml.h" // for mfMusicformatsError

#include "displayOah.h"

#include "oahEarlyOptions.h"

#include "mfcLibraryComponent.h"

#include "mfslInterpreterInsiderHandler.h"
#include "mfslInterpreterRegularHandler.h"

#include "mfslInterpreterInterface.h"


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
  sigaction (SIGILL, &sa, NULL);
  sigaction (SIGFPE, &sa, NULL);
}

#else
static void catchSignals ()	{}
#endif

//_______________________________________________________________________________
int main (int argc, char* argv[])
	{
  // setup signals catching
  // ------------------------------------------------------

// JMI	catchSignals ();

  // the executable name
  // ------------------------------------------------------

  string executableName = argv [0];

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

  // the oahHandler, set below
  // ------------------------------------------------------

  S_oahHandler handler;

  try {
    // create an mfsl insider OAH handler
    // ------------------------------------------------------

    S_mfslInterpreterInsiderHandler
      insiderOahHandler =
        mfslInterpreterInsiderHandler::create (
          serviceName,
          serviceName + " insider OAH handler with argc/argv");

    // the OAH handler to be used, a regular handler is the default
    // ------------------------------------------------------

    if (insiderOption) {
      // use the insider mfsl OAH handler
      handler = insiderOahHandler;
    }
    else {
      // create a regular mfsl OAH handler
      handler =
        mfslInterpreterRegularHandler::create (
          serviceName,
          serviceName + " regular OAH handler with argc/argv",
          insiderOahHandler);
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

  // check indentation
  // ------------------------------------------------------

  if (gIndenter != 0) {
    gLogStream <<
      "### " <<
      serviceName <<
      " gIndenter value after options ands arguments checking: " <<
      gIndenter.getIndentation () <<
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
    scriptSourceName =
      gGlobalServiceRunData->getInputSourceName ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    string separator =
      "%--------------------------------------------------------------";

    gLogStream <<
      serviceName << ": " <<
      "scriptSourceName = \"" << scriptSourceName << "\"" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // what if no script source name has been supplied?
  if (! scriptSourceName.size ()) {
    if (handler->getOahHandlerFoundAHelpOption ()) {
      return 0; // pure help run
    }
    else {
      stringstream s;

      s <<
        "this is not a pure help run, \"" <<
        serviceName <<
        " needs an input file name. " <<
        handler->getHandlerUsage ();

      oahWarning (s.str ());
//       oahError (s.str ()); JMI
    }
  }

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
    gLogStream <<
      "This is " << serviceName << ' ' <<
      getGlobalMusicFormatsVersionNumberAndDate () <<
      endl;

    gLogStream <<
      "Launching the interpretation of ";

    if (scriptSourceName == "-") {
      gLogStream <<
        "standard input";
    }
    else {
      gLogStream <<
        "\"" << scriptSourceName << "\"";
    }

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

  // do the interpretation
  // ------------------------------------------------------

  mfMusicformatsError
    err =
      mfMusicformatsError::k_NoError;

  try {
    err =
      launchMfslInterpreter ();
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
      "### " << executableName << " gIndenter final value: " <<
      gIndenter.getIndentation () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  // over!
  // ------------------------------------------------------

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
