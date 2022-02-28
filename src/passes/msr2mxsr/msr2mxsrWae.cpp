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
#include "msr2mxsrWae.h"

#include "oahOah.h"
#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
void msr2mxsrUnsupported (
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
      "### msr2mxsr LIMITATION ### " <<
      inputSourceName << ":" << inputLineNumber << ": " << message <<
      endl;

    gIndenter.setIndent (saveIndent);
  }

  throw msr2mxsrUnsupportedException (message);
}

//______________________________________________________________________________
void msr2mxsrInternalWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message)
{
  waeInternalWarning (
    "msr2mxsr",
    inputSourceName,
    inputLineNumber,
    message);
}

void msr2mxsrInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  waeErrorWithoutException (
    "msr2mxsr INTERNAL",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

#ifdef ABORT_TO_DEBUG_ERRORS
  abort ();
#endif

  gIndenter.setIndent (saveIndent);

  throw msr2mxsrInternalException (message);
}

//______________________________________________________________________________
void msr2mxsrWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message)
{
  waeWarning (
    "msr2mxsr",
    inputSourceName,
    inputLineNumber,
    message);
}

void msr2mxsrError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  waeError (
    "msr2mxsr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) {
    if (! gGlobalWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw msr2mxsrException (message);
    }
    else {
      throw msr2mxsrException (message);
    }
  }
}


}
