/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___waeInterface___
#define ___waeInterface___

#include "exports.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
// warnings

EXP void waeWarning (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message);

EXP void waeWarningWithLocationDetails (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message);
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber);

//______________________________________________________________________________
// internal warnings

EXP void waeInternalWarning (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message);

EXP void waeInternalWarningWithLocationDetails (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message);
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber);

//______________________________________________________________________________
// errors without exceptions

EXP void waeErrorWithoutException (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);

EXP void waeErrorWithoutExceptionWithLocationDetails (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber);

EXP void waeErrorWithoutException (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);

EXP void waeErrorWithoutExceptionWithLocationDetails (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber);

//______________________________________________________________________________
// errors with exceptions

EXP void waeErrorWithException (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  S_mfException      except);

EXP void waeErrorWithExceptionWithLocationDetails (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  S_mfException      except);
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber);

EXP void waeErrorWithException (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  S_mfException      except);

EXP void waeErrorWithExceptionWithLocationDetails (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  S_mfException      except);
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber);

//______________________________________________________________________________
// errors

EXP void waeError (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);

EXP void waeErrorWithLocationDetails (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber);

EXP void waeError (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);

EXP void waeErrorWithLocationDetails (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber);

//______________________________________________________________________________
// internal errors

EXP void waeInternalError (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);

EXP void waeInternalErrorWithLocationDetails (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber);

EXP void waeInternalErrorWithException (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  S_mfException      except);

EXP void waeInternalErrorWithExceptionWithLocationDetails (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  S_mfException      except);
//   const std::string  measureNumber,
//   int                scoreMeasuresNumber);

//______________________________________________________________________________

EXP void displayWarningsAndErrorsInputLineNumbers ();


}


#endif // ___waeInterface___

