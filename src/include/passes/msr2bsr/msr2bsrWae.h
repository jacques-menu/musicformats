/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2bsrWae___
#define ___msr2bsrWae___

#include "exports.h"

#include "mfStringsHandling.h"

#include "mfExceptions.h"

using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP msr2bsrException: public mfException
{
  public:
    msr2bsrException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msr2bsrException> S_msr2bsrException;

class EXP msr2bsrInternalException: public mfException
{
  public:
    msr2bsrInternalException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msr2bsrInternalException> S_msr2bsrInternalException;

class EXP msr2bsrUnsupportedException: public mfException
{
  public:
    msr2bsrUnsupportedException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msr2bsrUnsupportedException> S_msr2bsrUnsupportedException;

//______________________________________________________________________________
EXP void msr2bsrUnsupported (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

//______________________________________________________________________________
EXP void msr2bsrInternalWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void msr2bsrInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

//______________________________________________________________________________
EXP void msr2bsrWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void msr2bsrError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);


}


#endif
