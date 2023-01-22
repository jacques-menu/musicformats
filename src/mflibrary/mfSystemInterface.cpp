/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <errno.h>  // errno_t
#include <stdio.h>  // strerror_r()
#include <stdlib.h> // system()
// #include <string.h> // strerror_s()

#include "mfIndentedTextOutput.h"
#include "mfStringsHandling.h"
#include "mfSystemInterface.h"


namespace MusicFormats
{

//______________________________________________________________________________
/*
2022-11-24T08:40:38.8477149Z mfslScanner.ll(564,11): warning C4996: 'strerror': This function or variable may be unsafe. Consider using strerror_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details. [D:\a\musicformats\musicformats\build\libdir\libmusicformats.vcxproj]

strerror, strerror_s, strerrorlen_s
  C  Strings library  Null-terminated byte strings
Defined in header <string.h>
char* strerror( int errnum );
(1)

errno_t strerror_s( char *buf, rsize_t bufsz, errno_t errnum );
(2)	(since C11)

size_t strerrorlen_s( errno_t errnum );

1) Returns a pointer to the textual description of the system error code errnum, identical to the description that would be printed by perror().
 errnum is usually acquired from the errno variable, however the function accepts any value of type int. The contents of the string are locale-specific.
 The returned string must not be modified by the program, but may be overwritten by a subsequent call to the strerror function. strerror is not required to be thread-safe. Implementations may be returning different pointers to static read-only string literals or may be returning the same pointer over and over, pointing at a static buffer in which strerror places the string.

2) Same as (1), except that the message is copied into user-provided storage buf. No more than bufsz-1 bytes are written, the buffer is always null-terminated. If the message had to be truncated to fit the buffer and bufsz is greater than 3, then only bufsz-4 bytes are written, and the characters "..." are appended before the null terminator. In addition, the following errors are detected at runtime and call the currently installed constraint handler function:
buf is a null pointer
bufsz is zero or greater than RSIZE_MAX
 The behavior is undefined if writing to buf occurs past the end of the array, which can happen when the size of the buffer pointed to by buf is less than the number of characters in the error message which in turn is less than bufsz.

3) Computes the length of the untruncated locale-specific error message that strerror_s would write if it were called with errnum. The length does not include the null terminator.
As with all bounds-checked functions, strerror_s and strerrorlen_s are only guaranteed to be available if __STDC_LIB_EXT1__ is defined by the implementation and if the user defines __STDC_WANT_LIB_EXT1__ to the integer constant 1 before including <string.h>.


int
 strerror_r(int errnum, char *strerrbuf, size_t buflen);


*/

EXP char* mfStrErrorCString ()
{
  const int    kBufferSize = 1024;
  static char  pBuffer [kBufferSize];

  errno_t
    result =
//       strerror_s (pBuffer, kBufferSize, errno);
      strerror_r (errno, pBuffer, kBufferSize);

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

// #if (WIN64)
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
