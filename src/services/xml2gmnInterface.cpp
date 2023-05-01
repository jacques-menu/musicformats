/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <cstring>      // memset

#ifndef WIN32
#include <signal.h>
#endif // WIN32

#include "mfBool.h"
#include "mfInitialization.h"
#include "mfMusicformatsErrors.h"
#include "mfTiming.h"

#include "mfcComponents.h"

#include "waeInterface.h"
#include "oahWae.h"

#include "oahDisplayOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2guidoInsiderHandler.h"
#include "musicxml2guidoRegularHandler.h"

#include "musicxml2guidoInterface.h"

#include "xml2gmnInterface.h"

#include "waeHandlers.h"


namespace MusicFormats
{

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
EXP int xml2gmn (
  int   argc,
  char* argv[])
{
  // setup signals catching
  // ------------------------------------------------------

  catchSignals ();

  // fetch service name
  // ------------------------------------------------------

  std::string serviceName = argv [0];

  // create the global log indented output stream
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (std::cout, std::cerr);

  // initialize common things
  // ------------------------------------------------------

  initializeMusicFormats ();

  initializeWAE ();

  // register xml2gmn as current service
  // ------------------------------------------------------

  setGlobalService (mfServiceKind::kMfService_xml2gmn);

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
      serviceName << " xml2gmn()" <<
      ", insiderOption: " << insiderOption;

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // the oahHandler, set below
  // ------------------------------------------------------

  S_oahHandler handler;

  try {
    // create an xml2gmn insider OAH handler
    // ------------------------------------------------------

    const S_xml2gmnInsiderHandler&
      insiderOahHandler =
        xml2gmnInsiderHandler::create (
          serviceName,
          serviceName + " insider OAH handler with argc/argv");

    // the OAH handler to be used, a regular handler is the default
    // ------------------------------------------------------

    if (insiderOption) {
      // use the insider xml2gmn OAH handler
      handler = insiderOahHandler;
    }
    else {
      // create a regular xml2gmn OAH handler
      handler =
        xml2gmnRegularHandler::create (
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

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // what if no input source name has been supplied?
  if (! inputSourceName.size ()) {
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

      oahError (ss.str ());
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
    int
      outputFileNameSize =
        outputFileName.size ();

    gLog <<
      "This is " << serviceName << ' ' <<
      getGlobalMusicFormatsVersionNumberAndDate () <<
      std::endl;

    gLog <<
      "Launching the conversion of ";

    if (inputSourceName == "-") {
      gLog <<
        "standard input";
    }
    else {
      gLog <<
        "\"" << inputSourceName << "\"";
    }

    gLog <<
      " to Guido music notation" <<
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
      "Guido code will be written to ";
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

    gWaeHandler->waeTraceWithoutLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // sanity check // in xml2gmnInsiderOahGroup ??? JMI
  // ------------------------------------------------------

  if (inputSourceName.size () > 0 && inputSourceName == outputFileName) { // JMI
    std::stringstream ss;

    ss <<
      "\"" << inputSourceName << "\" is both the input and output file name";

    oahError (ss.str ());
  }

  // do the conversion
  // ------------------------------------------------------

  mfMusicformatsErrorKind err;

  try {
    if (inputSourceName == "-") {
      // MusicXML data comes from standard input
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getEarlyTracePasses ()) {
        gLog << "Reading standard input" << std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      err =
        convertMusicxmlFd2guidoWithHandler (
          stdin,
          gOutput,
          gLog,
          handler);
    }

    else {
      // MusicXML data comes from a file
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getEarlyTracePasses ()) {
        gLog << "Reading file \"" << inputSourceName << "\"" << std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      err =
        convertMusicxmlFile2guidoWithHandler (
          inputSourceName.c_str(),
          gOutput,
          gLog,
          handler);
    }
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
      "### Conversion from MusicXML to Guido failed ###" <<
      std::endl;

    return 1;
  }

  return 0;
}


}
