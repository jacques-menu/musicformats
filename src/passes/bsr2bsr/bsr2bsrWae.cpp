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

#include "bsr2bsrWae.h"


namespace MusicFormats
{

//______________________________________________________________________________
bsr2bsrException::bsr2bsrException (
  std::string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_bsr2bsrException bsr2bsrException::create (
  std::string const& exceptionDescription)
{
  bsr2bsrException* o = new
    bsr2bsrException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

bsr2finalizedBsrException::bsr2finalizedBsrException (
  std::string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_bsr2finalizedBsrException bsr2finalizedBsrException::create (
  std::string const& exceptionDescription)
{
  bsr2finalizedBsrException* o = new
    bsr2finalizedBsrException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

bsr2bsrInternalException::bsr2bsrInternalException (
  std::string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_bsr2bsrInternalException bsr2bsrInternalException::create (
  std::string const& exceptionDescription)
{
  bsr2bsrInternalException* o = new
    bsr2bsrInternalException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

//______________________________________________________________________________
void bsr2bsrWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "bsr2bsr",
    inputSourceName,
    inputLineNumber,
    message);
}

void bsr2bsrInternalError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
{
  gIndenter.resetToZero ();

  waeErrorWithoutException (
    "bsr2bsr INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif

  throw bsr2bsrInternalException (message);
}


}
