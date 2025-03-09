/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___stringMatcherWae___
#define ___stringMatcherWae___

#include "exports.h"

#include "mfExceptions.h"

#include "oahBasicTypes.h"

#include "location.hh"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP stringMatcherException : public mfException
{
  public:
    stringMatcherException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringMatcherException> S_stringMatcherException;

class EXP stringMatcherInternalException: public mfException
{
  public:
    stringMatcherInternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringMatcherInternalException> S_stringMatcherInternalException;

class EXP msdrException : public mfException
{
  public:
    msdrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdrException> S_msdrException;

class EXP msdrInternalException: public mfException
{
  public:
    msdrInternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdrInternalException> S_msdrInternalException;

class EXP stringMatcher2msdrException : public mfException
{
  public:
    stringMatcher2msdrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringMatcher2msdrException> S_stringMatcher2msdrException;

class EXP stringMatcher2msrInternalException: public mfException
{
  public:
    stringMatcher2msrInternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringMatcher2msrInternalException> S_stringMatcher2msrInternalException;

class EXP stringMatcher2msrException : public mfException
{
  public:
    stringMatcher2msrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringMatcher2msrException> S_stringMatcher2msrException;

class EXP stringMatcher2lilyponException: public mfException
{
  public:
    stringMatcher2lilyponException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringMatcher2lilyponException> S_stringMatcher2lilyponException;

class EXP stringMatcher2brailleException: public mfException
{
  public:
    stringMatcher2brailleException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringMatcher2brailleException> S_stringMatcher2brailleException;

class EXP stringMatcher2musicxmlException: public mfException
{
  public:
    stringMatcher2musicxmlException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringMatcher2musicxmlException> S_stringMatcher2musicxmlException;

//______________________________________________________________________________
EXP void stringMatcherWarning (
  const std::string&       message,
  const smi::location& loc);

EXP void stringMatcherError (
  const std::string&       message,
  const smi::location& loc);

EXP void stringMatcherInternalError (
  const std::string&       message,
  const smi::location& loc);

EXP void stringMatcherFileError (
  const std::string& inputSourceName,
  const std::string& message);

EXP void stringMatcherOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2);


}


#endif // ___stringMatcherWae___
