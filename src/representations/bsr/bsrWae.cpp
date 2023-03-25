/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStaticSettings.h"

#include "mfIndentedTextOutput.h"

#include "waeInterface.h"

#include "bsrWae.h"


namespace MusicFormats
{

//______________________________________________________________________________
void bsrWarning (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "BSR",
    inputSourceName,
    inputLineNumber,
    message);
}

void bsrInternalError (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  gIndenter.resetToZero ();

  waeErrorWithoutException (
    "BSR INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif // MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED

  throw bsrInternalException (message);
}


}
