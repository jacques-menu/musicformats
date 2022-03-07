/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <fstream>      // ofstream, ofstream::open(), ofstream::close()

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfslScanner.h"
#include "mfslParser.h"

#include "mfslInterpreterInterface.h"


using namespace std;

namespace MusicFormats
{

//_______________________________________________________________________________
EXP extern mfMusicformatsError launchMfslInterpreter (
  int   argc,
  char* argv[],
  Bool  verboseMode)
{
  mfMusicformatsError
    res =
      mfMusicformatsError::k_NoError;

//  	yywrap ();	// open a first (and only) script file

	if (yyparse ()) {
	  // the interpretation failed
	  res = mfMusicformatsError::kErrorInvalidFile;
	}

	return res;
}


}
