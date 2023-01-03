/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

#include "waeInterface.h"
#include "mxsr2guidoWae.h"

#include "waeOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
void mxsr2guidoWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "mxsr2guido",
    inputSourceName,
    inputLineNumber,
    message);
}

void mxsr2guidoError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
{
  waeError (
    "mxsr2guido",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gGlobalWaeOahGroup->getDontShowErrors ()) {
    if (! gGlobalWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw mxsr2guidoException (message);
    }
    else {
      throw mxsr2guidoException (message);
    }
  }
}

void mxsr2guidoInternalError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
{
  waeInternalError (
    "mxsr2guido",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
}


}
