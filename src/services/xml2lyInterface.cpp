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

#include "mfInitialization.h" // for languages, before early options handling

#include "mfBool.h"
#include "mfMusicformatsErrors.h"
#include "mfcComponents.h"
#include "mfTiming.h"

#include "waeInterface.h"
#include "oahWae.h"

#include "oahDisplayOah.h"

#include "oahEarlyOptions.h"

#include "musicxml2lilypondInsiderHandler.h"
#include "musicxml2lilypondRegularHandler.h"

#include "musicxml2lilypondInterface.h"

#include "xml2lyInterface.h"

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
#endif

//_______________________________________________________________________________
EXP int xml2ly (
  int   argc,
  char* argv[])
{
  // setup signals catching
  // ------------------------------------------------------

// JMI  catchSignals ();

  // the service name
  // ------------------------------------------------------

  std::string serviceName = argv [0];

  // create the global output and log indented streams
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (std::cout, std::cerr);

// JMI  msrMoment::testMsrMomentComparisons (std::cerr); // BLARK CLAR

// JMI testBool ();

// JMI  testIncludeOptionsFromFile ();

//   gLog <<
//     "getGlobalMusicFormatsVersionNumberAndDate (): " << getGlobalMusicFormatsVersionNumberAndDate () <<
//     std::endl;

  // initialize common things
  // ------------------------------------------------------
  initializeMusicFormats ();


#include "mfServices.h"

  S_mfService xml2lyService =
    mfService::create (serviceName);

//     This multi-pass converter basically performs 5 passes:
//         Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
//                  and converts it to a MusicXML tree;
//         Pass 2a: converts that MusicXML tree into
//                  a first Music Score Representation (MSR) skeleton;
//         Pass 2b: populates the first MSR skeleton from the MusicXML tree
//                  to get a full MSR;
//         Pass 3:  converts the first MSR into a second MSR to apply options
//         Pass 4:  converts the second MSR into a
//                  LilyPond Score Representation (LPSR);
//         Pass 5:  converts the LPSR to LilyPond code
//                  and writes it to standard output.

  xml2lyService->
    appendPassToService (
      mfPassDescription::create (
        mfPassIDKind::kMfPassID_1,
        gLanguage->createAnMXSRFromAMusicXMLStream ()));

//   gLog << "--------------" << std::endl;
//
//   gLog <<
//     "xml2lyService::print ()" <<
//     std::endl;
//
//   ++gIndenter;
//   gLog <<
//     xml2lyService <<
//     std::endl;
//   --gIndenter;
//
//   gLog << "--------------" << std::endl;
//
//   gLog <<
//     "xml2lyService::printServiceForAboutOption ()" <<
//     std::endl;
//
//   ++gIndenter;
//   xml2lyService-> printServiceForAboutOption (gLog);
//   --gIndenter;
//
//   gLog << "--------------" << std::endl;
//
//   gLog <<
//     "xml2lyService::fetchServicePassDescriptionsAsString ()" <<
//     std::endl;
//
//   ++gIndenter;
//   gLog <<
//     xml2lyService-> fetchServicePassDescriptionsAsString () <<
//     std::endl;
//   --gIndenter;
//
//   gLog << "--------------" << std::endl;

  // register xml2ly as current service
  setGlobalService (xml2lyService);

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
  if (gEarlyOptions.getEarlyTraceOah ()) {
		std::stringstream ss;

    ss <<
      serviceName << " xml2ly()" <<
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
    // create an xml2ly insider OAH handler
    // ------------------------------------------------------

    const S_xml2lyInsiderHandler&
      insiderOahHandler =
        xml2lyInsiderHandler::create (
          serviceName,
          serviceName + " insider OAH handler with argc/argv",
          oahHandlerUsedThruKind::kHandlerUsedThruArgcArgv);

    // the OAH handler to be used, a regular handler is the default
    // ------------------------------------------------------

    Bool insiderOption =
      gEarlyOptions.getEarlyInsiderOption ();

    if (insiderOption) {
      // use the insider xml2ly OAH handler
      handler = insiderOahHandler;
    }
    else {
      // create a regular xml2ly OAH handler
      handler =
        xml2lyRegularHandler::create (
          serviceName,
          serviceName + " regular OAH handler with argc/argv",
          insiderOahHandler);
    }

    // create the global run data
    // ------------------------------------------------------

    setGlobalServiceRunData (
      mfServiceRunData::create (
        serviceName));

    // handle the options and arguments from argc/argv
    // ------------------------------------------------------
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

  // let's go ahead
  // ------------------------------------------------------

//   testRational ();

  std::string
    inputSourceName =
      gServiceRunData->getInputSourceName ();

  std::string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTraceOah ()) {
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
      " to LilyPond" <<
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
      "LilyPond code will be written to ";
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
#endif

  // acknoledge end of command line analysis
  // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getEarlyTracePasses ()) {
		std::stringstream ss;

    ss <<
      "The command line options and arguments have been analyzed" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  // do the conversion
  // ------------------------------------------------------

  mfMusicformatsErrorKind err;

  try {
    if (inputSourceName == "-") {
      // MusicXML data comes from standard input
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getEarlyTraceOah ()) {
        gLog << "Reading standard input" << std::endl;
      }
#endif

      err =
        convertMusicxmlFd2lilypondWithHandler (
          stdin,
          gOutput,
          gLog,
          handler);
    }

    else {
      // MusicXML data comes from a file
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getEarlyTraceOah ()) {
        std::stringstream ss;

        ss <<
          "Reading file \"" <<
          inputSourceName <<
          "\"" <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif

      err =
        convertMusicxmlFile2lilypondWithHandler (
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
      "### Conversion from MusicXML to LilyPond failed ###" <<
      std::endl;

    return 1;
  }

  return 0;
}


}
