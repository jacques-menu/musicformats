/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfPreprocessorSettings.h"

#include "mfIndentedTextOutput.h"

#include "waeOah.h"

#include "bmmlWae.h"


namespace MusicFormats
{

//______________________________________________________________________________
void bmmlWarning (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "BMML",
    inputSourceName,
    inputLineNumber,
    message);
}

void bmmlError (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeErrorWithInputLocation (
    "BMML",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gWaeOahGroup->getDontShowErrors ()) { // JMI
    throw bmmlException (message);
  }

  throw bmmlException (message);
}

void bmmlInternalError (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  gIndenter.resetToZero ();

  waeErrorWithoutExceptionWithInputLocation (
    "BMML INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif // MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED

  throw bmmlInternalException (message);
}


}
