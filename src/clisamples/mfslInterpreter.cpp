/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

// #include "Langues.h"
//
// #include "SupportCPlusPlus.h"
//
// #include "SupportFlexBison.h"
//
// #include "LanguesSyntaxe.h"
//
// #include "DecodageDOptionsSyntaxe.h"
//
//
// #include <cstdlib>

#include <iostream>

#ifndef WIN32
#include <signal.h>
#endif

#include "mfslWae.h"

#include "mfBool.h"
#include "mfIndentedTextOutput.h"

#include "mfslInterpreterInterface.h"


using namespace std;

using namespace MusicFormats;

//_______________________________________________________________________________
#ifndef WIN32

static void _sigaction (int signal, siginfo_t *si, void *arg)
{
  cerr << "Signal #" << signal << " catched!" << endl;
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
static void catchSignals ()	{}
#endif

//_______________________________________________________________________________
int main (int argc, char* argv[])
	{
  // setup signals catching
  // ------------------------------------------------------

// JMI	catchSignals ();

  // the executable name
  // ------------------------------------------------------

  string executableName = argv [0];

  // create the global output and log indented streams
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (cout, cerr);

	// decipher the options and arguments
  // ------------------------------------------------------

  string scriptFileName;

	switch (argc)
		{
		case 2 :
			{
        scriptFileName = argv [1];
			}
			break;

		default:
		  {
        string message =
          "Input file name or '-' for standard input expected";

        gLogStream <<
          message <<
          endl;

        throw mfslException (message);
			}
		}	//	switch

  // welcome message
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    int
      outputFileNameSize =
        outputFileName.size ();

    gLogStream <<
      "This is " << executableName << ' ' <<
      getGlobalMusicFormatsVersionNumberAndDate () <<
      endl;

    gLogStream <<
      "Launching the interpretation of ";

//     if (inputSourceName == "-") {
//       gLogStream <<
//         "standard input";
//     }
//     else {
//       gLogStream <<
//         "\"" << inputSourceName << "\"";
//     }
      scriptFileName <<
      endl;

    gLogStream <<
      "Time is " <<
      gGlobalServiceRunData->getRunDateFull () <<
      endl;
  }
#endif

  // acknoledge end of command line analysis
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracePasses ()) {
    gLogStream <<
      "The command line options and arguments have been analyzed" <<
      endl;
  }
#endif

  // do the interpretation
  // ------------------------------------------------------

  mfMusicformatsError
    err =
		  launchMfslInterpreter (
        argc,
        argv,
        true // gModeVerbeux
  		);

//   // display the input line numbers for which messages have been issued
//   // ------------------------------------------------------
//
//   displayWarningsAndErrorsInputLineNumbers ();
//
//   // print timing information
//   // ------------------------------------------------------
//
//   if (gGlobalDisplayOahGroup->getDisplayCPUusage ()) {
//     gLogStream <<
//       mfTimingItemsList::gGlobalTimingItemsList;
//   }

  // check indentation
  // ------------------------------------------------------

  if (gIndenter != 0) {
    gLogStream <<
      "### " << executableName << " gIndenter final value: " <<
      gIndenter.getIndent () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  // over!
  // ------------------------------------------------------

//   if (err != mfMusicformatsError::k_NoError) {
  if (err != mfMusicformatsError::k_NoError) {
    gLogStream <<
      "### Conversion from MusicXML to LilyPond failed ###" <<
      endl;

    return 1;
  }

  return 0;
}
