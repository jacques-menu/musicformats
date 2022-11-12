/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfExceptions.h"

#include "wae.h"

#include "mfIndentedTextOutput.h"
#include "mfStringsHandling.h"

#include "oahEarlyOptions.h"

#include "oahOah.h"
#include "waeOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
void waeWarning (
  const std::string& context,
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message)
{
  if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
    int saveIndent = gIndenter.getIndentation ();

    gIndenter.resetToZero ();

    gLogStream <<
      "*** " << context << " warning *** " <<
      inputSourceName << ":" << inputLineNumber << ": " <<message <<
      std::endl;

    gGlobalWarningsInputLineNumbers.insert (inputLineNumber);

    gIndenter.setIndentation (saveIndent);
  }
}

void waeInternalWarning (
  const std::string& context,
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message)
{
  if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
    int saveIndent = gIndenter.getIndentation ();

    gIndenter.resetToZero ();

    gLogStream <<
      "*** " << context << " INTERNAL warning *** " <<
      inputSourceName << ":" << inputLineNumber << ": " <<message <<
      std::endl;

    gGlobalWarningsInputLineNumbers.insert (
      inputLineNumber);

    gIndenter.setIndentation (saveIndent);
  }
}

//______________________________________________________________________________
void waeErrorWithoutException (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
{
  if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
    if (gGlobalOahOahGroup->getDisplaySourceCodePositions ()) {
      gLogStream <<
        mfBaseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        ' ';
    }

    if (! gGlobalWaeOahGroup->getDontShowErrors ()) {
      int saveIndent = gIndenter.getIndentation ();

      gIndenter.resetToZero ();

      gLogStream <<
        "### " << context << " ERROR ### " <<
        message <<
        std::endl;

      gIndenter.setIndentation (saveIndent);
    }
  }
}

void waeErrorWithoutException (
  const std::string& context,
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
{
  if (! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
    if (gGlobalOahOahGroup->getDisplaySourceCodePositions ()) {
      gLogStream <<
        mfBaseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        ' ';
    }

    if (! gGlobalWaeOahGroup->getDontShowErrors ()) {
      int saveIndent = gIndenter.getIndentation ();

      gIndenter.resetToZero ();

      gLogStream <<
        "### " << context << " ERROR ### " <<
        inputSourceName << ":" << inputLineNumber << ": " << message <<
        std::endl;

      gIndenter.setIndentation (saveIndent);

      gGlobalErrorsInputLineNumbers.insert (
        inputLineNumber);
    }
  }
}

void waeError (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message)
{
  waeErrorWithoutException (
    context,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  throw mfException (message);
}

void waeError (
  const std::string& context,
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
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

void waeErrorWithException (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message,
  S_mfException except)
{
  waeErrorWithoutException (
    context,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

abort (); // JMI

  throw *except;
}

void waeErrorWithException (
  const std::string& context,
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message,
  S_mfException except)
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

//______________________________________________________________________________
void waeInternalError (
  const std::string& context,
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
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

void waeInternalErrorWithException (
  const std::string& context,
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message,
  S_mfException except)
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

//______________________________________________________________________________
std::set<int> gGlobalWarningsInputLineNumbers;
std::set<int> gGlobalErrorsInputLineNumbers;

void displayWarningsAndErrorsInputLineNumbers ()
{

  gIndenter.resetToZero ();

  size_t warningsInputLineNumbersSize =
    gGlobalWarningsInputLineNumbers.size ();

  if (
    warningsInputLineNumbersSize
      &&
    ! gGlobalOahEarlyOptions.getEarlyQuietOption ()
  ) {
    gLogStream <<
      std::endl <<
      mfSingularOrPluralWithoutNumber (
        warningsInputLineNumbersSize, "A warning message has", "Warning messages have") <<
      " been issued for input " <<
      mfSingularOrPluralWithoutNumber (
        warningsInputLineNumbersSize, "line", "lines") <<
      ' ';

    std::set<int>::const_iterator
      iBegin = gGlobalWarningsInputLineNumbers.begin (),
      iEnd   = gGlobalWarningsInputLineNumbers.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogStream << (*i);
      if (++i == iEnd) break;
      gLogStream << ", ";
    } // for

    gLogStream << std::endl;
  }

  size_t errorsInputLineNumbersSize =
    gGlobalErrorsInputLineNumbers.size ();

  if (errorsInputLineNumbersSize) {
    gLogStream <<
      std::endl <<
      mfSingularOrPluralWithoutNumber (
        errorsInputLineNumbersSize, "An error message has", "Error messages have") <<
      " been issued for input " <<
      mfSingularOrPluralWithoutNumber (
        errorsInputLineNumbersSize, "line", "lines") <<
      ' ';

    std::set<int>::const_iterator
      iBegin = gGlobalErrorsInputLineNumbers.begin (),
      iEnd   = gGlobalErrorsInputLineNumbers.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLogStream << (*i);
      if (++i == iEnd) break;
      gLogStream << ", ";
    } // for

    gLogStream << std::endl;
  }
}


}


//______________________________________________________________________________
/*
void msrStreamsWarning (
  int    inputLineNumber,
  const std::string& sourceCodeFileName,
  int    sourceCodeLineNumber,
  std::string  message)
{
  if (! (gGlobalOahEarlyOptions.getEarlyQuietOption () && gGlobalWaeOahGroup->getDontShowErrors ())) {
    if (gGlobalOahOahGroup->getDisplaySourceCodePositions ()) {
      gLogStream <<
        mfBaseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        ' ';
    }

    gLogStream <<
      "*** " << "MSR STREAMS" << " warning *** " <<
      " ### " << "MSR STREAMS" << " ERROR ### " <<
      "fake line number" << ":" << inputLineNumber << ": " << message <<
      std::endl;
  }
}

void msrStreamsError (
  int    inputLineNumber,
  const std::string& sourceCodeFileName,
  int    sourceCodeLineNumber,
  std::string  message)
{
  if (! (gGlobalOahEarlyOptions.getEarlyQuietOption () && gGlobalWaeOahGroup->getDontShowErrors ())) {
    if (gGlobalOahOahGroup->getDisplaySourceCodePositions ()) {
      gLogStream <<
        mfBaseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        ' ';
    }

    gLogStream <<
      "### " << "MSR STREAMS" << " ERROR ### " <<
      "fake line number" << ":" << inputLineNumber << ": " << message <<
      std::endl;
  }

  throw mfStreamsException (message);
}
*/

