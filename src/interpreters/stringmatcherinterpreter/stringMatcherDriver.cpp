/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include "mfAssert.h"
// #include "mfStringsHandling.h"
// #include "mfIndentedTextOutput.h"
// #include "mfSystemInterface.h"
//
// #include "oahEarlyOptions.h"
//
// #include "stringMatcherInterpreterOah.h"

#include "stringMatcherDriver.h"
// #include "stringMatcherParser.h"

// #include "stringMatcherWae.h"


using namespace std;

/*
The following routines are available for setting up input buffers for scanning in-memory strings instead of files. All of them create a new input buffer for scanning the string, and return a corresponding YY_BUFFER_STATE handle (which you should delete with yy_delete_buffer() when done with it). They also switch to the new buffer using yy_switch_to_buffer(), so the next call to yylex() will start scanning the string.

Function: YY_BUFFER_STATE yy_scan_string ( const char *str )
scans a NUL-terminated string.

Function: YY_BUFFER_STATE yy_scan_bytes ( const char *bytes, int len )
scans len bytes (including possibly NULs) starting at location bytes.

Note that both of these functions create and scan a copy of the string or bytes. (This may be desirable, since yylex() modifies the contents of the buffer it is scanning.) You can avoid the copy by using:

Function: YY_BUFFER_STATE yy_scan_buffer (char *base, yy_size_t size)
which scans in place the buffer starting at base, consisting of size bytes, the last two bytes of which must be YY_END_OF_BUFFER_CHAR (ASCII NUL). These last two bytes are not scanned; thus, scanning consists of base[0] through base[size-2], inclusive.

If you fail to set up base in this manner (i.e., forget the final two YY_END_OF_BUFFER_CHAR bytes), then yy_scan_buffer() returns a NULL pointer instead of creating a new input buffer.

Data type: yy_size_t
is an integral type to which you can cast an integer expression reflecting the size of the buffer.
*/

//______________________________________________________________________________
stringMatcherDriver::stringMatcherDriver ()
{
  // get the script source name
//   if (fScriptName == "-") {
//     // stringMatcher data comes from standard input
// #ifdef MF_TRACE_IS_ENABLED
//     if (gEarlyOptions.getEarlyTraceOah ()) {
//       gLog << "Reading standard input" << endl;
//     }
// #endif // MF_TRACE_IS_ENABLED
//   }
//
//   else {
//     // stringMatcher data comes from a file
// #ifdef MF_TRACE_IS_ENABLED
//     if (gEarlyOptions.getEarlyTraceOah ()) {
//       gLog <<
//         "Reading file \"" << fScriptName << "\"" <<
//         endl;
//     }
// #endif // MF_TRACE_IS_ENABLED
//   }
}

stringMatcherDriver::~stringMatcherDriver ()
{}

// void stringMatcherDriver::expressionsStackPush (
//   const S_stringMatcherExpression& expression,
//   const std::string&        context)
// {
//   if (fTraceExpressions) {
//     gLog <<
//       "====> Pushing [" <<
//       expression->asString () <<
//       "] onto the options blocks stack" <<
//       ", context: " << context <<
//       endl;
//   }
//
//   if (fExpressionsStack.size () == 0) {
//     // first push on the stack
//     fMainExpression = expression;
//   }
//
//   fExpressionsStack.push (
//     expression);
//
//   if (fTraceExpressions) {
//     ++gIndenter;
//     displayExpressionsStack (context);
//     --gIndenter;
//   }
// }

// S_stringMatcherExpression stringMatcherDriver::expressionsStackTop () const
// {
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     fExpressionsStack.size (),
//     "expressionsStackTop(): fExpressionsStack is empty");
//
//   return fExpressionsStack.front ();
// }


// void stringMatcherDriver::displayExpressionsStack (
//   const std::string& context) const
// {
//   gLog <<
//     "Expressions stack, context: " <<
//     context <<
//     ":" ;
//
//   if (fExpressionsStack.size ()) {
//     gLog << endl;
//
//     ++gIndenter;
//
//     for (S_stringMatcherExpression expression : fExpressionsStack) {
//       gLog << expression;
//     } // for
//
//     --gIndenter;
//   }
//
//   else {
//     gLog << "empty" << endl;
//   }
// }


int stringMatcherDriver::parseInput_Pass1 ()
{
  // initialize scanner location
  fScannerLocation.initialize (
    &fScriptName);

  // begin scan
  scanBegin ();

  if (fScriptName.size () == 0 || fScriptName == "-") {
    fScriptName = "stdin"; // nicer for warning and error messages
  }

  // do the parsing
  smi::parser theParser (*this);

  theParser.set_debug_level (
    fTraceParsing);

  int parseResult = theParser ();

  // end scan
  scanEnd ();

  // print the basic results
  if (fTraceParsing) {
    gLog <<
      "--> parseResult:   " << parseResult <<
      endl;
  }

//   // print the options blocks stack if relevant
//   if (gGlobalstringMatcherInterpreterOahGroup->getTraceExpressions ()) {
//     gLog <<
//       "====> fExpressionsStack:" <<
//       endl;
//
//     ++gIndenter;
//
//     for (S_stringMatcherExpression expression : fExpressionsStack) {
//       gLog <<
//         expression <<
//         endl;
//     } // for
//
//     --gIndenter;
//
//     gLog << std::endl;
//   }

//   // do the final semantics check
//   finalSemanticsCheck ();

  return parseResult;
}

// //_______________________________________________________________________________
// void stringMatcherDriver::expressionsStackPush (
//   const S_stringMatcherExpression& expression,
//   const std::string&        context)
// {
//   if (fTraceExpressions) {
//     gLog <<
//       "====> Pushing [" <<
//       expression->asString () <<
//       "] onto the options blocks stack" <<
//       ", context: " << context <<
//       endl;
//   }
//
//   if (fExpressionsStack.size () == 0) {
//     // first push on the stack
//     fMainExpression = expression;
//   }
//
//   fExpressionsStack.push (
//     expression);
//
//   if (fTraceExpressions) {
//     ++gIndenter;
//     displayExpressionsStack (context);
//     --gIndenter;
//   }
// }
//
// S_stringMatcherExpression stringMatcherDriver::expressionsStackTop ()
// {
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     fExpressionsStack.size (),
//     "expressionsStackTop(): fExpressionsStack is empty");
//
//   return fExpressionsStack.top ();
// }
//
// void stringMatcherDriver::expressionsStackPop (
//   const std::string& context)
// {
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     fExpressionsStack.size () != 0,
//     "expressionsStackPop(): fExpressionsStack is empty");
//
//   if (fTraceExpressions) {
//     gLog <<
//       "====> Popping [" <<
//       fExpressionsStack.top ()->asString () <<
//       "] from the expressions stack" <<
//       endl;
//   }
//
//   fExpressionsStack.pop ();
//
//   if (fTraceExpressions) {
//     displayExpressionsStack (
//       "after popping an expression");
//   }
// }
//
// void stringMatcherDriver::displayExpressionsStack (
//   const std::string& context) const
// {
//   gLog <<
//     "Expressions stack, context: " <<
//     context <<
//     ":" ;
//
//   if (fExpressionsStack.size ()) {
//     gLog << endl;
//
//     ++gIndenter;
//
// //     for (S_stringMatcherExpression expression : fExpressionsStack) {
// //       gLog << expression;
// //     } // for
//     fExpressionsStack.print (gLog);
//
//     --gIndenter;
//   }
//
//   else {
//     gLog << "empty" << endl;
//   }
// }

//_______________________________________________________________________________
mfMusicformatsErrorKind stringMatcherDriver::launchstringMatcherService_Pass2 ()
{
  mfMusicformatsErrorKind
    result =
      mfMusicformatsErrorKind::kMusicformatsError_NONE;

//   mfAssert (
//     __FILE__, __LINE__,
//     fExpressionsStack.size () == 1,
//     "fExpressionsStack should contain only the main options block after parsing");
//
//   if (fDisplayServiceAndInput) {
//     gLog <<
//       "====> Launching " <<
//       fService <<
//       " with the argument and option gathered from " <<
//       fScriptName <<
//       endl;
//   }
//
//   // sanity checks
//   mfAssert (
//     __FILE__, __LINE__,
//     fExpressionsStack.size () == 0,
//     "fExpressionsStack should be empty after parsing");
//
//   // populate the commands list with the options gathered in the script
//   populateTheCommandsList ();
//
//   // display the commands list
//   if (fDisplayServiceAndInput) {
//     gLog <<
//       "====> The " <<
//       mfSingularOrPluralWithoutNumber (
//         fCommandsList.size (),
//         "command", "commands") <<
//       " to be executed " <<
//       mfSingularOrPluralWithoutNumber (
//         fCommandsList.size (),
//         "is", "are") <<
//       ":" <<
//       endl;
//
//     ++gIndenter;
//
//     for (std::string command : fCommandsList) {
//       gLog <<
//         command <<
//         endl;
//     } // for
//
//     --gIndenter;
//   }
//
//   // execute the commands in the list
//   if (fNoLaunch) {
//     gLog <<
//       "====> The " <<
//       mfSingularOrPluralWithoutNumber (
//         fCommandsList.size (),
//         "command", "commands") <<
//       " above " <<
//       mfSingularOrPluralWithoutNumber (
//         fCommandsList.size (),
//         "is", "are") <<
//       " *NOT* executed" <<
//       endl;
//   }
//
//   else {
//     for (std::string command : fCommandsList) {
//       if (fDisplayServiceAndInput) {
//         gLog <<
//           "====> Running the service with command: [" << command << ']' <<
//           endl;
//       }
//
//       int
//         commandExecutionResult =
//           mfExecuteCommand (
//             command,
//             fDisplayServiceAndInput);
//
//       if (fDisplayServiceAndInput) {
//         gLog <<
//           "====> The execution result is: " <<
//           commandExecutionResult <<
//           endl;
//       }
//
//       if (commandExecutionResult) {
//         result =
//           mfMusicformatsErrorKind::kMusicformatsErrorInvalidFile;
//       }
//
//       // sleep for some milliseconds
//           this_thread::sleep_for (chrono::milliseconds (100));
//     } // for
//   }

  return result;
}


