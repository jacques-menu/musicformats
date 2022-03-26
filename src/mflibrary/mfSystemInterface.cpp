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

#if (WIN32)

  FILE*
    commandOutputStream =
      _popen (commandAsCString, mode)

  if (! commandOutputStream) {
    E_ERROR_SYSTEM ("_popen (%s, %s) failed\n", command, mode);;
  }

#else

  system (commandAsCString); // JMI

//   FILE*
//     commandOutputStream =
//       popen (commandAsCString, mode);
//
//   if (! commandOutputStream) {
//     gLogStream <<
//       "#### mfExecuteCommand" <<
//       ", failed to execute command [" <<
//       command <<
//       ", quitting." <<
//       endl;
//
//     result = pclose (commandOutputStream);
//   }


// 	pid_t		processID;
//
// 	switch (processID = fork ())
// 		{
// 		case -1:
// 			Erreur ("'fork ()' a echoue");
// 			break;
//
// 		case 0:
// 			{
// 			// ENFANT
// 			// ------
//
// 			printf (
// 				"--> Ici l'ENFANT %d, de parent %d\n\n", getpid (), getppid () );
//
// 			varGlobale += 30000;
// 			varLocaleAMain += 60000;
//
// 			sleep (30);
// 			}
// 			break;
//
// 		default:
// 			{
// 			// PARENT
// 			// ------
//
// 			printf (
// 				"<-- Ici le PARENT %d, pid de l'enfant = %d\n\n",
// 				monIdProcess, processID );
//
// 			# define DELAI_D_ATTENTE	30
// 			sleep (DELAI_D_ATTENTE);
// 			}
// 			break;
// 		} // switch
//
//
// 	// PARENT ET ENFANT
// 	// ----------------
//
//

#endif

	return result;
}


}
