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

#include "waeInterface.h"

#include "bsr2brailleWae.h"


namespace MusicFormats
{

//______________________________________________________________________________
bsr2brailleException::bsr2brailleException (
  std::string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_bsr2brailleException bsr2brailleException::create (
  std::string const& exceptionDescription)
{
  bsr2brailleException* obj = new
    bsr2brailleException (
      exceptionDescription);
  assert (obj!=0);

  return obj;
}

bsr2brailleInternalException::bsr2brailleInternalException (
  std::string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_bsr2brailleInternalException bsr2brailleInternalException::create (
  std::string const& exceptionDescription)
{
  bsr2brailleInternalException* obj = new
    bsr2brailleInternalException (
      exceptionDescription);
  assert (obj!=0);

  return obj;
}

//______________________________________________________________________________
void bsr2brailleWarning (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "bsr2braille",
    inputSourceName,
    inputLineNumber,
    message);
}

void bsr2brailleInternalError (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  gIndenter.resetToZero ();

  waeErrorWithoutExceptionWithInputLocation (
    "bsr2braille INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif // MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED

  throw bsr2brailleInternalException (message);
}


}
