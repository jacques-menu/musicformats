/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

#include "waeEnableAbortToDebugErrors.h"

#include "waeOah.h"

#include "meiWae.h"



namespace MusicFormats
{

//______________________________________________________________________________
void meiWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "MEI",
    inputSourceName,
    inputLineNumber,
    message);
}

void meiError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
{
  waeError (
    "MEI",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) { // JMI
    throw meiException (message);
  }

  throw meiException (message);
}

void meiInternalError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
{
  gIndenter.resetToZero ();

  waeErrorWithoutException (
    "MEI INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  throw meiInternalException (message);
}


}
