/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <cstring>      // memset

#ifndef WIN32
#include <signal.h>
#endif // WIN32

#include <fstream>

#include "mfBool.h"
#include "mfInitialization.h"
#include "mfMusicformatsErrors.h"
#include "mfTiming.h"

#include "mfcComponents.h"

#include "waeInterface.h"
#include "oahWae.h"

#include "oahDisplayOah.h"

#include "oahEarlyOptions.h"

#include "stringMatcherBasicTypes.h"

#include "stringMatcherInterpreterInsiderHandler.h"
#include "stringMatcherInterpreterRegularHandler.h"

#include "stringMatcherInterface.h"
#include "stringMatcherDriver.h"

#include "waeHandlers.h"


namespace MusicFormats
{

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

//_______________________________________________________________________________
mfMusicformatsErrorKind executeStringMatcher (
  const std::string& stringMatcherExpressionString,
  const std::string& stringMatcherInputString)
{
  mfMusicformatsErrorKind
    result =
      mfMusicformatsErrorKind::kMusicformatsError_NONE;

  // the driver
  stringMatcherDriver
    theDriver;

  // parse the script
  int
    parseResult =
      theDriver.parseInput_Pass1 ();

  // launch the service
  if (parseResult != 0) {
    result =
      mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
  }

  else {
    result =
      theDriver.launchstringMatcherService_Pass2 ();
  }

  return result;
}

//_______________________________________________________________________________
EXP int stringMatcher (
  int   argc,
  char* argv[])
{
  // setup signals catching
  // ------------------------------------------------------

#ifdef MF_CATCH_SIGNALS_IS_ENABLED
  catchSignals ();
#endif // MF_CATCH_SIGNALS_IS_ENABLED

  // the service name
  // ------------------------------------------------------

  std::string serviceName = argv [0];

  // create the global output and log indented streams
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (std::cout, std::cerr);

// JMI  mfMoment::testMsrMomentComparisons (std::cerr); // BLARK CLAR

// JMI testBool ();

// JMI  testIncludeOptionsFromFile ();

//   gLog <<
//     "getGlobalMusicFormatsVersionNumberAndDate (): " << getGlobalMusicFormatsVersionNumberAndDate () <<
//     std::endl;

  // initialize common things
  // ------------------------------------------------------

  initializeMusicFormats ();

  initializeWAE ();

  // register stringMatcher as current service
  // ------------------------------------------------------

  setGlobalService (mfServiceKind::kMfService_stringMatcher);

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
      serviceName << " stringMatcher()" <<
      ", insiderOption: " << insiderOption;

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // the oahHandler, set below
  // ------------------------------------------------------

  S_oahHandler handler;

  try {
    // create a stringMatcher insider OAH handler
    // ------------------------------------------------------

    const S_stringMatcherInterpreterInsiderHandler&
      insiderOahHandler =
        stringMatcherInterpreterInsiderHandler::create (
          serviceName,
          serviceName + " insider OAH handler with argc/argv"); // JMI 0.9.67
//           serviceName + " insider OAH handler with argc/argv",
//           oahHandlerUsedThruKind::kHandlerUsedThruArgcArgv);

    // the OAH handler to be used, a regular handler is the default
    // ------------------------------------------------------

    Bool insiderOption =
      gEarlyOptions.getEarlyInsiderOption ();

    if (insiderOption) {
      // use the insider stringMatcher OAH handler
      handler = insiderOahHandler;
    }
    else {
      // create a regular stringMatcher OAH handler
      handler =
        stringMatcherInterpreterRegularHandler::create (
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
  } // try

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

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
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
      ", a part of the MusicFormats library (https://github.com/jacques-menu/musicformats)" <<
      std::endl;

    gLog <<
      "Launching the filtering of ";

    if (inputSourceName == "-") {
      gLog <<
        "standard input";
    }
    else {
      gLog <<
        "\"" << inputSourceName << "\"";
    }

    gLog <<
      " with expression " <<
      "???EXPRESSION???" << // JMI 0.9.72
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
      "stringMatcher output will be written to ";
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

    gWaeHandler->waeTraceWithoutInputLocation (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // do the filtering
  // ------------------------------------------------------

  mfMusicformatsErrorKind
    err =
      mfMusicformatsErrorKind::kMusicformatsError_NONE;

  try {
    if (inputSourceName == "-") {
      // MusicXML data comes from standard input
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptions ()) {
        gLog << "Reading standard input" << std::endl;
      }
#endif // MF_TRACE_IS_ENABLED

      // the std::istringstream to read lines from theString
//       std::istringstream inputStream (theString);
      std::string        line;

      // print theString line by line
//       while (getline (inputStream, line)) {
      while (getline (std::cin, line)) {
        gOutput << line << std::endl;
      } // while

//       executeStringMatcher (line);
    }

    else {
      // MusicXML data comes from a file
#ifdef MF_TRACE_IS_ENABLED
      if (gEarlyOptions.getTraceEarlyOptions ()) {
        std::stringstream ss;

        ss <<
          "Reading file \"" <<
          inputSourceName <<
          "\"" <<
          std::endl;

        gWaeHandler->waeTraceWithoutInputLocation (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      std::ifstream
        inputStream (
          inputSourceName.c_str (),
          std::ifstream::in);

      if (! inputStream.is_open ()) {
        std::stringstream ss;

        ss <<
          gLanguage->cannotOpenOptionsFileForReading (inputSourceName);

        std::string message = ss.str ();

        gLog <<
          message <<
          std::endl;

        throw mfOahException (message);
      }

      // filter inputStream line by line
      while (true) {
        std::string currentLine;

        getline (inputStream, currentLine);

#ifdef MF_TRACE_IS_ENABLED
        if (gEarlyOptions.getTraceEarlyOptionsDetails ()) {
          std::stringstream ss;

          ss <<
            "==> currentLine: [" << currentLine << ']';

          gWaeHandler->waeTraceWithoutInputLocation (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED

//         executeStringMatcher (currentLine);
      } // while
    }
  } // try

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
      "### stringMatcher interpreter failed ###" <<
      std::endl;

    return 1;
  }

  return 0;
}

//______________________________________________________________________________
void testStringMatcher (std::ostream& os)
{
  S_stringMatcherNode
    theExpression =

      stringMatcherAnd::create (

        stringMatcherString::create (
          "xml"),

        stringMatcherNot::create (
          stringMatcherString::create (
            "lilypond")
          )
        );

  os <<
    "theExpression:" <<
    std::endl;

  ++gIndenter;
  os <<
    theExpression <<
    std::endl;
  --gIndenter;

  os <<
    "as string:" <<
    std::endl;

  ++gIndenter;
  os <<
    theExpression->asString () <<
    std::endl <<
    std::endl;
  --gIndenter;

  os <<
    "as string with full parentheses:" <<
    std::endl;

  ++gIndenter;
  os <<
    theExpression->asStringWithFullParentheses () <<
    std::endl;
  --gIndenter;

  std::string
    data1 =
      "xml lilypond";

  gLog <<
    "data1 = " << data1 <<
    std::endl;

//   executeStringMatcher (data1);
}


}
