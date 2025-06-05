/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfPreprocessorSettings.h"

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
  msrException* obj = new
    msrException (
      exceptionDescription);
  assert (obj!=0);

  return obj;
}

msrInternalException::msrInternalException (
  std::string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_msrInternalException msrInternalException::create (
  std::string const& exceptionDescription)
{
  msrInternalException* obj = new
    msrInternalException (
      exceptionDescription);
  assert (obj!=0);

  return obj;
}

msrUnsupportedException::msrUnsupportedException (
  std::string const& exceptionDescription) throw ()
  : mfException (exceptionDescription)
{}

S_msrUnsupportedException msrUnsupportedException::create (
  std::string const& exceptionDescription)
{
  msrUnsupportedException* obj = new
    msrUnsupportedException (
      exceptionDescription);
  assert (obj!=0);

  return obj;
}

//______________________________________________________________________________
void msrWarningWithoutInputLocation (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "MSR",
    inputSourceName,
    inputLineNumber,
    message);
}

void msrWarning (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  waeWarning (
    "MSR",
    inputSourceName,
    inputLineNumber,
    message);
//     measureNumber,
//     scoreMeasuresNumber);
}

//______________________________________________________________________________
void msrError (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeErrorWithInputLocation (
    "MSR",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gWaeOahGroup->getDontShowErrors ()) {
    if (! gWaeOahGroup->getDontQuitOnErrors ()) { // JMI
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
  const mfInputLineNumber& inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  if (!
    (
      gEarlyOptions.getEarlyQuietOption ()
        &&
      gWaeOahGroup->getDontShowErrors ()
    )
  ) {
    gIndenter.resetToZero ();

    if (gOahOahGroup->getDisplaySourceCodePositions ()) {
      gLog <<
        mfBaseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        ' ';
    }

    gLog <<
      "### MSR LIMITATION ### " <<
      inputSourceName << ":" << inputLineNumber << ": " << message <<
      std::endl;
  }

  throw msrUnsupportedException (message);
}

//______________________________________________________________________________
void msrInternalWarning (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message)
{
  waeInternalWarning (
    "MSR",
    inputSourceName,
    inputLineNumber,
    message);
}

//______________________________________________________________________________
void msrInternalError (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  gIndenter.resetToZero ();

//  waeErrorWithoutException ( // JMI
   waeErrorWithExceptionWithInputLocation (
    "MSR INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message,
    msrInternalException::create (message));

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif // MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED

  throw msrInternalException (message);
}


}
