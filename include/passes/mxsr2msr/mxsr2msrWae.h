/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2msrWae___
#define ___mxsr2msrWae___

#include "exports.h"

#include "mfBasicTypes.h"

#include "mfExceptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
class EXP mxsr2msrException : public mfException
{
  public:
    mxsr2msrException (std::string const& exceptionDescription) throw ()
      : mfException (exceptionDescription)
    {}
};
typedef SMARTP<mxsr2msrException> S_mxsr2msrException;

//______________________________________________________________________________
// warnings

EXP void mxsr2msrWarningWithoutInputLocation (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message);

EXP void mxsr2msrWarning (
  const std::string&       inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string&       message);

// internal warnings

EXP void mxsr2msrInternalWarning (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string& message);

// errors

EXP void mxsr2msrError (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       message);

// internal errors

EXP void mxsr2msrInternalError (
  const std::string& inputSourceName,
  const mfInputLineNumber& inputLineNumber,
  const std::string&       sourceCodeFileName,
  const mfInputLineNumber& sourceCodeLineNumber,
  const std::string&       message);


}


#endif // ___mxsr2msrWae___
