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
#include "msr2lpsrWae.h"

#include "oahOah.h"
#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
void msr2lpsrUnsupported (
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

  throw msr2lpsrUnsupportedException (message);
}

//______________________________________________________________________________
void msr2lpsrInternalWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message)
{
  waeInternalWarning (
    "msr2lpsr",
    inputSourceName,
    inputLineNumber,
    message);
}

void msr2lpsrInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  waeInternalError (
    "msr2lpsr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
}

//______________________________________________________________________________
void msr2lpsrWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message)
{
  waeWarning (
    "LPSR",
    inputSourceName,
    inputLineNumber,
    message);
}


}
