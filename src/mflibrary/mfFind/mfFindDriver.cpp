/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

/*
int main()
{
  std::cout << "countdown:\n";
  for (int i = 10; i>0; --i) {
    std::cout << i << std::endl;
    std::this_thread::sleep_for (std::chrono::seconds(1));
  }
*/

#include "mfAssert.h"
#include "mfStringsHandling.h"
#include "mfIndentedTextOutput.h"
#include "mfSystemInterface.h"

#include "oahEarlyOptions.h"

#include "mfFindInterpreterOah.h"

#include "mfFindDriver.h"
#include "mfFindParser.h"

#include "mfFindWae.h"


using namespace std;

//______________________________________________________________________________
mfFindDriver::mfFindDriver ()
{
  // get the script source name
//   if (fScriptName == "-") {
//     // mfFind data comes from standard input
// #ifdef MF_TRACE_IS_ENABLED
//     if (gEarlyOptions.getEarlyTraceOah ()) {
//       gLog << "Reading standard input" << endl;
//     }
// #endif // MF_TRACE_IS_ENABLED
//   }
//
//   else {
//     // mfFind data comes from a file
// #ifdef MF_TRACE_IS_ENABLED
//     if (gEarlyOptions.getEarlyTraceOah ()) {
//       gLog <<
//         "Reading file \"" << fScriptName << "\"" <<
//         endl;
//     }
// #endif // MF_TRACE_IS_ENABLED
//   }
}

mfFindDriver::~mfFindDriver ()
{}

// void mfFindDriver::optionsBlocksStackPush (
//   S_mfFindOptionsBlock optionsBlock,
//   const std::string& context)
// {
//   if (fTraceOptionsBlocks) {
//     gLog <<
//       "====> Pushing [" <<
//       optionsBlock->asString () <<
//       "] onto the options blocks stack" <<
//       ", context: " << context <<
//       endl;
//   }
//
//   if (fOptionsBlocksStack.size () == 0) {
//     // first push on the stack
//     fMainOptionsBlock = optionsBlock;
//   }
//
//   fOptionsBlocksStack.push_front (
//     optionsBlock);
//
//   if (fTraceOptionsBlocks) {
//     ++gIndenter;
//     displayOptionsBlocksStack (context);
//     --gIndenter;
//   }
// }

// S_mfFindOptionsBlock mfFindDriver::optionsBlocksStackTop () const
// {
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     fOptionsBlocksStack.size (),
//     "optionsBlocksStackTop(): fOptionsBlocksStack is empty");
//
//   return fOptionsBlocksStack.front ();
// }


// void mfFindDriver::displayOptionsBlocksStack (
//   const std::string& context) const
// {
//   gLog <<
//     "OptionsBlocks stack, context: " <<
//     context <<
//     ":" ;
//
//   if (fOptionsBlocksStack.size ()) {
//     gLog << endl;
//
//     ++gIndenter;
//
//     for (S_mfFindOptionsBlock optionsBlock : fOptionsBlocksStack) {
//       gLog << optionsBlock;
//     } // for
//
//     --gIndenter;
//   }
//
//   else {
//     gLog << "empty" << endl;
//   }
// }


int mfFindDriver::parseInput_Pass1 ()
{
  // initialize scanner location
  fScannerLocation.initialize (
    &fScriptName);

  // begin scan
  scanBegin ();

  if (fScriptName.empty () || fScriptName == "-") {
    fScriptName = "stdin"; // nicer for warning and error messages
  }

  // do the parsing
  mfFind::parser theParser (*this);

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
//   if (gGlobalmfFindInterpreterOahGroup->getTraceOptionsBlocks ()) {
//     gLog <<
//       "====> fOptionsBlocksStack:" <<
//       endl;
//
//     ++gIndenter;
//
//     for (S_mfFindOptionsBlock optionsBlock : fOptionsBlocksStack) {
//       gLog <<
//         optionsBlock <<
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

mfMusicformatsErrorKind mfFindDriver::launchmfFindService_Pass2 ()
{
  mfMusicformatsErrorKind
    result =
      mfMusicformatsErrorKind::kMusicformatsError_NONE;

//   mfAssert (
//     __FILE__, __LINE__,
//     fOptionsBlocksStack.size () == 1,
//     "fOptionsBlocksStack should contain only the main options block after parsing");
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
//     fCaseChoiceStatementsStack.size () == 0,
//     "fCaseChoiceStatementsStack should be empty after parsing");
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


