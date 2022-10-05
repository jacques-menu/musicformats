/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___ischemeWae___
#define ___ischemeWae___

#include "exports.h"

#include "mfExceptions.h"

#include "oahBasicTypes.h"

#include "location.hh"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP ischemeException : public mfException
{
  public:
    ischemeException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<ischemeException> S_ischemeException;

class EXP ischementernalException: public mfException
{
  public:
    ischementernalException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<ischementernalException> S_ischementernalException;

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

class EXP ischeme2msdrException : public mfException
{
  public:
    ischeme2msdrException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<ischeme2msdrException> S_ischeme2msdrException;

class EXP ischeme2msrInternalException: public mfException
{
  public:
    ischeme2msrInternalException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<ischeme2msrInternalException> S_ischeme2msrInternalException;

class EXP ischeme2msrException : public mfException
{
  public:
    ischeme2msrException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<ischeme2msrException> S_ischeme2msrException;

class EXP ischeme2lilyponException: public mfException
{
  public:
    ischeme2lilyponException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<ischeme2lilyponException> S_ischeme2lilyponException;

class EXP ischeme2brailleException: public mfException
{
  public:
    ischeme2brailleException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<ischeme2brailleException> S_ischeme2brailleException;

class EXP ischeme2musicxmlException: public mfException
{
  public:
    ischeme2musicxmlException (string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<ischeme2musicxmlException> S_ischeme2musicxmlException;

//______________________________________________________________________________
EXP void ischemeWarning (
  const string&       message,
  const iscm::location& loc);

EXP void ischemeError (
  const string&       message,
  const iscm::location& loc);

EXP void ischemeInternalError (
  const string&       message,
  const iscm::location& loc);

EXP void ischemeFileError (
  const string& inputSourceName,
  const string& message);

EXP void ischemeOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2);


}


#endif
