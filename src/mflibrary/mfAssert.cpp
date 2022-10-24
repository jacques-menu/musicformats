/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfExceptions.h"

#include "mfAssert.h"
#include "mfIndentedTextOutput.h"

#include "enableAbortToDebugErrors.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
void mfAssert (
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  Bool          condition,
  const string& messageIfFalse)
{
  if (! condition) {
    gIndenter.resetToZero ();

    gLogStream <<
      "#### mfAssert" <<
      ", " << sourceCodeFileName << ":" << sourceCodeLineNumber <<
      " -- " << messageIfFalse <<
      " - quitting." <<
      endl;

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

    throw mfAssertException (messageIfFalse);
  }
}


}
