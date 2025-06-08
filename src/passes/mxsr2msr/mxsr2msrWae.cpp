/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfIndentedTextOutput.h"

#include "oahEarlyOptions.h"

#include "waeInterface.h"
#include "mxsr2msrWae.h"

#include "waeHandlers.h"
#include "waeOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
void mxsr2msrWarningWithoutInputLocation (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    message);
}

EXP void mxsr2msrWarning (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  waeWarning (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    message);
//     measureNumber,
//     scoreMeasuresNumber);
}

//______________________________________________________________________________
void mxsr2msrInternalWarning (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message)
{
  waeInternalWarning (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    message);
}

//______________________________________________________________________________
void mxsr2msrError (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  waeErrorWithInputLocation (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
//     measureNumber,
//     scoreMeasuresNumber);

  if (! gWaeOahGroup->getDontShowErrors ()) {
    if (! gWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw mxsr2msrException (message);
    }
    else {
      throw mxsr2msrException (message);
    }
  }
}

//______________________________________________________________________________
void mxsr2msrInternalError (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       message)
{
  waeInternalErrorWithInputLocation (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
}


}
