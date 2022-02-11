/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

#include "wae.h"
#include "musicxml2mxsrWae.h"

#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
void musicxml2mxsrWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message)
{
  waeWarning (
    "musicxml2mxsr",
    inputSourceName,
    inputLineNumber,
    message);
}

void musicxml2mxsrError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  waeError (
    "musicxml2mxsr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) {
    if (! gGlobalWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw musicxml2mxsrException (message);
    }
    else {
      throw musicxml2mxsrException (message);
    }
  }
}

void musicxml2mxsrInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  waeInternalError (
    "musicxml2mxsr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
}


}
