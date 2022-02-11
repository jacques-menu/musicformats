/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

#include "wae.h"
#include "bsr2bsrWae.h"



using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
bsr2bsrException::bsr2bsrException (
  string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_bsr2bsrException bsr2bsrException::create (
  string const& exceptionDescription)
{
  bsr2bsrException* o = new
    bsr2bsrException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

bsr2finalizedBsrException::bsr2finalizedBsrException (
  string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_bsr2finalizedBsrException bsr2finalizedBsrException::create (
  string const& exceptionDescription)
{
  bsr2finalizedBsrException* o = new
    bsr2finalizedBsrException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

bsr2bsrInternalException::bsr2bsrInternalException (
  string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_bsr2bsrInternalException bsr2bsrInternalException::create (
  string const& exceptionDescription)
{
  bsr2bsrInternalException* o = new
    bsr2bsrInternalException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

//______________________________________________________________________________
void bsr2bsrWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message)
{
  waeWarning (
    "bsr2bsr",
    inputSourceName,
    inputLineNumber,
    message);
}

void bsr2bsrInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  waeErrorWithoutException (
    "bsr2bsr INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  gIndenter.setIndent (saveIndent);

  throw bsr2bsrInternalException (message);
}


}
