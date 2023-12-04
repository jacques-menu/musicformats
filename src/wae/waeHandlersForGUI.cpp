/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfPreprocessorSettings.h"

#include "mfStringsHandling.h"

#include "oahOah.h"
#include "oahEarlyOptions.h"

#include "waeOah.h"

#include "waeHandlersForGUI.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_waeHandlerForGUI waeHandlerForGUI::create ()
{
  waeHandlerForGUI* obj =
    new waeHandlerForGUI ();
  assert (obj != nullptr);
  return obj;
}

waeHandlerForGUI::waeHandlerForGUI ()
{}

waeHandlerForGUI::~waeHandlerForGUI ()
{}

//______________________________________________________________________________
void waeHandlerForGUI::waeWarning (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  if (! gEarlyOptions.getEarlyQuietOption ()) {
    int saveIndent = gIndenter.getIndentation ();

    gIndenter.resetToZero ();

    gLog <<
      "*** " << context << " warning *** " <<
      inputSourceName << ":" << inputLineNumber << ": " <<message <<
      std::endl;

    fWarningsInputLineNumbers.insert (inputLineNumber);

    gIndenter.setIndentation (saveIndent);
  }
}

void waeHandlerForGUI::waeInternalWarning (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  if (! gEarlyOptions.getEarlyQuietOption ()) {
    int saveIndent = gIndenter.getIndentation ();

    gIndenter.resetToZero ();

    gLog <<
      "*** " << context << " INTERNAL warning *** " <<
      inputSourceName << ":" << inputLineNumber << ": " <<message <<
      std::endl;

    fWarningsInputLineNumbers.insert (
      inputLineNumber);

    gIndenter.setIndentation (saveIndent);
  }
}

//______________________________________________________________________________
void waeHandlerForGUI::waeErrorWithoutException (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  if (! gEarlyOptions.getEarlyQuietOption ()) {
    if (gOahOahGroup->getDisplaySourceCodePositions ()) {
      gLog <<
        mfBaseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        ' ';
    }

    if (! gWaeOahGroup->getDontShowErrors ()) {
      int saveIndent = gIndenter.getIndentation ();

      gIndenter.resetToZero ();

      gLog <<
        "### " << context << " ERROR ### " <<
        message <<
        std::endl;

      gIndenter.setIndentation (saveIndent);
    }
  }
}

void waeHandlerForGUI::waeErrorWithoutExceptionWithInputLocation (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  if (! gEarlyOptions.getEarlyQuietOption ()) {
    if (gOahOahGroup->getDisplaySourceCodePositions ()) {
      gLog <<
        mfBaseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        ' ';
    }

    if (! gWaeOahGroup->getDontShowErrors ()) {
      int saveIndent = gIndenter.getIndentation ();

      gIndenter.resetToZero ();

      gLog <<
        "### " << context << " ERROR ### " <<
        inputSourceName << ":" << inputLineNumber << ": " << message <<
        std::endl;

      gIndenter.setIndentation (saveIndent);

      fErrorsInputLineNumbers.insert (
        inputLineNumber);
    }
  }
}

void waeHandlerForGUI::waeError (
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

  throw mfException (message);
}

void waeHandlerForGUI::waeErrorWithInputLocation (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
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

void waeHandlerForGUI::waeErrorWithException (
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

abort (); // JMI

  throw *except;
}

void waeHandlerForGUI::waeErrorWithExceptionWithInputLocation (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
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
void waeHandlerForGUI::waeInternalErrorWithInputLocation (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
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

void waeHandlerForGUI::waeInternalErrorWithExceptionWithInputLocation (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
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

//______________________________________________________________________________
void waeHandlerForGUI::displayWarningsAndErrorsInputLineNumbers ()
{

  gIndenter.resetToZero ();

  size_t warningsInputLineNumbersSize =
    fWarningsInputLineNumbers.size ();

  if (
    warningsInputLineNumbersSize
      &&
    ! gEarlyOptions.getEarlyQuietOption ()
  ) {
    gLog <<
      std::endl <<
      mfSingularOrPluralWithoutNumber (
        warningsInputLineNumbersSize, "A warning message has", "Warning messages have") <<
      " been issued for input " <<
      mfSingularOrPluralWithoutNumber (
        warningsInputLineNumbersSize, "line", "lines") <<
      ' ';

    std::set<int>::const_iterator
      iBegin = fWarningsInputLineNumbers.begin (),
      iEnd   = fWarningsInputLineNumbers.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLog << (*i);
      if (++i == iEnd) break;
      gLog << ", ";
    } // for

    gLog << std::endl;
  }

  size_t errorsInputLineNumbersSize =
    fErrorsInputLineNumbers.size ();

  if (errorsInputLineNumbersSize) {
    gLog <<
      std::endl <<
      mfSingularOrPluralWithoutNumber (
        errorsInputLineNumbersSize, "An error message has", "Error messages have") <<
      " been issued for input " <<
      mfSingularOrPluralWithoutNumber (
        errorsInputLineNumbersSize, "line", "lines") <<
      ' ';

    std::set<int>::const_iterator
      iBegin = fErrorsInputLineNumbers.begin (),
      iEnd   = fErrorsInputLineNumbers.end (),
      i      = iBegin;
    for ( ; ; ) {
      gLog << (*i);
      if (++i == iEnd) break;
      gLog << ", ";
    } // for

    gLog << std::endl;
  }
}

void waeHandlerForGUI::print (std::ostream& os) const
{
  os <<
    "waeHandlerForGUI" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_waeHandlerForGUI& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
