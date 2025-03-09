/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mfslWae___
#define ___mfslWae___

#include "exports.h"

#include "mfExceptions.h"

#include "oahBasicTypes.h"

#include "location.hh"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP mfslException : public mfException
{
  public:
    mfslException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfslException> S_mfslException;

class EXP mfslnternalException: public mfException
{
  public:
    mfslnternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfslnternalException> S_mfslnternalException;

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

class EXP mfsl2msdrException : public mfException
{
  public:
    mfsl2msdrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfsl2msdrException> S_mfsl2msdrException;

class EXP mfsl2msrInternalException: public mfException
{
  public:
    mfsl2msrInternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfsl2msrInternalException> S_mfsl2msrInternalException;

class EXP mfsl2msrException : public mfException
{
  public:
    mfsl2msrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfsl2msrException> S_mfsl2msrException;

class EXP mfsl2lilyponException: public mfException
{
  public:
    mfsl2lilyponException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfsl2lilyponException> S_mfsl2lilyponException;

class EXP mfsl2brailleException: public mfException
{
  public:
    mfsl2brailleException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfsl2brailleException> S_mfsl2brailleException;

class EXP mfsl2musicxmlException: public mfException
{
  public:
    mfsl2musicxmlException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mfsl2musicxmlException> S_mfsl2musicxmlException;

//______________________________________________________________________________
EXP void mfslWarning (
  const std::string&       message,
  const mfsl::location& loc);

EXP void mfslError (
  const std::string&       message,
  const mfsl::location& loc);

EXP void mfslInternalError (
  const std::string&       message,
  const mfsl::location& loc);

EXP void mfslFileError (
  const std::string& inputSourceName,
  const std::string& message);

EXP void mfslOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2);


}


#endif // ___mfslWae___
