/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#include <cstring>
#include <iomanip>      // setw()), set::precision(), ...
#include <fstream>      // ifstream, ...

#ifndef WIN32
#include <signal.h>
#endif

#include "mfServiceRunData.h"
#include "mfStringsHandling.h"
#include "mfTiming.h"

#include "wae.h"
#include "oahWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahBasicTypes.h"

#include "oahOah.h"
#include "waeOah.h"
#include "displayOah.h"

#include "oahEarlyOptions.h"

#include "msr.h"

#include "mfcLibraryComponent.h"
#include "musicxml2musicxmlComponent.h"

#include "musicxml2musicxmlInsiderHandler.h"
#include "musicxml2musicxmlRegularHandler.h"

#include "musicxml2musicxmlInterface.h"

#include "mfcLibraryComponent.h"


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

	catchSignals ();

  // fetch service name
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

  // the oahHandler, set below
  // ------------------------------------------------------

  S_oahHandler handler;

  try {
    // create an xml2xml insider OAH handler
    // ------------------------------------------------------

    S_xml2xmlInsiderHandler
      insiderOahHandler =
        xml2xmlInsiderHandler::create (
          serviceName,
          serviceName + " insider OAH handler with argc/argv");

    // the OAH handler to be used, a regular handler is the default
    // ------------------------------------------------------

    if (insiderOption) {
      // use the insider xml2xml OAH handler
      handler = insiderOahHandler;
    }
    else {
      // create a regular xml2xml OAH handler
      handler =
        xml2xmlRegularHandler::create (
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
  catch (std::exception& e) {
    mfDisplayException (e, gOutputStream);
    return (int) mfMusicformatsError::kErrorInvalidFile;
  }

  // check indentation
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

  // what if no input source name has been supplied?
  if (! inputSourceName.size ()) {
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

      oahError (s.str ());
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
    int
      outputFileNameSize =
        outputFileName.size ();

    gLogStream <<
      "This is " << serviceName << ' ' <<
      getGlobalMusicFormatsVersionNumberAndDate () <<
      endl;

    gLogStream <<
      "Launching the conversion of ";

    if (inputSourceName == "-") {
      gLogStream <<
        "standard input";
    }
    else {
      gLogStream <<
        "\"" << inputSourceName << "\"";
    }

    gLogStream <<
      " back to MusicXML" <<
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
      "MusicXML data will be written to ";
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

  // sanity check // in xml2xmlInsiderOahGroup ??? JMI
  // ------------------------------------------------------

  if (inputSourceName.size () > 0 && inputSourceName == outputFileName) { // JMI
    stringstream s;

    s <<
      "\"" << inputSourceName << "\" is both the input and output file name";

    oahError (s.str ());
  }

  // do the conversion
  // ------------------------------------------------------

  mfMusicformatsError err;

  try {
    if (inputSourceName == "-") {
      // MusicXML data comes from standard input
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
        gLogStream << "Reading standard input" << endl;
      }
#endif

      err =
        convertMusicxmlFd2musicxmlWithHandler (
          stdin,
          gOutputStream,
          gLogStream,
          handler);
    }

    else {
      // MusicXML data comes from a file
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
        gLogStream <<
          "Reading file \"" <<
          inputSourceName <<
          "\"" <<
          endl;
      }
#endif

      err =
        convertMusicxmlFile2musicxmlWithHandler (
          inputSourceName.c_str(),
          gOutputStream,
          gLogStream,
          handler);
    }
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
      "### Conversion from MusicXML back to MusicXML failed ###" <<
      endl;

    return 1;
  }

  return 0;
}
