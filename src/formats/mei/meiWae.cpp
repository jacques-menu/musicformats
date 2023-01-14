/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

#include "mfEnableAbortToDebugErrorsSetting.h"

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

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif

  throw meiInternalException (message);
}


}
