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


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
void waeWarning (
  const string& context,
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

void waeInternalWarning (
  const string& context,
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

//______________________________________________________________________________
void waeErrorWithoutException (
  const string& context,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

void waeErrorWithoutException (
  const string& context,
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

void waeError (
  const string& context,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

void waeError (
  const string& context,
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

void waeErrorWithException (
  const string& context,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message,
  S_mfException exception);

void waeErrorWithException (
  const string& context,
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message,
  S_mfException exception);

//______________________________________________________________________________
void waeInternalError (
  const string& context,
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

void waeInternalErrorWithException (
  const string& context,
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message,
  S_mfException exception);

//______________________________________________________________________________
EXP extern set<int> gGlobalWarningsInputLineNumbers;
EXP extern set<int> gGlobalErrorsInputLineNumbers;

EXP void displayWarningsAndErrorsInputLineNumbers ();

}


#endif

//______________________________________________________________________________
/*
EXP void msrStreamsError (
  int    inputLineNumber,
  const string& sourceCodeFileName,
  int    sourceCodeLineNumber,
  const string& message);

EXP void msrStreamsWarning (
  int    inputLineNumber,
  const string& sourceCodeFileName,
  int    sourceCodeLineNumber,
  const string& message);
*/

