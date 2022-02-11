/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "wae.h"
#include "lpsr2lilypondWae.h"

#include "waeOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
void lpsr2lilypondWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message)
{
  waeWarning (
    "lpsr2lilypond",
    inputSourceName,
    inputLineNumber,
    message);
}

void lpsr2lilypondInternalWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message)
{
  waeInternalWarning (
    "lpsr2lilypond",
    inputSourceName,
    inputLineNumber,
    message);
}

void lpsr2lilypondError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  waeError (
    "lpsr2lilypond",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) {
    if (! gGlobalWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw lpsr2lilypondException (message);
    }
    else {
      throw lpsr2lilypondException (message);
    }
  }
}

void lpsr2lilypondInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message)
{
  waeInternalError (
    "lpsr2lilypond",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
}


}
