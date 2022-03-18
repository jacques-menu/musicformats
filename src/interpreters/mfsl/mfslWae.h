/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfslWae___
#define ___mfslWae___

#include "exports.h"

#include "mfExceptions.h"

#include "location.hh"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP mfslException : public mfException
{
  public:
    mfslException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfslException> S_mfslException;

class EXP mfslInternalException: public mfException
{
  public:
    mfslInternalException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfslInternalException> S_mfslInternalException;

class EXP msdrException : public mfException
{
  public:
    msdrException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdrException> S_msdrException;

class EXP msdrInternalException: public mfException
{
  public:
    msdrInternalException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdrInternalException> S_msdrInternalException;

class EXP mfsl2msdrException : public mfException
{
  public:
    mfsl2msdrException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfsl2msdrException> S_mfsl2msdrException;

class EXP mfsl2msrInternalException: public mfException
{
  public:
    mfsl2msrInternalException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfsl2msrInternalException> S_mfsl2msrInternalException;

class EXP mfsl2msrException : public mfException
{
  public:
    mfsl2msrException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfsl2msrException> S_mfsl2msrException;

class EXP mfsl2lilyponException: public mfException
{
  public:
    mfsl2lilyponException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfsl2lilyponException> S_mfsl2lilyponException;

class EXP mfsl2brailleException: public mfException
{
  public:
    mfsl2brailleException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfsl2brailleException> S_mfsl2brailleException;

class EXP mfsl2musicxmlException: public mfException
{
  public:
    mfsl2musicxmlException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfsl2musicxmlException> S_mfsl2musicxmlException;

//______________________________________________________________________________
EXP void mfslWarning (
  const string&       message,
  const yy::location& loc);

EXP void mfslError (
  const string&       message,
  const yy::location& loc);

EXP void mfslInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

EXP void mfslFileError (
  const string& inputSourceName,
  const string& message);


}


#endif
