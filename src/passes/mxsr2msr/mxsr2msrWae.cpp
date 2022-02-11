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
#include "mxsr2msrWae.h"

#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
void mxsr2msrWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message)
{
  waeWarning (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    message);
}

void mxsr2msrInternalWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message)
{
  waeInternalWarning (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    message);
}

void mxsr2msrError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  waeError (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) {
    if (! gGlobalWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw mxsr2msrException (message);
    }
    else {
      throw mxsr2msrException (message);
    }
  }
}

void mxsr2msrInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  waeInternalError (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
}


}
