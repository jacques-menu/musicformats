/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

// https://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor

#ifdef __APPLE__
  // nothing specific needed here
#endif // __APPLE__

#ifdef __gnu_linux__
  #include <string.h> // strerror_r()
#endif // __gnu_linux__

#ifdef WIN32
  // nothing specific needed here
#endif // WIN32

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
#endif // MF_TRACE_IS_ENABLED

#ifdef __gnu_linux__
  char*
    result =
      strerror_r (errno, pBuffer, kBufferSize);
#endif // MF_TRACE_IS_ENABLED

#ifdef WIN32
  int
    result =
      strerror_s (pBuffer, kBufferSize, errno);
#endif // WIN32

  // use result just to avoid a compiler warning...
  result = result + 0;

  return pBuffer;
}

//______________________________________________________________________________
EXP int mfExecuteCommand (
  const std::string& command,
  Bool               doTrace)
{
  int result = 0;

  if (doTrace) {
    gLog <<
      "Executing command:" <<
      std::endl;

    ++gIndenter;

    gLog <<
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
// //     gLog <<
// //       "#### mfExecuteCommand" <<
// //       ", failed to execute command [" <<
// //       command <<
// //       " - quitting." <<
// //       std::endl;
// //
// //     result = pclose (commandOutputStream);
// //   }
//
// #endif // MF_TRACE_IS_ENABLED

  return result;
}


}
