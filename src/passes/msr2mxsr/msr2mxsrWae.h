/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2mxsrWae___
#define ___msr2mxsrWae___

#include "exports.h"

#include "mfStringsHandling.h"

#include "mfExceptions.h"

using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP msr2mxsrException: public mfException
{
  public:
    msr2mxsrException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msr2mxsrException> S_msr2mxsrException;

class EXP msr2mxsrInternalException: public mfException
{
  public:
    msr2mxsrInternalException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msr2mxsrInternalException> S_msr2mxsrInternalException;

class EXP msr2mxsrUnsupportedException: public mfException
{
  public:
    msr2mxsrUnsupportedException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msr2mxsrUnsupportedException> S_msr2mxsrUnsupportedException;

//______________________________________________________________________________
EXP void msr2mxsrUnsupported (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

//______________________________________________________________________________
EXP void msr2mxsrInternalWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void msr2mxsrInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

//______________________________________________________________________________
EXP void msr2mxsrWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void msr2mxsrError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);


}


#endif
