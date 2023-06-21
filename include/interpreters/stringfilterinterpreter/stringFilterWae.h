/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___stringFilterWae___
#define ___stringFilterWae___

#include "exports.h"

#include "mfExceptions.h"

#include "oahBasicTypes.h"

#include "location.hh"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP stringFilterException : public mfException
{
  public:
    stringFilterException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringFilterException> S_stringFilterException;

class EXP stringFilternternalException: public mfException
{
  public:
    stringFilternternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringFilternternalException> S_stringFilternternalException;

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

class EXP stringFilter2msdrException : public mfException
{
  public:
    stringFilter2msdrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringFilter2msdrException> S_stringFilter2msdrException;

class EXP stringFilter2msrInternalException: public mfException
{
  public:
    stringFilter2msrInternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringFilter2msrInternalException> S_stringFilter2msrInternalException;

class EXP stringFilter2msrException : public mfException
{
  public:
    stringFilter2msrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringFilter2msrException> S_stringFilter2msrException;

class EXP stringFilter2lilyponException: public mfException
{
  public:
    stringFilter2lilyponException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringFilter2lilyponException> S_stringFilter2lilyponException;

class EXP stringFilter2brailleException: public mfException
{
  public:
    stringFilter2brailleException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringFilter2brailleException> S_stringFilter2brailleException;

class EXP stringFilter2musicxmlException: public mfException
{
  public:
    stringFilter2musicxmlException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<stringFilter2musicxmlException> S_stringFilter2musicxmlException;

//______________________________________________________________________________
EXP void stringFilterWarning (
  const std::string&       message,
  const stringfilter::location& loc);

EXP void stringFilterError (
  const std::string&       message,
  const stringfilter::location& loc);

EXP void stringFilterInternalError (
  const std::string&       message,
  const stringfilter::location& loc);

EXP void stringFilterFileError (
  const std::string& inputSourceName,
  const std::string& message);

EXP void stringFilterOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2);


}


#endif // ___stringFilterWae___
