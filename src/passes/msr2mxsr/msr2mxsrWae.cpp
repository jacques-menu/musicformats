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

#include "msr2mxsrWae.h"

#include "oahOah.h"
#include "waeOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
void msr2mxsrUnsupported (
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
      "### msr2mxsr LIMITATION ### " <<
      inputSourceName << ":" << inputLineNumber << ": " << message <<
      std::endl;
  }

  throw msr2mxsrUnsupportedException (message);
}

//______________________________________________________________________________
void msr2mxsrInternalWarning (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message)
{
  waeInternalWarning (
    "msr2mxsr",
    inputSourceName,
    inputLineNumber,
    message);
}

void msr2mxsrInternalError (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       message)
{
  gIndenter.resetToZero ();

  waeErrorWithoutExceptionWithInputLocation (
    "msr2mxsr INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED
  abort ();
#endif // MF_ABORT_TO_DEBUG_ERRORS_IS_ENABLED

  throw msr2mxsrInternalException (message);
}

//______________________________________________________________________________
void msr2mxsrWarning (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "msr2mxsr",
    inputSourceName,
    inputLineNumber,
    message);
}

void msr2mxsrError (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       message)
{
  waeErrorWithInputLocation (
    "msr2mxsr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gWaeOahGroup->getDontShowErrors ()) {
    if (! gWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw msr2mxsrException (message);
    }
    else {
      throw msr2mxsrException (message);
    }
  }
}


}
