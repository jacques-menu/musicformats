/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

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
void mxsr2msrWarning (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  waeWarning (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    message);
}

EXP void mxsr2msrWarningWithMeasureInfo (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message,
  const std::string  measureNumber,
  int                scoreMeasuresNumber)
{
  waeWarningWithMeasureInfo (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    message,
    measureNumber,
    scoreMeasuresNumber);
}

//______________________________________________________________________________
void mxsr2msrInternalWarning (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  waeInternalWarning (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    message);
}

void mxsr2msrInternalWarningWithMeasureInfo (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message,
  const std::string  measureNumber,
  int                scoreMeasuresNumber)
{
  waeInternalWarningWithMeasureInfo (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    message,
    measureNumber,
    scoreMeasuresNumber);
}

//______________________________________________________________________________
void mxsr2msrError (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeError (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  if (! gWaeOahGroup->getDontShowErrors ()) {
    if (! gWaeOahGroup->getDontQuitOnErrors ()) { // JMI
      throw mxsr2msrException (message);
    }
    else {
      throw mxsr2msrException (message);
    }
  }
}

void mxsr2msrErrorWithMeasureInfo (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  const std::string  measureNumber,
  int                scoreMeasuresNumber)
{
  waeErrorWithMeasureInfo (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message,
    measureNumber,
    scoreMeasuresNumber);

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
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeInternalError (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
}

void mxsr2msrInternalErrorWithMeasureInfo (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  const std::string  measureNumber,
  int                scoreMeasuresNumber)
{
  waeInternalErrorWithMeasureInfo (
    "mxsr2msr",
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message,
    measureNumber,
    scoreMeasuresNumber);
}


}
