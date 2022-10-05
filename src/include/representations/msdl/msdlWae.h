/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdlWae___
#define ___msdlWae___

#include "exports.h"

#include "mfExceptions.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP msdlException : public mfException
{
  public:
    msdlException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdlException> S_msdlException;

class EXP msdlInternalException: public mfException
{
  public:
    msdlInternalException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdlInternalException> S_msdlInternalException;

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

class EXP msdl2msdrException : public mfException
{
  public:
    msdl2msdrException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdl2msdrException> S_msdl2msdrException;

class EXP msdl2msrInternalException: public mfException
{
  public:
    msdl2msrInternalException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdl2msrInternalException> S_msdl2msrInternalException;

class EXP msdl2msrException : public mfException
{
  public:
    msdl2msrException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdl2msrException> S_msdl2msrException;

class EXP msdl2lilyponException: public mfException
{
  public:
    msdl2lilyponException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdl2lilyponException> S_msdl2lilyponException;

class EXP msdl2brailleException: public mfException
{
  public:
    msdl2brailleException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdl2brailleException> S_msdl2brailleException;

class EXP msdl2musicxmlException: public mfException
{
  public:
    msdl2musicxmlException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdl2musicxmlException> S_msdl2musicxmlException;

//______________________________________________________________________________
EXP void msdlWarning (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& message);

EXP void msdlError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);

EXP void msdlInternalError (
  const string& inputSourceName,
  int           inputLineNumber,
  const string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const string& message);


}


#endif
