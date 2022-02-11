/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfExceptions.h"

#include "mfIndentedTextOutput.h"

#include "wae.h"
#include "oahWae.h"
#include "lpsrWae.h"

#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
void lpsrWarning (
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

void lpsrError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  waeError (
    "LPSR",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) {
    if (! gGlobalWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw lpsrException (message);
    }
    else {
      throw lpsrException (message);
    }
  }
}

void lpsrInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  waeInternalError (
    "LPSR",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
}


}
