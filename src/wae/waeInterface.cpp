/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

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
  const mfInputLineNumber& inputLineNumber,
  const std::string& message)
{
  gWaeHandler->
    waeWarning (
      context,
      inputSourceName,
      inputLineNumber,
      message);
}

//______________________________________________________________________________
// internal warnings

void waeInternalWarning (
  const std::string& context,
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message)
{
  gWaeHandler->
    waeInternalWarning (
      context,
      inputSourceName,
      inputLineNumber,
      message);
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

void waeErrorWithoutExceptionWithInputLocation (
  const std::string& context,
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  gWaeHandler->
    waeErrorWithoutExceptionWithInputLocation (
      context,
      inputSourceName,
      inputLineNumber,
      sourceCodeFileName,
      sourceCodeLineNumber,
      message);
}

// ------------------------------------------------------
// errors with exception

void waeErrorWithException (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  const S_mfException& except)
{
  waeErrorWithoutException (
    context,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

abort (); // JMI 0.9.67

  throw *except;
}

void waeErrorWithExceptionWithInputLocation (
  const std::string& context,
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  const S_mfException& except)
{
  waeErrorWithoutExceptionWithInputLocation (
    context,
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

abort (); // JMI

  throw *except;
}

//______________________________________________________________________________
// internal errors

void waeInternalErrorWithInputLocation (
  const std::string& context,
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeErrorWithoutExceptionWithInputLocation (
    context,
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  throw mfException (message);
}

void waeInternalErrorWithExceptionWithInputLocation (
  const std::string& context,
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  const S_mfException& except)
{
  waeErrorWithoutExceptionWithInputLocation (
    context,
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

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

abort (); // JMI 0.9.67

  throw mfException (message);
}

void waeErrorWithInputLocation (
  const std::string& context,
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  waeErrorWithoutExceptionWithInputLocation (
    context,
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

abort (); // JMI 0.9.67

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
