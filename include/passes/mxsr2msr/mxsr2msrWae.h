/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___mxsr2msrWae___
#define ___mxsr2msrWae___

#include "exports.h"

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
EXP void mxsr2msrWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message);

EXP void mxsr2msrInternalWarning (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& message);

EXP void mxsr2msrError (
  const std::string& inputSourceName,
  int           inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);

EXP void mxsr2msrInternalError (
  const std::string& inputSourceName,
  int          inputLineNumber,
  const std::string& sourceCodeFileName,
  int           sourceCodeLineNumber,
  const std::string& message);


}


#endif
