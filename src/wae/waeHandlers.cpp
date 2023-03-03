/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStaticSettings.h"

#include "mfStringsHandling.h"

#include "oahEarlyOptions.h"
#include "oahOah.h"
#include "waeOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_waeHandler waeHandler::create ()
{
  waeHandler* o =
    new waeHandler ();
  assert (o != nullptr);
  return o;
}

waeHandler::waeHandler ()
{}

waeHandler::~waeHandler ()
{}

//______________________________________________________________________________
void waeHandler::waeWarning (
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
      inputSourceName << ":" << inputLineNumber << ": " << message <<
      std::endl;

    fWarningsInputLineNumbers.insert (inputLineNumber);

    gIndenter.setIndentation (saveIndent);
  }
}

void waeHandler::waeInternalWarning (
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
      inputSourceName << ":" << inputLineNumber << ": " << message <<
      std::endl;

    fWarningsInputLineNumbers.insert (
      inputLineNumber);

    gIndenter.setIndentation (saveIndent);
  }
}

//______________________________________________________________________________
void waeHandler::waeErrorWithoutException (
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

void waeHandler::waeErrorWithoutException (
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

void waeHandler::waeError (
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

void waeHandler::waeError (
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

void waeHandler::waeErrorWithException (
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

abort (); // JMI

  throw *except;
}

void waeHandler::waeErrorWithException (
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

//______________________________________________________________________________
void waeHandler::waeInternalError (
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

void waeHandler::waeInternalErrorWithException (
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

//______________________________________________________________________________
#ifdef MF_TRACE_IS_ENABLED

void waeHandler::waeTrace (
//   const std::string& context,
  std::ostream&      os,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  // should the current pass be traced?

  Bool doTraceCurrentPass;

  {
    mfPassIDKind
      earlyTraceOnlyPass =
        gEarlyOptions.getEarlyTraceOnlyPass (),
      globalCurrentPassIDKind =
        getGlobalCurrentPassIDKind ();

  //   os <<
  //     "--> earlyTraceOnlyPass:      " << earlyTraceOnlyPass << std::endl <<
  //     "--> globalCurrentPassIDKind: " << globalCurrentPassIDKind << std::endl <<
  //     std::endl;

    switch (earlyTraceOnlyPass) {
      case mfPassIDKind::kMfPassID_UNKNOWN:
        os <<
          "waeHandler::waeTrace: " <<
          "fEarlyTraceOnlyPass = " << earlyTraceOnlyPass <<
          std::endl;
        break;

      case mfPassIDKind::kMfPassID_ALL:
        doTraceCurrentPass = true;
        break;

  //     case mfPassIDKind::kMfPassID_OptionsAndArgumentsHandling:
  //       break;
  //
  //     case mfPassIDKind::kMfPassID_1:
  //       break;
  //
  //     case mfPassIDKind::kMfPassID_2:
  //       break;
  //     case mfPassIDKind::kMfPassID_2a:
  //       break;
  //     case mfPassIDKind::kMfPassID_2b:
  //       break;
  //
  //     case mfPassIDKind::kMfPassID_3:
  //       break;
  //     case mfPassIDKind::kMfPassID_3a:
  //       break;
  //     case mfPassIDKind::kMfPassID_3b:
  //       break;
  //
  //     case mfPassIDKind::kMfPassID_4:
  //       break;
  //     case mfPassIDKind::kMfPassID_4a:
  //       break;
  //     case mfPassIDKind::kMfPassID_4b:
  //       break;
  //
  //     case mfPassIDKind::kMfPassID_5:
  //       break;

      default:
        doTraceCurrentPass =
          earlyTraceOnlyPass == globalCurrentPassIDKind;
    } // switch
  }

  Bool doTraceCurrentMeasure;

  {
    S_oahStringAtom
      traceOnlyMeasurerNumbeOahAtom =
         gTraceOahGroup->getTraceOnlyMeasurerNumbeOahAtom ();

    if (traceOnlyMeasurerNumbeOahAtom->getSelected ()) {
      doTraceCurrentMeasure =
        gServiceRunData->getCurrentMeasureNumber ()
          ==
        traceOnlyMeasurerNumbeOahAtom->getStringVariable ();
    }
    else {
      doTraceCurrentMeasure = true;
    }
  }



  // do trace if relevant
  if (doTraceCurrentPass && doTraceCurrentMeasure) {
    ++gIndenter;

//     '[' << context << "] " <<
//     inputSourceName << ":" << inputLineNumber << ": " <<

    // message can be a multi-line string
    gIndenter.indentMultiLineString (
      message,
      os);
    os << std::endl;

    --gIndenter;
  }
}

void waeHandler::waeTrace (
//   const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  waeTrace (
    gLog,
    inputSourceName,
    inputLineNumber,
    message);
}

#endif // MF_TRACE_IS_ENABLED

//______________________________________________________________________________
void waeHandler::displayWarningsAndErrorsInputLineNumbers ()
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

std::string waeHandler::asString () const
{
  std::stringstream ss;

  ss <<
    "WAE handler";

  return ss.str ();
}

void waeHandler::print (std::ostream& os) const
{
  os <<
    "WaeHandler" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_waeHandler& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//________________________________________________________________________
// hidden global WAE handler variable
S_waeHandler pGlobalWaeHandler;

EXP void setGlobalWaeHandler (S_waeHandler handler)
{
  pGlobalWaeHandler = handler;
}

EXP S_waeHandler getGlobalWaeHandler ()
{
  return pGlobalWaeHandler;
}

//________________________________________________________________________
EXP void initializeWAE ()
{
  pGlobalWaeHandler = waeHandler::create ();
}


}
