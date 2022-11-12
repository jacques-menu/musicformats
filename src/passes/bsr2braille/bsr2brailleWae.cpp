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

#include "wae.h"
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
  bsr2brailleException* o = new
    bsr2brailleException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

bsr2brailleInternalException::bsr2brailleInternalException (
  std::string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_bsr2brailleInternalException bsr2brailleInternalException::create (
  std::string const& exceptionDescription)
{
  bsr2brailleInternalException* o = new
    bsr2brailleInternalException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

//______________________________________________________________________________
void bsr2brailleWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
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
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
{
  gIndenter.resetToZero ();

  waeErrorWithoutException (
    "bsr2braille INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  throw bsr2brailleInternalException (message);
}


}
