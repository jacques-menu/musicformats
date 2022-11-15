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

#include "waeEnableAbortToDebugErrors.h"

#include "oahEarlyOptions.h"

#include "wae.h"
#include "msr2msrWae.h"

#include "oahOah.h"
#include "waeOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
void msr2msrUnsupported (
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

  throw msr2msrUnsupportedException (message);
}

//______________________________________________________________________________
void msr2msrInternalWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
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
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
{
  gIndenter.resetToZero ();

  waeErrorWithoutException (
    "MSR INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  throw msr2msrInternalException (message);
}

//______________________________________________________________________________
void msr2msrWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
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
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
{
  waeError (
    "msr2msr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) {
    if (! gGlobalWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw msr2msrException (message);
    }
    else {
      throw msr2msrException (message);
    }
  }
}


}
