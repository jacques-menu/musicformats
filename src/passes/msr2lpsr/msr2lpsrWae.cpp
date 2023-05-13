/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStringsHandling.h"
#include "mfIndentedTextOutput.h"

#include "oahEarlyOptions.h"

#include "waeInterface.h"
#include "msr2lpsrWae.h"

#include "oahOah.h"
#include "waeOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
void msr2lpsrUnsupported (
  const std::string& inputSourceName,
  int                inputLineNumber,
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

  throw msr2lpsrUnsupportedException (message);
}

//______________________________________________________________________________
void msr2lpsrInternalWarning (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  waeInternalWarning (
    "msr2lpsr",
    inputSourceName,
    inputLineNumber,
    message);
}

void msr2lpsrInternalError (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeInternalErrorWithInputLocation (
    "msr2lpsr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
}

//______________________________________________________________________________
void msr2lpsrWarning (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "LPSR",
    inputSourceName,
    inputLineNumber,
    message);
}


}
