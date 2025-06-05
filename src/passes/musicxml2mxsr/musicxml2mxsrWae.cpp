/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

#include "waeInterface.h"
#include "musicxml2mxsrWae.h"

#include "waeOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
void musicxml2mxsrWarning (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "musicxml2mxsr",
    inputSourceName,
    inputLineNumber,
    message);
}

void musicxml2mxsrError (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeErrorWithInputLocation (
    "musicxml2mxsr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gWaeOahGroup->getDontShowErrors ()) {
    if (! gWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw musicxml2mxsrException (message);
    }
    else {
      throw musicxml2mxsrException (message);
    }
  }
}

void musicxml2mxsrInternalError (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeInternalErrorWithInputLocation (
    "musicxml2mxsr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
}


}
