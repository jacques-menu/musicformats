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
#endif

#include "mfBool.h"
#include "mfMusicformatsErrors.h"
#include "mfcComponents.h"
#include "mfTiming.h"

#include "waeInterface.h"
#include "oahWae.h"

#include "oahDisplayOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2guidoInsiderHandler.h"
#include "musicxml2guidoRegularHandler.h"

#include "musicxml2guidoInterface.h"

#include "xml2gmnInterface.h"


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
#endif

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

  // apply early options if any
  // ------------------------------------------------------

  gGlobalOahEarlyOptions.applyEarlyOptionsIfPresentInArgcArgv (
    argc,
    argv);

  // has the '-insider' option been used?
  // ------------------------------------------------------

  Bool insiderOption =
    gGlobalOahEarlyOptions.getEarlyInsiderOption ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
		std::stringstream ss;

    ss <<
      serviceName << " xml2gmn()" <<
      ", insiderOption: " << insiderOption <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

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

    gGlobalCurrentServiceRunData =
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

  // check indentation
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

  // let's go ahead
  // ------------------------------------------------------

  std::string
    inputSourceName =
      gGlobalCurrentServiceRunData->getInputSourceName ();

  std::string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    std::string separator =
      "%--------------------------------------------------------------";

		std::stringstream ss;

    ss <<
      serviceName << ": " <<
      "inputSourceName = \"" << inputSourceName << "\"" <<
      ", outputFileName = \"" << outputFileName << "\"" <<
      std::endl <<
      separator <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

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

  if (gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // welcome message
  // ------------------------------------------------------

#ifdef MF_TRACING_IS_ENABLED
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

    if (inputSourceName == "-") {
      gLogStream <<
        "standard input";
    }
    else {
      gLogStream <<
        "\"" << inputSourceName << "\"";
    }

    gLogStream <<
      " to Guido music notation" <<
      std::endl;

    gLogStream <<
      "Time is " <<
      gGlobalCurrentServiceRunData->getRunDateFull () <<
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
      "Guido code will be written to ";
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

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
		std::stringstream ss;

    ss <<
      "The command line options and arguments have been analyzed" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

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
#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
        gLogStream << "Reading standard input" << std::endl;
      }
#endif

      err =
        convertMusicxmlFd2guidoWithHandler (
          stdin,
          gOutputStream,
          gLogStream,
          handler);
    }

    else {
      // MusicXML data comes from a file
#ifdef MF_TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
        gLogStream << "Reading file \"" << inputSourceName << "\"" << std::endl;
      }
#endif

      err =
        convertMusicxmlFile2guidoWithHandler (
          inputSourceName.c_str(),
          gOutputStream,
          gLogStream,
          handler);
    }
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

  if (err != mfMusicformatsErrorKind::kMusicformatsError_NONE) {
    gLogStream <<
      "### Conversion from MusicXML to Guido failed ###" <<
      std::endl;

    return 1;
  }

  return 0;
}


}
