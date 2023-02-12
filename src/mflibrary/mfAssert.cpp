/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStaticSettings.h"

#include "mfExceptions.h"

#include "mfAssert.h"
#include "mfIndentedTextOutput.h"


namespace MusicFormats
{

//______________________________________________________________________________
void mfAssert (
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  Bool               condition,
  const std::string& messageIfFalse)
{
  if (! condition) {
    gIndenter.resetToZero ();

    gLog <<
      "#### mfAssert" <<
      ", " << sourceCodeFileName << ":" << sourceCodeLineNumber <<
      " -- " << messageIfFalse <<
      " - quitting." <<
      std::endl;

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif

    throw mfAssertException (messageIfFalse);
  }
}

void mfAssertWithInputLocalisation (
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  Bool               condition,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& messageIfFalse)
{
  if (! condition) {
    gIndenter.resetToZero ();

    gLog <<
      "#### mfAssert" <<
      ", " << inputSourceName << ":" << inputLineNumber <<
      ", " << sourceCodeFileName << ":" << sourceCodeLineNumber <<
      " -- " << messageIfFalse <<
      " - quitting." <<
      std::endl;

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif

    throw mfAssertException (messageIfFalse);
  }
}


}
