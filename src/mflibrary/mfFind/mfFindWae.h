/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfFindWae___
#define ___mfFindWae___

#include "exports.h"

#include "mfExceptions.h"

#include "oahBasicTypes.h"

#include "location.hh"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP mfFindException : public mfException
{
  public:
    mfFindException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFindException> S_mfFindException;

class EXP mfFindnternalException: public mfException
{
  public:
    mfFindnternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFindnternalException> S_mfFindnternalException;

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

class EXP mfFind2msdrException : public mfException
{
  public:
    mfFind2msdrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFind2msdrException> S_mfFind2msdrException;

class EXP mfFind2msrInternalException: public mfException
{
  public:
    mfFind2msrInternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFind2msrInternalException> S_mfFind2msrInternalException;

class EXP mfFind2msrException : public mfException
{
  public:
    mfFind2msrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFind2msrException> S_mfFind2msrException;

class EXP mfFind2lilyponException: public mfException
{
  public:
    mfFind2lilyponException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFind2lilyponException> S_mfFind2lilyponException;

class EXP mfFind2brailleException: public mfException
{
  public:
    mfFind2brailleException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFind2brailleException> S_mfFind2brailleException;

class EXP mfFind2musicxmlException: public mfException
{
  public:
    mfFind2musicxmlException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfFind2musicxmlException> S_mfFind2musicxmlException;

//______________________________________________________________________________
EXP void mfFindWarning (
  const std::string&       message,
  const mfFind::location& loc);

EXP void mfFindError (
  const std::string&       message,
  const mfFind::location& loc);

EXP void mfFindInternalError (
  const std::string&       message,
  const mfFind::location& loc);

EXP void mfFindFileError (
  const std::string& inputSourceName,
  const std::string& message);

EXP void mfFindOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2);


}


#endif // ___mfFindWae___
