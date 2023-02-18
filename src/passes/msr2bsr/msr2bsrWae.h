/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2bsrWae___
#define ___msr2bsrWae___

#include "exports.h"

#include "mfStringsHandling.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msr2bsrException: public mfException
{
  public:
    msr2bsrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msr2bsrException> S_msr2bsrException;

class EXP msr2bsrInternalException: public mfException
{
  public:
    msr2bsrInternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msr2bsrInternalException> S_msr2bsrInternalException;

class EXP msr2bsrUnsupportedException: public mfException
{
  public:
    msr2bsrUnsupportedException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msr2bsrUnsupportedException> S_msr2bsrUnsupportedException;

//______________________________________________________________________________
EXP void msr2bsrUnsupported (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);

//______________________________________________________________________________
EXP void msr2bsrInternalWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message);

EXP void msr2bsrInternalError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);

//______________________________________________________________________________
EXP void msr2bsrWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message);

EXP void msr2bsrError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);


}


#endif // ___msr2bsrWae___
