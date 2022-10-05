/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2lpsrWae___
#define ___msr2lpsrWae___

#include "exports.h"

#include "mfStringsHandling.h"

#include "mfExceptions.h"

using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP msr2lpsrException: public mfException
{
  public:
    msr2lpsrException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msr2lpsrException> S_msr2lpsrException;

class EXP msr2lpsrInternalException: public mfException
{
  public:
    msr2lpsrInternalException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msr2lpsrInternalException> S_msr2lpsrInternalException;

class EXP msr2lpsrUnsupportedException: public mfException
{
  public:
    msr2lpsrUnsupportedException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msr2lpsrUnsupportedException> S_msr2lpsrUnsupportedException;

//______________________________________________________________________________
EXP void msr2lpsrUnsupported (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int    sourceCodeLineNumber,
  const string& message);

//______________________________________________________________________________
EXP void msr2lpsrInternalWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void msr2lpsrInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

//______________________________________________________________________________
EXP void msr2lpsrWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void msr2lpsrError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);


}


#endif
