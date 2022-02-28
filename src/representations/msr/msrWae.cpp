/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStringsHandling.h"
#include "mfIndentedTextOutput.h"

#include "oahEarlyOptions.h"

#include "wae.h"
#include "msrWae.h"

#include "oahOah.h"
#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
msrException::msrException (
  string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_msrException msrException::create (
  string const& exceptionDescription)
{
  msrException* o = new
    msrException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

msrInternalException::msrInternalException (
  string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_msrInternalException msrInternalException::create (
  string const& exceptionDescription)
{
  msrInternalException* o = new
    msrInternalException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

msrUnsupportedException::msrUnsupportedException (
  string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_msrUnsupportedException msrUnsupportedException::create (
  string const& exceptionDescription)
{
  msrUnsupportedException* o = new
    msrUnsupportedException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

//______________________________________________________________________________
void msrWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message)
{
  waeWarning (
    "MSR",
    inputSourceName,
    inputLineNumber,
    message);
}

void msrError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  waeError (
    "MSR",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) {
    if (! gGlobalWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw msrException (message);
    }
    else {
      throw msrException (message);
    }
  }
}

//______________________________________________________________________________
void msrUnsupported (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  if (!
    (
      gGlobalOahEarlyOptions.getEarlyQuietOption ()
        &&
      gGlobalWaeOahGroup->getDontShowErrors ()
    )
  ) {
    int saveIndent = gIndenter.getIndent ();

    gIndenter.resetToZero ();

    if (gGlobalOahOahGroup->getDisplaySourceCodePositions ()) {
      gLogStream <<
        mfBaseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " ";
    }

    gLogStream <<
      "### MSR LIMITATION ### " <<
      inputSourceName << ":" << inputLineNumber << ": " << message <<
      endl;

    gIndenter.setIndent (saveIndent);
  }

  throw msrUnsupportedException (message);
}

//______________________________________________________________________________
void msrInternalWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message)
{
  waeInternalWarning (
    "MSR",
    inputSourceName,
    inputLineNumber,
    message);
}

void msrInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

//  waeErrorWithoutException ( // JMI
   waeErrorWithException (
    "MSR INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message,
    msrInternalException::create (message));

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  gIndenter.setIndent (saveIndent);

  throw msrInternalException (message);
}


}
