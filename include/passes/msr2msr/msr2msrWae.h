/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___msr2msrWae___
#define ___msr2msrWae___

#include "exports.h"

#include "mfStringsHandling.h"

#include "mfExceptions.h"

#include "waeInterface.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP msr2msrException: public mfException
{
  public:
    msr2msrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msr2msrException> S_msr2msrException;

class EXP msr2msrInternalException: public mfException
{
  public:
    msr2msrInternalException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msr2msrInternalException> S_msr2msrInternalException;

class EXP msr2msrUnsupportedException: public mfException
{
  public:
    msr2msrUnsupportedException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<msr2msrUnsupportedException> S_msr2msrUnsupportedException;

//______________________________________________________________________________
EXP void msr2msrUnsupported (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);

//______________________________________________________________________________
EXP void msr2msrInternalWarning (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message);

EXP void msr2msrInternalError (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);

//______________________________________________________________________________
EXP void msr2msrWarning (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& message);

EXP void msr2msrError (
  const std::string& inputSourceName,
  int                inputLineNumber,
  const std::string& sourceCodeFileName,
  int                sourceCodeLineNumber,
  const std::string& message);


}


#endif
