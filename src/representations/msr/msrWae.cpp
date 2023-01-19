/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStaticSettings.h"

#include "mfStringsHandling.h"

#include "oahEarlyOptions.h"

#include "waeInterface.h"

#include "msrWae.h"

#include "oahOah.h"
#include "waeOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
msrException::msrException (
  std::string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_msrException msrException::create (
  std::string const& exceptionDescription)
{
  msrException* o = new
    msrException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

msrInternalException::msrInternalException (
  std::string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_msrInternalException msrInternalException::create (
  std::string const& exceptionDescription)
{
  msrInternalException* o = new
    msrInternalException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

msrUnsupportedException::msrUnsupportedException (
  std::string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_msrUnsupportedException msrUnsupportedException::create (
  std::string const& exceptionDescription)
{
  msrUnsupportedException* o = new
    msrUnsupportedException (
      exceptionDescription);
  assert (o!=0);

  return o;
}

//______________________________________________________________________________
void msrWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "MSR",
    inputSourceName,
    inputLineNumber,
    message);
}

void msrError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
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
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
{
  if (!
    (
      gGlobalOahEarlyOptions.getEarlyQuietOption ()
        &&
      gGlobalWaeOahGroup->getDontShowErrors ()
    )
  ) {
    gIndenter.resetToZero ();

    if (gGlobalOahOahGroup->getDisplaySourceCodePositions ()) {
      gLogStream <<
        mfBaseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        ' ';
    }

    gLogStream <<
      "### MSR LIMITATION ### " <<
      inputSourceName << ":" << inputLineNumber << ": " << message <<
      std::endl;
  }

  throw msrUnsupportedException (message);
}

//______________________________________________________________________________
void msrInternalWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message)
{
  waeInternalWarning (
    "MSR",
    inputSourceName,
    inputLineNumber,
    message);
}

void msrInternalError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
{
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

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif

  throw msrInternalException (message);
}


}
