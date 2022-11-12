/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

#include "enableAbortToDebugErrors.h"

#include "waeOah.h"

#include "bmmlWae.h"



namespace MusicFormats
{

//______________________________________________________________________________
void bmmlWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
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
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
{
  waeError (
    "BMML",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw bmmlException (message);
  }

  throw bmmlException (message);
}

void bmmlInternalError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
{
  gIndenter.resetToZero ();

  waeErrorWithoutException (
    "BMML INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  throw bmmlInternalException (message);
}


}
