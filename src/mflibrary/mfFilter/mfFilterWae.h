/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfFilterWae___
#define ___mfFilterWae___

#include "exports.h"

#include "mfExceptions.h"

#include "oahBasicTypes.h"

#include "location.hh"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP mfFilterException : public mfException
{
  public:
    mfFilterException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFilterException> S_mfFilterException;

class EXP mfFilternternalException: public mfException
{
  public:
    mfFilternternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFilternternalException> S_mfFilternternalException;

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

class EXP mfFilter2msdrException : public mfException
{
  public:
    mfFilter2msdrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFilter2msdrException> S_mfFilter2msdrException;

class EXP mfFilter2msrInternalException: public mfException
{
  public:
    mfFilter2msrInternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFilter2msrInternalException> S_mfFilter2msrInternalException;

class EXP mfFilter2msrException : public mfException
{
  public:
    mfFilter2msrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFilter2msrException> S_mfFilter2msrException;

class EXP mfFilter2lilyponException: public mfException
{
  public:
    mfFilter2lilyponException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFilter2lilyponException> S_mfFilter2lilyponException;

class EXP mfFilter2brailleException: public mfException
{
  public:
    mfFilter2brailleException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFilter2brailleException> S_mfFilter2brailleException;

class EXP mfFilter2musicxmlException: public mfException
{
  public:
    mfFilter2musicxmlException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFilter2musicxmlException> S_mfFilter2musicxmlException;

//______________________________________________________________________________
EXP void mfFilterWarning (
  const std::string&       message,
  const mfFilter::location& loc);

EXP void mfFilterError (
  const std::string&       message,
  const mfFilter::location& loc);

EXP void mfFilterInternalError (
  const std::string&       message,
  const mfFilter::location& loc);

EXP void mfFilterFileError (
  const std::string& inputSourceName,
  const std::string& message);

EXP void mfFilterOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2);


}


#endif // ___mfFilterWae___
