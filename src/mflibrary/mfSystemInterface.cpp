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

#include "mfIndentedTextOutput.h"
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

  const char* mode = "r+";

#if (WIN32)

  FILE*
    commandOutputStream =
      _popen (commandAsCString, mode)

  if (! commandOutputStream) {
    E_ERROR_SYSTEM ("_popen (%s, %s) failed\n", command, mode);;
  }

#else

  FILE*
    commandOutputStream =
      popen (commandAsCString, mode);

  if (! commandOutputStream) {
    gLogStream <<
      "#### mfExecuteCommand" <<
      ", failed to execute command [" <<
      command <<
      ", quitting." <<
      endl;

    result = pclose (commandOutputStream);
  }

#endif

	return result;
}


}
