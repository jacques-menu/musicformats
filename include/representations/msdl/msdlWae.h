/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msdlWae___
#define ___msdlWae___

#include "exports.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msdlException : public mfException
{
  public:
    msdlException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdlException> S_msdlException;

class EXP msdlInternalException: public mfException
{
  public:
    msdlInternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdlInternalException> S_msdlInternalException;

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

class EXP msdl2msdrException : public mfException
{
  public:
    msdl2msdrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdl2msdrException> S_msdl2msdrException;

class EXP msdl2msrInternalException: public mfException
{
  public:
    msdl2msrInternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdl2msrInternalException> S_msdl2msrInternalException;

class EXP msdl2msrException : public mfException
{
  public:
    msdl2msrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdl2msrException> S_msdl2msrException;

class EXP msdl2lilyponException: public mfException
{
  public:
    msdl2lilyponException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdl2lilyponException> S_msdl2lilyponException;

class EXP msdl2brailleException: public mfException
{
  public:
    msdl2brailleException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdl2brailleException> S_msdl2brailleException;

class EXP msdl2musicxmlException: public mfException
{
  public:
    msdl2musicxmlException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msdl2musicxmlException> S_msdl2musicxmlException;

//______________________________________________________________________________
EXP void msdlWarning (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message);

EXP void msdlError (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       message);

EXP void msdlInternalError (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       message);


}


#endif // ___msdlWae___
