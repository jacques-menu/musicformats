/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfExceptions.h"

#include "waeInterface.h"

#include "mfIndentedTextOutput.h"
#include "mfStringsHandling.h"

#include "oahEarlyOptions.h"

#include "oahOah.h"
#include "waeOah.h"

#include "waeInterface.h"
#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
// warnings

void waeWarning (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  gWaeHandler->
    waeWarning (
      context,
      inputSourceName,
      inputLineNumber,
      message);
}

void waeWarningWithLocationDetails (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  gWaeHandler->
    waeWarningWithLocationDetails (
      context,
      inputSourceName,
      inputLineNumber,
      message);
//       measureNumber,
//       scoreMeasuresNumber);
}

//______________________________________________________________________________
// internal warnings

void waeInternalWarning (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  gWaeHandler->
    waeInternalWarning (
      context,
      inputSourceName,
      inputLineNumber,
      message);
}

void waeInternalWarningWithLocationDetails (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  gWaeHandler->
    waeInternalWarningWithLocationDetails (
      context,
      inputSourceName,
      inputLineNumber,
      message);
//       measureNumber,
//       scoreMeasuresNumber);
}

//______________________________________________________________________________
// errors without exception

void waeErrorWithoutException (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  gWaeHandler->
    waeErrorWithoutException (
      context,
      sourceCodeFileName,
      sourceCodeLineNumber,
      message);
}

void waeErrorWithoutExceptionWithLocationDetails (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  gWaeHandler->
    waeErrorWithoutExceptionWithLocationDetails (
      context,
      sourceCodeFileName,
      sourceCodeLineNumber,
      message);
//       measureNumber,
//       scoreMeasuresNumber);
}

void waeErrorWithoutException (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  gWaeHandler->
    waeErrorWithoutException (
      context,
      inputSourceName,
      inputLineNumber,
      sourceCodeFileName,
      sourceCodeLineNumber,
      message);
}

void waeErrorWithoutExceptionWithLocationDetails (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  gWaeHandler->
    waeErrorWithoutExceptionWithLocationDetails (
      context,
      inputSourceName,
      inputLineNumber,
      sourceCodeFileName,
      sourceCodeLineNumber,
      message);
//       measureNumber,
//       scoreMeasuresNumber);
}

// ------------------------------------------------------
// errors with exception

void waeErrorWithException (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  S_mfException      except)
{
  waeErrorWithoutException (
    context,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

abort (); // JMI v0.9.67

  throw *except;
}

void waeErrorWithExceptionWithLocationDetails (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  S_mfException      except)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  waeErrorWithoutExceptionWithLocationDetails (
    context,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
//     measureNumber,
//     scoreMeasuresNumber);

abort (); // JMI v0.9.67

  throw *except;
}

void waeErrorWithException (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  S_mfException      except)
{
  waeErrorWithoutException (
    context,
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

abort (); // JMI

  throw *except;
}

void waeErrorWithExceptionWithLocationDetails (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  S_mfException      except)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  waeErrorWithoutExceptionWithLocationDetails (
    context,
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
//     measureNumber,
//     scoreMeasuresNumber);

abort (); // JMI

  throw *except;
}

//______________________________________________________________________________
// internal errors

void waeInternalError (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeErrorWithoutException (
    context,
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  throw mfException (message);
}

void waeInternalErrorWithLocationDetails (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  waeErrorWithoutExceptionWithLocationDetails (
    context,
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
//     measureNumber,
//     scoreMeasuresNumber);

  throw mfException (message);
}

void waeInternalErrorWithException (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  S_mfException      except)
{
  waeErrorWithoutException (
    context,
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  throw *except;
}

void waeInternalErrorWithExceptionWithLocationDetails (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  S_mfException      except)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  waeErrorWithoutExceptionWithLocationDetails (
    context,
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
//     measureNumber,
//     scoreMeasuresNumber);

  throw *except;
}

// ------------------------------------------------------
// errors

void waeError (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeErrorWithoutException (
    context,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

abort (); // JMI v0.9.67

  throw mfException (message);
}

void waeErrorWithLocationDetails (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  waeErrorWithoutExceptionWithLocationDetails (
    context,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
//     measureNumber,
//     scoreMeasuresNumber);

abort (); // JMI v0.9.67

  throw mfException (message);
}

void waeError (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeErrorWithoutException (
    context,
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

abort (); // JMI v0.9.67

  throw mfException (message);
}

void waeErrorWithLocationDetails (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  waeErrorWithoutExceptionWithLocationDetails (
    context,
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);
//     measureNumber,
//     scoreMeasuresNumber);

abort (); // JMI v0.9.67

  throw mfException (message);
}

//______________________________________________________________________________
// display input line numbers

void displayWarningsAndErrorsInputLineNumbers ()
{
  gWaeHandler->
    displayWarningsAndErrorsInputLineNumbers ();
}


}
