/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

// https://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor

// #include <errno.h>
// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h> // system()

#ifdef __APPLE__
#endif

#ifdef __gnu_linux__
#endif

#ifdef WIN32
// #include <string.h> // strerror_s()
#endif

#include "mfIndentedTextOutput.h"
#include "mfStringsHandling.h"
#include "mfSystemInterface.h"


namespace MusicFormats
{

//______________________________________________________________________________
EXP char* mfStrErrorCString ()
{
  const int    kBufferSize = 1024;
  static char  pBuffer [kBufferSize];

  // OS-specific call
#ifdef __APPLE__
  errno_t
    result =
      strerror_r (errno, pBuffer, kBufferSize);
#endif

#ifdef __gnu_linux__
  char*
    result =
      strerror_r (errno, pBuffer, kBufferSize);
#endif

#ifdef WIN32
  int
    result =
      strerror_s (pBuffer, kBufferSize, errno);
#endif

  // use result just to avoid a compiler warning...
  result = result;

  return pBuffer;
}

//______________________________________________________________________________
EXP int mfExecuteCommand (
  const std::string& command,
  Bool               doTrace)
{
  int result = 0;

  if (doTrace) {
    gLogStream <<
      "Executing command:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      command <<
        std::endl;

    --gIndenter;
  }

  const char* commandAsCString = command.c_str ();

//   const char* mode = "r+"; // bi-directional pipe

  // avoid memory overflow by system(),
  // truncating command if necessary

  const int
    COMMAND_STRING_BUFFER_SIZE = 1024;
  char*
    commandStringBuffer =
      new char [COMMAND_STRING_BUFFER_SIZE];

  mfCharStarCat (
    commandStringBuffer,
    commandAsCString,
    COMMAND_STRING_BUFFER_SIZE);

  // execute the command

  result =
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
// //       " - quitting." <<
// //       std::endl;
// //
// //     result = pclose (commandOutputStream);
// //   }
//
// #endif

  return result;
}


}
