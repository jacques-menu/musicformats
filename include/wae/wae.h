/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___wae___
#define ___wae___

#include <string>
#include <set>

#include "exports.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
void waeWarning (
  const std::string& context,
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message);

void waeInternalWarning (
  const std::string& context,
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message);

//______________________________________________________________________________
void waeErrorWithoutException (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);

void waeErrorWithoutException (
  const std::string& context,
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);

void waeError (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);

void waeError (
  const std::string& context,
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);

void waeErrorWithException (
  const std::string& context,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message,
  S_mfException except);

void waeErrorWithException (
  const std::string& context,
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message,
  S_mfException except);

//______________________________________________________________________________
void waeInternalError (
  const std::string& context,
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);

void waeInternalErrorWithException (
  const std::string& context,
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message,
  S_mfException except);

//______________________________________________________________________________
EXP extern std::set<int> gGlobalWarningsInputLineNumbers;
EXP extern std::set<int> gGlobalErrorsInputLineNumbers;

EXP void displayWarningsAndErrorsInputLineNumbers ();

}


#endif

//______________________________________________________________________________
/*
EXP void msrStreamsError (
  int    inputLineNumber,
  const std::string& sourceCodeFileName,
  int    sourceCodeLineNumber,
  const std::string& message);

EXP void msrStreamsWarning (
  int    inputLineNumber,
  const std::string& sourceCodeFileName,
  int    sourceCodeLineNumber,
  const std::string& message);
*/

