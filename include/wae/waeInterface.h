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

// #include <string>

#include "exports.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
EXP void waeWarning (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message);

EXP void waeInternalWarning (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message);

//______________________________________________________________________________
EXP void waeErrorWithoutException (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);

EXP void waeErrorWithoutException (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);

EXP void waeError (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);

EXP void waeError (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);

EXP void waeErrorWithException (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  S_mfException      except);

EXP void waeErrorWithException (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  S_mfException      except);

//______________________________________________________________________________
EXP void waeInternalError (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);

EXP void waeInternalErrorWithException (
  const std::string& context,
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message,
  S_mfException      except);

//______________________________________________________________________________

EXP void displayWarningsAndErrorsInputLineNumbers ();


}


#endif

