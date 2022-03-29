/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

// #include "mfExceptions.h"
//

#include <stdio.h>
#include <stdlib.h> // system()

#include "mfIndentedTextOutput.h"
#include "mfStringsHandling.h"
#include "mfSystemInterface.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
EXP int mfExecuteCommand (
  const string& command,
  Bool          doTrace)
{
  int result = 0;

  if (doTrace) {
    gLogStream <<
      "Executing command: " << command <<
      endl;
  }

  const char* commandAsCString = command.c_str ();

  const char* mode = "r+"; // bi-directional pipe

  // avoid memory overflow by system(),
  // truncating command if necessary

      const int
        COMMAND_STRING_BUFFER_SIZE = 512;
      char*
        commandStringBuffer =
          new char (COMMAND_STRING_BUFFER_SIZE);

  mfCharStarCat (
    commandStringBuffer,
    commandAsCString,
    COMMAND_STRING_BUFFER_SIZE);

  // execute the command

  system (commandStringBuffer);

// #if (WIN32)
//
// //   system (commandAsCString); // JMI
//
//   FILE*
//     commandOutputStream =
//       _popen (commandAsCString, mode);
//
//   if (! commandOutputStream) {
//     E_ERROR_SYSTEM (
//       "_popen (%s, %s) failed\n",
//       command,
//       mode);
//   }
//
// #else
//
//   system (commandBuffer);
//
// //   FILE*
// //     commandOutputStream =
// //       popen (commandAsCString, mode);
// //
// //   if (! commandOutputStream) {
// //     gLogStream <<
// //       "#### mfExecuteCommand" <<
// //       ", failed to execute command [" <<
// //       command <<
// //       ", quitting." <<
// //       endl;
// //
// //     result = pclose (commandOutputStream);
// //   }
//
// #endif

	return result;
}


}
