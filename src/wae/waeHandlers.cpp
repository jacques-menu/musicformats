/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfPreprocessorSettings.h"

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
  waeHandler* obj =
    new waeHandler ();
  assert (obj != nullptr);
  return obj;
}

waeHandler::waeHandler ()
{}

waeHandler::~waeHandler ()
{}

//______________________________________________________________________________
// warnings

void waeHandler::waeWarning (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  if (! gEarlyOptions.getEarlyQuietOption ()) {
    int saveIndent = gIndenter.getIndentation ();

    gIndenter.resetToZero ();

    gLog <<
      "*** " << context << " warning *** " <<
      mfBaseName (inputSourceName) << ":" << inputLineNumber << ": " <<
      message;

    if (gServiceRunData) {
      S_mfService
        service =
          getGlobalService ();

      if (service) {
        gLog <<
          " -- service " <<
          service <<
          ", measure ";
      }
      else {
        gLog <<
          " -- measure ";
      }

      gLog <<
        gServiceRunData->getCurrentMeasureNumber () <<
        '/' <<
        gServiceRunData->getScoreMeasuresNumber () <<
        ", part: " <<
        gServiceRunData->getCurrentPartIDAndName () <<
        ", staff: " <<
        gServiceRunData->getCurrentStaveNumber () <<
        ", voice: " <<
        gServiceRunData->getCurrentVoiceNumber ();
    }

    gLog << std::endl << std::endl;

    fWarningsInputLineNumbers.insert (inputLineNumber);

    gIndenter.setIndentation (saveIndent);
  }
}

//______________________________________________________________________________
// internal warnings

void waeHandler::waeInternalWarning (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  if (! gEarlyOptions.getEarlyQuietOption ()) {
    int saveIndent = gIndenter.getIndentation ();

    gIndenter.resetToZero ();

    gLog <<
      "*** " << context << " INTERNAL warning *** " <<
      mfBaseName (inputSourceName) << ":" << inputLineNumber << ": " <<
      message;

    if (gServiceRunData) {
      S_mfService
        service =
          getGlobalService ();

      if (service) {
        gLog <<
          " -- service " <<
          service <<
          ", measure ";
      }
      else {
        gLog <<
          " -- measure ";
      }

      gLog <<
        gServiceRunData->getCurrentMeasureNumber () <<
        '/' <<
        gServiceRunData->getScoreMeasuresNumber () <<
        ", part: " <<
        gServiceRunData->getCurrentPartIDAndName () <<
        ", staff: " <<
        gServiceRunData->getCurrentStaveNumber () <<
        ", voice: " <<
        gServiceRunData->getCurrentVoiceNumber ();
    }

    gLog << std::endl << std::endl;

    fWarningsInputLineNumbers.insert (
      inputLineNumber);

    gIndenter.setIndentation (saveIndent);
  }
}

//______________________________________________________________________________
// errors without exceptions

void waeHandler::waeErrorWithoutException (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  if (! gEarlyOptions.getEarlyQuietOption ()) {
    int saveIndent = gIndenter.getIndentation ();

    gIndenter.resetToZero ();

    if (gOahOahGroup->getDisplaySourceCodePositions ()) {
      gLog <<
        mfBaseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " --FOO 2 -- " <<
        ' ';
    }

    if (! gWaeOahGroup->getDontShowErrors ()) {
      gLog <<
        "### " << context << " ERROR ### " <<
//         mfBaseName (sourceCodeFileName) << ":" << sourceCodeLineNumber << ": " <<
        message;

      if (gServiceRunData) {
        S_mfService
          service =
            getGlobalService ();

        if (service) {
          gLog <<
            " -- service " <<
            service <<
            ", measure ";
        }
        else {
          gLog <<
            " -- measure ";
        }

        gLog <<
          gServiceRunData->getCurrentMeasureNumber () <<
          '/' <<
          gServiceRunData->getScoreMeasuresNumber () <<
          ", part: " <<
          gServiceRunData->getCurrentPartIDAndName () <<
          ", staff: " <<
          gServiceRunData->getCurrentStaveNumber () <<
          ", voice: " <<
          gServiceRunData->getCurrentVoiceNumber ();
      }

      gLog << std::endl << std::endl;
    }

    gIndenter.setIndentation (saveIndent);
  }
}

void waeHandler::waeErrorWithoutExceptionWithInputLocation (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  if (! gEarlyOptions.getEarlyQuietOption ()) {
    int saveIndent = gIndenter.getIndentation ();

    gIndenter.resetToZero ();

    if (gOahOahGroup->getDisplaySourceCodePositions ()) {
      gLog <<
        mfBaseName (sourceCodeFileName) << ":" << sourceCodeLineNumber <<
        " --FOO 4 -- " <<
        ' ';
    }

    if (! gWaeOahGroup->getDontShowErrors ()) {
      gLog <<
        "### " << context << " ERROR ### " <<
        mfBaseName (inputSourceName) << ":" << inputLineNumber << ": " <<
        message;

      if (gServiceRunData) {
        S_mfService
          service =
            getGlobalService ();

        if (service) {
          gLog <<
            " -- service " <<
            service <<
            ", measure ";
        }
        else {
          gLog <<
            " -- measure ";
        }

        gLog <<
          gServiceRunData->getCurrentMeasureNumber () <<
          '/' <<
          gServiceRunData->getScoreMeasuresNumber () <<
          ", part: " <<
          gServiceRunData->getCurrentPartIDAndName () <<
          ", staff: " <<
          gServiceRunData->getCurrentStaveNumber () <<
          ", voice: " <<
          gServiceRunData->getCurrentVoiceNumber ();
      }

      gLog << std::endl << std::endl;

      fErrorsInputLineNumbers.insert (
        inputLineNumber);
    }

    gIndenter.setIndentation (saveIndent);
  }
}

//______________________________________________________________________________
// errors with exceptions

void waeHandler::waeErrorWithException (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  const S_mfException& except)
{
  this->waeErrorWithoutException (
    context,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

abort (); // JMI

  throw *except;
}

void waeHandler::waeErrorWithExceptionWithInputLocation (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  const S_mfException& except)
{
  this->waeErrorWithoutExceptionWithInputLocation (
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
// errors

void waeHandler::waeError (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  this->waeErrorWithoutException (
    context,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  throw mfException (message);
}

void waeHandler::waeErrorWithInputLocation (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  this->waeErrorWithoutExceptionWithInputLocation (
    context,
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  throw mfException (message);
}

//______________________________________________________________________________
// internal errors

void waeHandler::waeInternalErrorWithInputLocation (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message)
{
  this->waeErrorWithoutExceptionWithInputLocation (
    context,
    inputSourceName,
    inputLineNumber,
    sourceCodeFileName,
    sourceCodeLineNumber,
    message);

  throw mfException (message);
}

void waeHandler::waeInternalErrorWithExceptionWithInputLocation (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  const S_mfException& except)
{
  this->waeErrorWithoutExceptionWithInputLocation (
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

void waeHandler::waeTraceToStreamWithoutInputLocation (
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
      case mfPassIDKind::kMfPassID_UNKNOWN_:
        os <<
          "waeHandler::waeTraceWithoutInputLocation: " <<
          "fEarlyTraceOnlyPass = " << earlyTraceOnlyPass <<
          std::endl;
        break;

      case mfPassIDKind::kMfPassID_ALL:
        doTraceCurrentPass = true;
        break;

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

//     '[' << context << "] " << JMI v0.9.68

    // message can be a multi-line string
    gIndenter.indentMultiLineString (
      message,
      os);
    os << std::endl;

    --gIndenter;
  }
}

void waeHandler::waeTraceToStream (
//    const std::string& context,
  std::ostream&      os,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  // should the current pass be traced?
  Bool doTraceCurrentPass (false);

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
      case mfPassIDKind::kMfPassID_UNKNOWN_:
        os <<
          "waeHandler::waeTraceToStreamWithLocationDetails: " <<
          "fEarlyTraceOnlyPass = " << earlyTraceOnlyPass <<
          std::endl;
        break;

      case mfPassIDKind::kMfPassID_ALL:
        doTraceCurrentPass = true;
        break;

      default:
        doTraceCurrentPass =
          earlyTraceOnlyPass == globalCurrentPassIDKind;
    } // switch
  }

  // should the current measure be traced?
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

//     '[' << context << "] " << JMI v0.9.68
    os <<
      mfBaseName (inputSourceName) << ":" << inputLineNumber << ": " <<
      message;

    if (gServiceRunData) {
      S_mfService
        service =
          getGlobalService ();

      if (service) {
        os <<
          " -- service " <<
          service <<
          ", measure ";
      }
      else {
        os <<
          " -- measure ";
      }

      os <<
        gServiceRunData->getCurrentMeasureNumber () <<
        '/' <<
        gServiceRunData->getScoreMeasuresNumber () <<
        ", part: " <<
        gServiceRunData->getCurrentPartIDAndName () <<
        ", staff: " <<
        gServiceRunData->getCurrentStaveNumber () <<
        ", voice: " <<
        gServiceRunData->getCurrentVoiceNumber ();
    }

    os << std::endl << std::endl;

//     os << ss.str ();
//     gIndenter.indentMultiLineString (
//       iss.str (),
//       os);

    --gIndenter;
  }
}

void waeHandler::waeTraceWithoutInputLocation (
//   const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
{
  this->waeTraceToStreamWithoutInputLocation (
    gLog,
    inputSourceName,
    inputLineNumber,
    message);
}

void waeHandler::waeTrace (
//   const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message)
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber)
{
  this->waeTraceToStream (
    gLog,
    inputSourceName,
    inputLineNumber,
    message);
//     measureNumber,
//     scoreMeasuresNumber);
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
    os << "[NULL]" << std::endl;
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
