/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2msrWae___
#define ___mxsr2msrWae___

#include "exports.h"

#include "mfExceptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP mxsr2msrException : public mfException
{
  public:
    mxsr2msrException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mxsr2msrException> S_mxsr2msrException;

//______________________________________________________________________________
EXP void mxsr2msrWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void mxsr2msrInternalWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void mxsr2msrError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

EXP void mxsr2msrInternalError (
  const string& inputSourceName,
  int          inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);


}


#endif
