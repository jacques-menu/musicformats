/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "stringFilterInterface.h"


using namespace MusicFormats;

//_______________________________________________________________________________
int main (int argc, char* argv[])
{
  return
    stringfilter (
      argc,
      argv);
}

// #include <cstring> // memset
// #include <signal.h>
//
// #include <fstream>
//
// #include "mfPreprocessorSettings.h"
//
// #include "mfServices.h"
// #include "mfStringsHandling.h"
// #include "mfTiming.h"
//
// #include "waeInterface.h"
// #include "oahWae.h"
//
// #include "mfMusicformatsErrors.h" // for mfMusicformatsErrorKind
//
// #include "oahOah.h"
// #include "waeOah.h"
// #include "oahDisplayOah.h"
//
// #include "oahEarlyOptions.h"
//
// #include "mfLibraryComponent.h"
//
// #include "stringFilterInterpreterComponent.h"
//
// #include "stringFilterInterpreterInsiderHandler.h"
// #include "stringFilterInterpreterRegularHandler.h"
//
//
//
// #include "waeHandlers.h"
//
//
// using namespace MusicFormats;
//
// //_______________________________________________________________________________
// #ifndef WIN32
// static void _sigaction (int signal, siginfo_t *si, void *arg)
// {
//   std::cerr << "Signal #" << signal << " catched!" << std::endl;
//   exit (-2);
// }
//
// static void catchSignals ()
// {
//   struct sigaction sa;
//
//   memset (&sa, 0, sizeof(struct sigaction));
//
//   sigemptyset (&sa.sa_mask);
//
//   sa.sa_sigaction = _sigaction;
//   sa.sa_flags     = SA_SIGINFO;
//
//   sigaction (SIGSEGV, &sa, NULL);
//   sigaction (SIGILL,  &sa, NULL);
//   sigaction (SIGFPE,  &sa, NULL);
// }
// #else
// static void catchSignals () {}
// #endif // WIN32
//
// mfMusicformatsErrorKind filterStandardInput (
//   S_oahHandler handler)
// {
//   mfMusicformatsErrorKind result = mfMusicformatsErrorKind::kMusicformatsError_NONE;
//
//   return result;
// }
//
// mfMusicformatsErrorKind filterAFile (
//   std::string  inputFileName,
//   S_oahHandler handler)
// {
//   mfMusicformatsErrorKind result = mfMusicformatsErrorKind::kMusicformatsError_NONE;
//
//   return result;
// }
//
// //------------------------------------------------------------------------
// int main (int argc, char*  argv[])
// //------------------------------------------------------------------------
// {
//   // setup signals catching
//   // ------------------------------------------------------
//
//   catchSignals ();
//
// //  setEarlyTraceOah (); // JMI TEMP
//
//   // the service name
//   // ------------------------------------------------------
//
//   std::string serviceName = argv [0];
//
//   // create the global output and log indented streams
//   // ------------------------------------------------------
//
//   createTheGlobalIndentedOstreams (std::cout, std::cerr);
//
//   // apply early options if any
//   // ------------------------------------------------------
//
//   gEarlyOptions.applyEarlyOptionsIfPresentInArgcArgv (
//     argc,
//     argv);
//
//   // has the '-insider' option been used?
//   // ------------------------------------------------------
//
//   Bool insiderOption =
//     gEarlyOptions.getEarlyInsiderOption ();
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//     std::stringstream ss;
//
//     ss <<
//       serviceName << " main()" <<
//       ", insiderOption: " << insiderOption;
//
//     gWaeHandler->waeTraceWithoutInputLocation (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // fetch the generated output kind from optionsAndArguments,
//   // right now before any OAH handler is created,
//   // since the OAH handler should only use the OAH groups it needs
//   // depending on the generated output kind
//   // ------------------------------------------------------
//
//
//   // the oahHandler, set below
//   // ------------------------------------------------------
//
//   S_oahHandler handler;
//
//   try {
//     // create a stringFilterInterpreter insider OAH handler
//     // ------------------------------------------------------
//
//     const S_stringFilterInterpreterInsiderHandler&
//       insiderOahHandler =
//         stringFilterInterpreterInsiderHandler::create (
//           serviceName,
//           serviceName + " insider OAH handler with argc/argv");
//
//     // the OAH handler to be used, a regular handler is the default
//     // ------------------------------------------------------
//
//     if (insiderOption) {
//       // use the insider stringFilterInterpreter OAH handler
//       handler = insiderOahHandler;
//     }
//     else {
//       // create a regular stringFilterInterpreter OAH handler
//       handler =
//         stringFilterInterpreterRegularHandler::create (
//           serviceName,
//           serviceName + " regular OAH handler with argc/argv",
//           insiderOahHandler);
//     }
//
//     // create the global run data
//     // ------------------------------------------------------
//
//     setGlobalServiceRunData (
//       mfServiceRunData::create (
//         serviceName));
//
//     // handle the command line options and arguments
//     // ------------------------------------------------------
//
//     // handle the options and arguments from argc/argv
//     oahElementHelpOnlyKind
//       helpOnlyKind =
//         handler->
//           handleOptionsAndArgumentsFromArgcArgv (
//             argc,
//             argv);
//
//     // have help options been used?
//     switch (helpOnlyKind) {
//       case oahElementHelpOnlyKind::kElementHelpOnlyYes:
//         return 0; // quit now
//         break;
//       case oahElementHelpOnlyKind::kElementHelpOnlyNo:
//         // go ahead
//         break;
//     } // switch
//   }
//   catch (mfOahException& e) {
//     mfDisplayException (e, gOutput);
//     return (int) mfMusicformatsErrorKind::kMusicformatsErrorInvalidOption;
//   }
//   catch (std::exception& e) {
//     mfDisplayException (e, gOutput);
//     return (int) mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//
//   // has the output kind been selected?
//   // ------------------------------------------------------
//
//
//   // check indentation
//   // ------------------------------------------------------
//
//   if (gIndenter != 0) {
//     gLog <<
//       "### " <<
//       serviceName <<
//       " gIndenter value after options ands arguments checking: " <<
//       gIndenter.getIndentation () <<
//       " ###" <<
//       std::endl;
//
//     gIndenter.resetToZero ();
//   }
//
//   // display the OAH handler if needed
//   // ------------------------------------------------------
//
//   if (gGlobalDisplayOahGroup->getDisplayOahHandler ()) {
//     gLog <<
//       "The OAH handler contains:" <<
//       std::endl;
//
//     ++gIndenter;
//     handler->print (gLog);
//     --gIndenter;
//   }
//
//   if (gGlobalDisplayOahGroup->getDisplayOahHandlerSummary ()) {
//     gLog <<
//       "The summary of the OAH handler contains:" <<
//       std::endl;
//
//     ++gIndenter;
//     handler->printSummary (gLog);
//     --gIndenter;
//   }
//
//   if (gGlobalDisplayOahGroup->getDisplayOahHandlerEssentials ()) {
//     gLog <<
//       "The essentials of the OAH handler contains:" <<
//       std::endl;
//
//     ++gIndenter;
//     handler->printHandlerEssentials (
//       gLog,
//       30); // fieldWidth
//     --gIndenter;
//   }
//
//   // let's go ahead
//   // ------------------------------------------------------
//
//   std::string
//     inputSourceName =
//       gServiceRunData->getInputSourceName ();
//
//   std::string
//     outputFileName =
//       handler->
//         fetchOutputFileNameFromTheOptions ();
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getTraceEarlyOptions ()) {
//     std::string separator =
//       "%--------------------------------------------------------------";
//
//     std::stringstream ss;
//
//     ss <<
//       serviceName << ": " <<
//       "inputSourceName = \"" << inputSourceName << "\"" <<
//       ", outputFileName = \"" << outputFileName << "\"" <<
//       std::endl <<
//       separator;
//
//     gWaeHandler->waeTraceWithoutInputLocation (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // set the desired options
//   // ------------------------------------------------------
//
//
//
//   // should we generate LilyPond, Braille, MusicXML or Guido?
//   // ------------------------------------------------------
//
//
//   // has quiet mode been requested?
//   // ------------------------------------------------------
//
//   if (gEarlyOptions.getEarlyQuietOption ()) {
//     // disable all trace and display options
//     handler->
//       enforceHandlerQuietness ();
//   }
//
//   // welcome message
//   // ------------------------------------------------------
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getEarlyTracePasses ()) {
//     int
//       outputFileNameSize =
//         outputFileName.size ();
//
//     gLog <<
//       "This is " << serviceName << ' ' <<
//       getGlobalMusicFormatsVersionNumberAndDate () <<
//       std::endl;
//
//     gLog <<
//       "Launching the conversion of ";
//
//     if (inputSourceName == MSDR_STANDARD_INPUT_NAME) {
//       gLog <<
//         "standard input";
//     }
//     else {
//       gLog <<
//         "\"" << inputSourceName << "\"";
//     }
//     gLog << std::endl;
//
//     gLog <<
//       "Time is " <<
//       gServiceRunData->getRunDateFull () <<
//       std::endl;
//
//     gLog <<
//       "The command line is:" <<
//       std::endl;
//
//     ++gIndenter;
//     gLog <<
//       handler->
//         getLaunchCommandAsSupplied () <<
//       std::endl;
//     --gIndenter;
//
//     gLog <<
//       "or with options long names:" <<
//       std::endl;
//
//     ++gIndenter;
//     gLog <<
//       handler->
//         getLaunchCommandWithLongOptionsNames () <<
//       std::endl;
//     --gIndenter;
//
//     gLog <<
//       "or with options short names:" <<
//       std::endl;
//
//     ++gIndenter;
//     gLog <<
//       handler->
//         getLaunchCommandWithShortOptionsNames () <<
//       std::endl;
//     --gIndenter;
//
//     gLog <<
//       "LilyPond code will be written to "; // JMI
//     if (outputFileNameSize) {
//       gLog <<
//         outputFileName;
//     }
//     else {
//       gLog <<
//         "standard output";
//     }
//     gLog << std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // acknoledge end of command line analysis
//   // ------------------------------------------------------
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gEarlyOptions.getEarlyTracePasses ()) {
//     std::stringstream ss;
//
//     ss <<
//       "The command line options and arguments have been analyzed";
//
//     gWaeHandler->waeTraceWithoutInputLocation (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // do the job
//   // ------------------------------------------------------
//
//   mfMusicformatsErrorKind err = mfMusicformatsErrorKind::kMusicformatsError_NONE;// JMI
//
//   // generate code
//   try {
//     if (inputSourceName == MSDR_STANDARD_INPUT_NAME) {
//       // MSDL data comes from standard input
// #ifdef MF_TRACE_IS_ENABLED
//       if (gEarlyOptions.getTraceEarlyOptions ()) {
//         gLog << "Reading standard input" << std::endl;
//       }
// #endif // MF_TRACE_IS_ENABLED
//
//       err =
//         filterStandardInput (
//           handler);
//     }
//
//     else {
//       // MSDL data comes from a file
// #ifdef MF_TRACE_IS_ENABLED
//       if (gEarlyOptions.getTraceEarlyOptions ()) {
//         gLog << "Reading file \"" << inputSourceName << "\"" << std::endl;
//       }
// #endif // MF_TRACE_IS_ENABLED
//
//     err =
//       filterAFile (
//         inputSourceName,
//         handler);
//     }
//
// #ifdef MF_TRACE_IS_ENABLED
//     if (gEarlyOptions.getTraceEarlyOptions ()) {
//       if (err != mfMusicformatsErrorKind::kMusicformatsError_NONE) {
//         gLog <<
//           serviceName << ", " <<
//           ", err: \"" <<
//           mfMusicFormatsErrorAsString (err) <<
//           std::endl;
//       }
//     }
// #endif // MF_TRACE_IS_ENABLED
//   }
//   catch (mfException& e) {
//     mfDisplayException (e, gOutput);
//     return (int) mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//   catch (std::exception& e) {
//     mfDisplayException (e, gOutput);
//     return (int) mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//   }
//
//   // display the input line numbers for which messages have been issued
//   // ------------------------------------------------------
//
//   displayWarningsAndErrorsInputLineNumbers ();
//
//   // print timing information
//   // ------------------------------------------------------
//
//   if (gGlobalDisplayOahGroup->getDisplayCPUusage ()) {
//     gLog <<
//       gGlobalTimingItemsList;
//   }
//
//   // check indentation
//   // ------------------------------------------------------
//
//   if (gIndenter != 0) {
//     gLog <<
//       "### " << serviceName << " gIndenter final value: " <<
//       gIndenter.getIndentation () <<
//       " ###" <<
//       std::endl;
//
//     gIndenter.resetToZero ();
//   }
//
//   // over!
//   // ------------------------------------------------------
//
//   if (err != mfMusicformatsErrorKind::kMusicformatsError_NONE) {
//     gLog <<
//       "### The generation of " <<
//       " output failed ###" <<
//       std::endl;
//   }
//
//   switch (err) {
//     case mfMusicformatsErrorKind::kMusicformatsError_NONE:
//       return 0;
//       break;
//     case mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile:
//       return 1;
//       break;
//     case mfMusicformatsErrorKind::kMusicformatsErrorInvalidOption:
//       return 2;
//       break;
//     case mfMusicformatsErrorKind::kMusicformatsErrorUnsupported:
//       return 3;
//       break;
//   } // switch
// }
