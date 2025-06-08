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
#include "mfIndentedTextOutput.h"

#include "oahEarlyOptions.h"

#include "waeInterface.h"

#include "msr2msrWae.h"

#include "oahOah.h"
#include "waeOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
void msr2msrUnsupported (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       message)
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

  throw msr2msrUnsupportedException (message);
}

//______________________________________________________________________________
void msr2msrInternalWarning (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message)
{
  waeInternalWarning (
    "msr2msr",
    inputSourceName,
    inputLineNumber,
    message);
}

void msr2msrInternalError (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       message)
{
  gIndenter.resetToZero ();

  waeErrorWithoutExceptionWithInputLocation (
    "msr2msr INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif // MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED

  throw msr2msrInternalException (message);
}

//______________________________________________________________________________
void msr2msrWarning (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "msr2msr",
    inputSourceName,
    inputLineNumber,
    message);
}

void msr2msrError (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       message)
{
  waeErrorWithInputLocation (
    "msr2msr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gWaeOahGroup->getDontShowErrors ()) {
    if (! gWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw msr2msrException (message);
    }
    else {
      throw msr2msrException (message);
    }
  }
}


}
