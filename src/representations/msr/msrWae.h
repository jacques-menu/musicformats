/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msrWae___
#define ___msrWae___

#include "exports.h"

#include "mfStringsHandling.h"

#include "mfExceptions.h"

using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP msrException: public mfException
{
  public:
    msrException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msrException> S_msrException;

class EXP msrInternalException: public mfException
{
  public:
    msrInternalException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msrInternalException> S_msrInternalException;

class EXP msrUnsupportedException: public mfException
{
  public:
    msrUnsupportedException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msrUnsupportedException> S_msrUnsupportedException;

//______________________________________________________________________________
EXP void msrWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void msrError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

EXP void msrUnsupported (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

EXP void msrInternalWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void msrInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);


}


#endif
