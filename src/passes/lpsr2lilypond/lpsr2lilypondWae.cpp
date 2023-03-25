/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "waeInterface.h"
#include "lpsr2lilypondWae.h"

#include "waeOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
void lpsr2lilypondWarning (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "lpsr2lilypond",
    inputSourceName,
    inputLineNumber,
    message);
}

void lpsr2lilypondInternalWarning (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  waeInternalWarning (
    "lpsr2lilypond",
    inputSourceName,
    inputLineNumber,
    message);
}

void lpsr2lilypondError (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeError (
    "lpsr2lilypond",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gWaeOahGroup->getDontShowErrors ()) {
    if (! gWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw lpsr2lilypondException (message);
    }
    else {
      throw lpsr2lilypondException (message);
    }
  }
}

void lpsr2lilypondInternalError (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
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
