/*

  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This file is provided as an example of the MusicXML Library use.
*/

#include <cstring>      // memset
#include <iomanip>      // std::setw()), set::precision(), ...
#include <fstream>      // std::ifstream, ...

#ifndef WIN32
#include <signal.h>
#endif

#include "mfServiceRunData.h"
#include "mfStringsHandling.h"
#include "mfTiming.h"

#include "waeInterface.h"
#include "oahWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahBasicTypes.h"

#include "oahOah.h"
#include "waeOah.h"
#include "displayOah.h"

#include "oahEarlyOptions.h"

#include "mfcLibraryComponent.h"
#include "musicxml2musicxmlComponent.h"

#include "musicxml2musicxmlInsiderHandler.h"
#include "musicxml2musicxmlRegularHandler.h"

#include "musicxml2musicxmlInterface.h"

#include "mfcLibraryComponent.h"


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
#endif

//_______________________________________________________________________________
int main (int argc, char* argv[])
{
  // setup signals catching
  // ------------------------------------------------------

  catchSignals ();

  // fetch service name
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      serviceName << " xml2xml()" <<
      ", insiderOption: " << insiderOption <<
      std::endl;
  }
#endif

  // the oahHandler, set below
  // ------------------------------------------------------

  S_oahHandler handler;

  try {
    // create an xml2xml insider OAH handler
    // ------------------------------------------------------

    const S_xml2xmlInsiderHandler&
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

  // what if no input source name has been supplied?
  if (! inputSourceName.size ()) {
    if (handler->getOahHandlerFoundAHelpOption ()) {
      return 0; // pure help run
    }
    else {
      std::stringstream s;

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
      " back to MusicXML" <<
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
      "MusicXML data will be written to ";
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

  // sanity check // in xml2xmlInsiderOahGroup ??? JMI
  // ------------------------------------------------------

  if (inputSourceName.size () > 0 && inputSourceName == outputFileName) { // JMI
    std::stringstream s;

    s <<
      "\"" << inputSourceName << "\" is both the input and output file name";

    oahError (s.str ());
  }

  // do the conversion
  // ------------------------------------------------------

  mfMusicformatsErrorKind err;

  try {
    if (inputSourceName == "-") {
      // MusicXML data comes from standard input
#ifdef TRACING_IS_ENABLED
      if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
        gLogStream << "Reading standard input" << std::endl;
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
          std::endl;
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
      "### Conversion from MusicXML back to MusicXML failed ###" <<
      std::endl;

    return 1;
  }

  return 0;
}
