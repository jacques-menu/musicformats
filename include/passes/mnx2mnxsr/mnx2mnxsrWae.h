/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mnx2mnxsrWae___
#define ___mnx2mnxsrWae___

#include "exports.h"

#include "mfExceptions.h"

#include "oahBasicTypes.h"

#include "location.hh"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
class EXP mnx2mnxsrException : public mfException
{
  public:
    mnx2mnxsrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mnx2mnxsrException> S_mnx2mnxsrException;

class EXP mnx2mnxsrnternalException: public mfException
{
  public:
    mnx2mnxsrnternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mnx2mnxsrnternalException> S_mnx2mnxsrnternalException;

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

class EXP mnx2mnxsr2msdrException : public mfException
{
  public:
    mnx2mnxsr2msdrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mnx2mnxsr2msdrException> S_mnx2mnxsr2msdrException;

class EXP mnx2mnxsr2msrInternalException: public mfException
{
  public:
    mnx2mnxsr2msrInternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mnx2mnxsr2msrInternalException> S_mnx2mnxsr2msrInternalException;

class EXP mnx2mnxsr2msrException : public mfException
{
  public:
    mnx2mnxsr2msrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mnx2mnxsr2msrException> S_mnx2mnxsr2msrException;

class EXP mnx2mnxsr2lilyponException: public mfException
{
  public:
    mnx2mnxsr2lilyponException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mnx2mnxsr2lilyponException> S_mnx2mnxsr2lilyponException;

class EXP mnx2mnxsr2brailleException: public mfException
{
  public:
    mnx2mnxsr2brailleException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mnx2mnxsr2brailleException> S_mnx2mnxsr2brailleException;

class EXP mnx2mnxsr2musicxmlException: public mfException
{
  public:
    mnx2mnxsr2musicxmlException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mnx2mnxsr2musicxmlException> S_mnx2mnxsr2musicxmlException;

//______________________________________________________________________________
EXP void mnx2mnxsrWarning (
  const std::string&       message,
  const mnx2mnxsr::location& loc);

EXP void mnx2mnxsrError (
  const std::string&       message,
  const mnx2mnxsr::location& loc);

EXP void mnx2mnxsrInternalError (
  const std::string&       message,
  const mnx2mnxsr::location& loc);

EXP void mnx2mnxsrFileError (
  const std::string& inputSourceName,
  const std::string& message);

EXP void mnx2mnxsrOptionsIncompatibilityError (
  const S_oahAtom atom1,
  const S_oahAtom atom2);


}


#endif // ___mnx2mnxsrWae___
